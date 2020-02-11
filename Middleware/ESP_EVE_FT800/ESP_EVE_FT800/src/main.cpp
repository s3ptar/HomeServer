/* Heltec Automation I2C scanner example (also it's a basic example how to use I2C1)
 *
 * ESP32 have two I2C (I2C0 and I2C1) bus
 *
 * OLED is connected to I2C0, so if scan with Wire (I2C0), the return address should be 0x3C.
 *
 * If you need scan other device address in I2C1...
 *		- Comment all Wire.***() codes;
 * 		- Uncomment all Wire1.***() codes;
 *
 * I2C scan example and I2C0
 *
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/Heltec_ESP32
 * */

#include <heltec.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPI.h>				// Arduino SPI Library definitions
#include "FT800.h"				// FT800 register, memory and command values

// Set LCD display resolution here
//#define LCD_QVGA				// QVGA  = 320 x 240 (VM800B/C 3.5")
#define LCD_WQVGA				// WQVGA = 480 x 272 (VM800B/C 4.3" and 5.0")

// FT800 Chip Commands - use with cmdWrite
#define FT800_ACTIVE	0x00			// Initializes FT800
#define FT800_STANDBY	0x41			// Place FT800 in Standby (clk running)
#define FT800_SLEEP	0x42			// Place FT800 in Sleep (clk off)
#define FT800_PWRDOWN	0x50			// Place FT800 in Power Down (core off)
#define FT800_CLKEXT	0x44			// Select external clock source
#define FT800_CLK48M	0x62			// Select 48MHz PLL
#define FT800_CLK36M	0x61			// Select 36MHz PLL
#define FT800_CORERST	0x68			// Reset core - all registers default

// FT800 Memory Commands - use with ft800memWritexx and ft800memReadxx
#define MEM_WRITE	0x80			// FT800 Host Memory Write 
#define MEM_READ	0x00			// FT800 Host Memory Read

// Colors - fully saturated colors defined here
#define RED		0xFF0000UL		// Red
#define GREEN		0x00FF00UL		// Green
#define BLUE		0x0000FFUL		// Blue
#define WHITE		0xFFFFFFUL		// White
#define BLACK		0x000000UL		// Black

// Global Variables

// Arduino pins - others defined by Serial and SPI libraries
unsigned int triggerPin;			// Used for oscilloscope/logic analyzer trigger
unsigned int ft800irqPin;			// Interrupt from FT800 to Arduino - not used here
unsigned int ft800pwrPin;			// PD_N from Arduino to FT800 - effectively FT800 reset
unsigned int ft800csPin;			// SPI chip select - defined separately since it's manipulated with GPIO calls

// LCD display parameters
unsigned int lcdWidth;				// Active width of LCD display
unsigned int lcdHeight;				// Active height of LCD display
unsigned int lcdHcycle;				// Total number of clocks per line
unsigned int lcdHoffset;			// Start of active line
unsigned int lcdHsync0;				// Start of horizontal sync pulse
unsigned int lcdHsync1;				// End of horizontal sync pulse
unsigned int lcdVcycle;				// Total number of lines per screen
unsigned int lcdVoffset;			// Start of active screen
unsigned int lcdVsync0;				// Start of vertical sync pulse
unsigned int lcdVsync1;				// End of vertical sync pulse
unsigned char lcdPclk;				// Pixel Clock
unsigned char lcdSwizzle;			// Define RGB output pins
unsigned char lcdPclkpol;			// Define active edge of PCLK

unsigned long ramDisplayList = RAM_DL;		// Set beginning of display list memory 
unsigned long ramCommandBuffer = RAM_CMD;	// Set beginning of graphics command memory

unsigned int cmdBufferRd = 0x0000;		// Used to navigate command ring buffer
unsigned int cmdBufferWr = 0x0000;		// Used to navigate command ring buffer
unsigned int cmdOffset = 0x0000;		// Used to navigate command rung buffer
unsigned int point_size = 0x0100;		// Define a default dot size
unsigned long point_x = (96 * 16);		// Define a default point x-location (1/16 anti-aliased)
unsigned long point_y = (136 * 16);		// Define a default point y-location (1/16 anti-aliased)
unsigned long color;				// Variable for chanign colors
unsigned char ft800Gpio;			// Used for FT800 GPIO register


const char* ssid = "chilihotdog";
const char* password = "bxJHckMMkGqEPfY3Jf3nZnAn5FtGYwKZSkzVvbzFHNbpUZfv79GXm8afDuNu";
char str_display[32];
SSD1306Wire *display;



/******************************************************************************
 * Function:        void ft800memWritexx(ftAddress, ftDataxx, ftLength)
 * PreCondition:    None
 * Input:           ftAddress = FT800 memory space address
 *                  ftDataxx = a byte, int or long to send
 * Output:          None
 * Side Effects:    None
 * Overview:        Writes FT800 internal address space
 * Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
void ft800memWrite8(unsigned long ftAddress, unsigned char ftData8)
{
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ftData8);			// Send data byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}

void ft800memWrite16(unsigned long ftAddress, unsigned int ftData16)
{
//  digitalWrite(triggerPin, HIGH);		// Toggle a pin to trigger the oscilloscope
//  digitalWrite(triggerPin, LOW);		// Toggle a pin to trigger the oscilloscope
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer((char)(ftData16));		// Send data low byte
  SPI.transfer((char)(ftData16 >> 8));		// Send data high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}

void ft800memWrite32(unsigned long ftAddress, unsigned long ftData32)
{
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer((char)(ftData32));		// Send data low byte
  SPI.transfer((char)(ftData32 >> 8));
  SPI.transfer((char)(ftData32 >> 16));
  SPI.transfer((char)(ftData32 >> 24));		// Send data high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}
/******************************************************************************
 * Function:        unsigned char ft800memReadxx(ftAddress, ftLength)
 * PreCondition:    None
 * Input:           ftAddress = FT800 memory space address
 * Output:          ftDataxx (byte, int or long)
 * Side Effects:    None
 * Overview:        Reads FT800 internal address space
 * Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
unsigned char ft800memRead8(unsigned long ftAddress)
{
  unsigned char ftData8 = ZERO;
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ZERO);				// Send dummy byte
ftData8 = SPI.transfer(ZERO);		// Read data byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
  return ftData8;				// Return byte read
}

unsigned int ft800memRead16(unsigned long ftAddress)
{
  unsigned int ftData16;
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ZERO);				// Send dummy byte
    ftData16 = (SPI.transfer(ZERO));		// Read low byte
    ftData16 = (SPI.transfer(ZERO) << 8) | ftData16; // Read high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS#hHigh
  return ftData16;				// Return integer read
}

unsigned long ft800memRead32(unsigned long ftAddress)
{
  unsigned long ftData32;
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ZERO);				// Send dummy byte
    ftData32 = (SPI.transfer(ZERO));		// Read low byte
    ftData32 = (SPI.transfer(ZERO) << 8) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 16) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 24) | ftData32; // Read high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
  return ftData32;				// Return long read
}
/******************************************************************************
 * Function:        void ft800cmdWrite(ftCommand)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Sends FT800 command
 * Note:            None
 *****************************************************************************/
void ft800cmdWrite(unsigned char ftCommand)
{
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer(ftCommand);			// Send command
  SPI.transfer(0x00);				// Commands consist of two more zero bytes
  SPI.transfer(0x00);				// Send last zero byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}

/******************************************************************************
 * Function:        void incCMDOffset(currentOffset, commandSize)
 * PreCondition:    None
 *                    starting a command list
 * Input:           currentOffset = graphics processor command list pointer
 *                  commandSize = number of bytes to increment the offset
 * Output:          newOffset = the new ring buffer pointer after adding the command
 * Side Effects:    None
 * Overview:        Adds commandSize to the currentOffset.  
 *                  Checks for 4K ring-buffer offset roll-over 
 * Note:            None
 *****************************************************************************/
unsigned int incCMDOffset(unsigned int currentOffset, unsigned char commandSize)
{
    unsigned int newOffset;			// used to hold new offset
    newOffset = currentOffset + commandSize;	// Calculate new offset
    if(newOffset > 4095)			// If new offset past boundary...
    {
        newOffset = (newOffset - 4096);		// ... roll over pointer
    }
    return newOffset;				// Return new offset
}

/******************************************************************************
 * Function:        void setup(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        System initialization routines.  Executed once on reset.
 * Note:            Default Arduino function
 *****************************************************************************/
  /* The Arduino I/O pins are connected to the VM800B/C as follows:
      Digital
        Pin 0 = TXD  (out)   Pin 8    = n/c
        Pin 1 = RXD  (in)    Pin 9    = n/c
        Pin 2 = n/c          Pin 10   = CS#  (out)
        Pin 3 = INT# (in)    Pin 23   = MOSI (out)
        Pin 4 = PD#  (out)   Pin 19   = MISO (in)
        Pin 5 = n/c          Pin 18   = SCK  (out)
        pin 6 = n/c          Pin GND  = GND
        pin 7 = n/c          Pin AREF = n/c
        
      Analog and power headers = n/c
  */
  
void setup()
{

	ft800csPin = 13;
	triggerPin = 2;				// Used for oscilloscope/logic analyzer trigger
  	ft800irqPin = 3;				// Interrupt from FT800 to Arduino - not used here
  	ft800pwrPin = 12;				// PD_N from Arduino to FT800 - effectively FT800 reset
  	lcdWidth   = 480;				// Active width of LCD display
  	lcdHeight  = 272;				// Active height of LCD display
  	lcdHcycle  = 548;				// Total number of clocks per line
  	lcdHoffset = 43;				// Start of active line
  	lcdHsync0  = 0;				// Start of horizontal sync pulse
  	lcdHsync1  = 41;				// End of horizontal sync pulse
  	lcdVcycle  = 292;				// Total number of lines per screen
  	lcdVoffset = 12;				// Start of active screen
  	lcdVsync0  = 0;				// Start of vertical sync pulse
  	lcdVsync1  = 10;				// End of vertical sync pulse
  	lcdPclk    = 5;				// Pixel Clock
  	lcdSwizzle = 0;				// Define RGB output pins
  	lcdPclkpol = 1;				// Define active edge of PCLKft800csPin  = 10;				// SPI chip select - defined separately since it's manipulated with GPIO calls

	Heltec.begin(true, false, true);
	Serial.println("Booting...");
//int8_t sck, int8_t miso, int8_t mosi, int8_t ss
	SPI.begin(18,19,23,36);					// Initialize SPI
  	SPI.setBitOrder(MSBFIRST);			// Send data most significant bit first
  	SPI.setDataMode(SPI_MODE0);			// Clock idle zero, clock data into FT800 on rising edge
  	SPI.setClockDivider(SPI_CLOCK_DIV4);		// Set rate at 4MHz (16MHz OSC / 4)
    
  	pinMode(triggerPin, OUTPUT);			// Arduino pin used for oscilloscope triggering
  	pinMode(ft800irqPin, INPUT_PULLUP);		// FT800 interrupt output (not used in this example)
  	pinMode(ft800pwrPin, OUTPUT);			// FT800 Power Down (reset) input
  	pinMode(ft800csPin, OUTPUT);			// FT800 SPI bus CS# input
  
  	digitalWrite(triggerPin, LOW);		// Initialize the oscilloscope trigger
  	digitalWrite(ft800csPin, HIGH);		// Set CS# high to start - SPI inactive
  	digitalWrite(ft800pwrPin, HIGH);		// Set PD# high to start
  	delay(20);					// Wait a few MS before waking the FT800

	//***************************************
	// Wake-up FT800
  
  	digitalWrite(ft800pwrPin, LOW);		// 1) lower PD#
  	delay(20);					// 2) hold for 20ms
  	digitalWrite(ft800pwrPin, HIGH);		// 3) raise PD#
  	delay(20);					// 4) wait for another 20ms before sending any commands

  	ft800cmdWrite(FT800_ACTIVE);			// Start FT800
  	delay(5);					// Give some time to process
  	ft800cmdWrite(FT800_CLKEXT);			// Set FT800 for external clock
  	delay(5);					// Give some time to process
  	ft800cmdWrite(FT800_CLK48M);			// Set FT800 for 48MHz PLL
  	delay(5);					// Give some time to process
  						// Now FT800 can accept commands at up to 30MHz clock on SPI bus
  						//   This application leaves the SPI bus at 4MHz
	Serial.write("Check FT800!\r\n");		// Send an error message on the UART
  	while (ft800memRead8(REG_ID) != 0x7C)		// Read ID register - is it 0x7C?
  	{
    	Serial.write("System Halted!\r\n");		// Send an error message on the UART
    	//while(1);					// If we don't get 0x7C, the ineface isn't working - halt with infinite loop
  	}  
  
  	ft800memWrite8(REG_PCLK, ZERO);		// Set PCLK to zero - don't clock the LCD until later
  	ft800memWrite8(REG_PWM_DUTY, ZERO);		// Turn off backlight
  
	// End of Wake-up FT800
	//***************************************

	//***************************************
	// Initialize Display
  	ft800memWrite16(REG_HSIZE,   lcdWidth);	// active display width
  	ft800memWrite16(REG_HCYCLE,  lcdHcycle);	// total number of clocks per line, incl front/back porch
  	ft800memWrite16(REG_HOFFSET, lcdHoffset);	// start of active line
  	ft800memWrite16(REG_HSYNC0,  lcdHsync0);	// start of horizontal sync pulse
  	ft800memWrite16(REG_HSYNC1,  lcdHsync1);	// end of horizontal sync pulse
  	ft800memWrite16(REG_VSIZE,   lcdHeight);	// active display height
  	ft800memWrite16(REG_VCYCLE,  lcdVcycle);	// total number of lines per screen, incl pre/post
  	ft800memWrite16(REG_VOFFSET, lcdVoffset);	// start of active screen
  	ft800memWrite16(REG_VSYNC0,  lcdVsync0);	// start of vertical sync pulse
  	ft800memWrite16(REG_VSYNC1,  lcdVsync1);	// end of vertical sync pulse
  	ft800memWrite8(REG_SWIZZLE,  lcdSwizzle);	// FT800 output to LCD - pin order
  	ft800memWrite8(REG_PCLK_POL, lcdPclkpol);	// LCD data is clocked in on this PCLK edge
						// Don't set PCLK yet - wait for just after the first display list
	// End of Initialize Display
	//***************************************

	//***************************************
	// Configure Touch and Audio - not used in this example, so disable both
  	ft800memWrite8(REG_TOUCH_MODE, ZERO);		// Disable touch
  	ft800memWrite16(REG_TOUCH_RZTHRESH, ZERO);	// Eliminate any false touches
  
  	ft800memWrite8(REG_VOL_PB, ZERO);		// turn recorded audio volume down
 	ft800memWrite8(REG_VOL_SOUND, ZERO);		// turn synthesizer volume down
  	ft800memWrite16(REG_SOUND, 0x6000);		// set synthesizer to mute
  
	// End of Configure Touch and Audio
	//***************************************

	//***************************************
	// Write Initial Display List & Enable Display

  	ramDisplayList = RAM_DL;			// start of Display List
  	ft800memWrite32(ramDisplayList, DL_CLEAR_RGB); // Clear Color RGB   00000010 RRRRRRRR GGGGGGGG BBBBBBBB  (R/G/B = Colour values) default zero / black
  	ramDisplayList += 4;				// point to next location
  	ft800memWrite32(ramDisplayList, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));	// Clear 00100110 -------- -------- -----CST  (C/S/T define which parameters to clear)
  	ramDisplayList += 4;				// point to next location
  	ft800memWrite32(ramDisplayList, DL_DISPLAY);	// DISPLAY command 00000000 00000000 00000000 00000000 (end of display list)

  	ft800memWrite32(REG_DLSWAP, DLSWAP_FRAME);	// 00000000 00000000 00000000 000000SS  (SS bits define when render occurs)
						// Nothing is being displayed yet... the pixel clock is still 0x00
  	ramDisplayList = RAM_DL;			// Reset Display List pointer for next list
  
  	ft800Gpio = ft800memRead8(REG_GPIO);		// Read the FT800 GPIO register for a read/modify/write operation
  	ft800Gpio = ft800Gpio | 0x80;			// set bit 7 of FT800 GPIO register (DISP) - others are inputs
  	ft800memWrite8(REG_GPIO, ft800Gpio);		// Enable the DISP signal to the LCD panel
  	ft800memWrite8(REG_PCLK, lcdPclk);		// Now start clocking data to the LCD panel
  	for(int duty = 128; duty > 0; duty--)
  	{
    	ft800memWrite8(REG_PWM_DUTY, duty);		// Turn on backlight - ramp up slowly to full brighness
    	delay(10);
  	}

	
	Wire.begin(SDA_OLED, SCL_OLED); //Scan OLED's I2C address via I2C0
	//Wire1.begin(SDA, SCL);        //If there have other device on I2C1, scan the device address via I2C1
	WiFi.mode(WIFI_STA);
  	WiFi.begin(ssid, password);
  	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    	Serial.println("Connection Failed! Rebooting...");
    	delay(5000);
    	ESP.restart();
  	}
	

// Port defaults to 3232
// ArduinoOTA.setPort(3232);

// Hostname defaults to esp3232-[MAC]
// ArduinoOTA.setHostname("myesp32");

// No authentication by default
// ArduinoOTA.setPassword("admin");

// Password can be set with it's md5 value as well
// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
// ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

ArduinoOTA
    .onStart([]() {
    	String type;
      	if (ArduinoOTA.getCommand() == U_FLASH)
        	type = "sketch";
      	else // U_SPIFFS
        	type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    	Serial.println("Start updating " + type);
    })
    .onEnd([]() {
    	Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
		sprintf(&str_display[0], "Progress: %u%%\r", (progress / (total / 100)));
    	Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		Heltec.display->clear();
		Heltec.display->setFont(ArialMT_Plain_10);
		Heltec.display->drawString(0, 0, &str_display[0]);
		Heltec.display->display();
    })
    .onError([](ota_error_t error) {
     	Serial.printf("Error[%u]: ", error);
     	if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
     	else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
     	else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
     	else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    	else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  	ArduinoOTA.begin();

  	Serial.println("Ready");
  	Serial.print("IP address: ");
  	Serial.println(WiFi.localIP());
}


void loop()
{
	byte error, address;
	int nDevices;
	ArduinoOTA.handle();

	Heltec.display->clear();
	Heltec.display->setFont(ArialMT_Plain_10);
	Heltec.display->drawString(0, 0, WiFi.localIP().toString());
	Heltec.display->display();

	Serial.println("Scanning...");

	nDevices = 0;
	for(address = 1; address < 127; address++ )
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

//		Wire1.beginTransmission(address);
//		error = Wire1.endTransmission();

		if (error == 0)
		{
			Serial.print("I2C device found at address 0x");
			if (address<16)
			Serial.print("0");
			Serial.print(address,HEX);
			Serial.println("  !");

			nDevices++;
		}
		else if (error==4)
		{
			Serial.print("Unknown error at address 0x");
			if (address<16)
				Serial.print("0");
			Serial.println(address,HEX);
		}
	}
	if (nDevices == 0)
	Serial.println("No I2C devices found\n");
	else
	Serial.println("done\n");

	// Wait for graphics processor to complete executing the current command list
  	// This happens when REG_CMD_READ matches REG_CMD_WRITE, indicating that all commands
  	// have been executed.  The next commands get executed when REG_CMD_WRITE is updated again...
  	// then REG_CMD_READ again catches up to REG_CMD_WRITE
  	// This is a 4Kbyte ring buffer, so keep pointers within the 4K roll-over
 	 do
  	{
    	cmdBufferRd = ft800memRead16(REG_CMD_READ);	// Read the graphics processor read pointer
    	cmdBufferWr = ft800memRead16(REG_CMD_WRITE); // Read the graphics processor write pointer
  	}while (cmdBufferWr != cmdBufferRd);		// Wait until the two registers match
  
  	cmdOffset = cmdBufferWr;			// The new starting point the first location after the last command

  	if (color != WHITE)				// If a red dot was just drawn (or first time through)...
    	color = WHITE;				// change color to white
  	else						// Otherwise...
    	color = RED;				// change the color to red
    
  	ft800memWrite32(RAM_CMD + cmdOffset, (CMD_DLSTART));
						// Start the display list
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer
  

  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR_RGB | BLACK));
						// Set the default clear color to black
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
						// Clear the screen - this and the previous prevent artifacts between lists
						// Attributes are the color, stencil and tag buffers
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_COLOR_RGB | color));
						// Set the color of the following item(s) - toggle red/white from above
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_POINT_SIZE | point_size));
						// Select the size of the dot to draw
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_BEGIN | FTPOINTS));
						// Indicate to draw a point (dot)
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_VERTEX2F | (point_x << 15) | point_y));
						// Set the point center location
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_END));
						// End the point
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (DL_DISPLAY));
						// Instruct the graphics processor to show the list
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite32(RAM_CMD + cmdOffset, (CMD_SWAP));
						// Make this list active
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800memWrite16(REG_CMD_WRITE, (cmdOffset));	// Update the ring buffer pointer so the graphics processor starts executing

  	delay(500);					// Wait a half-second to observe the changing color
}