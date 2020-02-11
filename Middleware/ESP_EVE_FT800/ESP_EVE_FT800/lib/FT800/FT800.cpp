/***********************************************************************
*! \file:                   FT800.c
*  \projekt:                FT800_ESP
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/

/***********************************************************************
* Includes
***********************************************************************/
#include "FT800.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Declarations
***********************************************************************/
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
/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* local Variable
***********************************************************************/
FT800_HAL ft800HAL;
  	
/***********************************************************************
* Local Funtions
***********************************************************************/

/***********************************************************************
*! \fn			FT800::FT800(int8_t sck, int8_t miso, int8_t mosi, int8_t ss, int8_t pwrdn)
*  \brief		Cunstructor from SPI Transferclass
*  \param		int8_t sck - serial clock pin
*  \param		int8_t miso - master in slave out pin
*  \param		int8_t mosi - master out slave in pin
*  \param		int8_t ss - Slave select pin
*  \param		int8_t pwrdn - powerdown pin
*  \exception	none
*  \return		none
***********************************************************************/
FT800::FT800(int8_t sck, int8_t miso, int8_t mosi, int8_t ss, int8_t pwrdn)
 {
    /* SPI library specific */
    this->ft800pwrPin = pwrdn;
    ft800HAL.ft800init(sck, miso, mosi, ss);
    pinMode(ft800pwrPin, OUTPUT);			// FT800 Power Down (reset) input
    digitalWrite(ft800pwrPin, HIGH);		// Set PD# high to start
    delay(20);					// Wait a few MS before waking the FT800	
    
 }

/***********************************************************************
*! \fn			void FT800_Init(void)
*  \brief		Wake the Ft800 up
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void FT800::FT800_Init( ){
  
  	digitalWrite(ft800pwrPin, LOW);		// 1) lower PD#
  	delay(20);					// 2) hold for 20ms
  	digitalWrite(ft800pwrPin, HIGH);		// 3) raise PD#
  	delay(20);					// 4) wait for another 20ms before sending any commands

  	ft800HAL.ft800cmdWrite(FT800_ACTIVE);			// Start FT800
  	delay(5);					// Give some time to process
  	ft800HAL.ft800cmdWrite(FT800_CLKEXT);			// Set FT800 for external clock
  	delay(5);					// Give some time to process
  	ft800HAL.ft800cmdWrite(FT800_CLK48M);			// Set FT800 for 48MHz PLL
  	delay(5);					// Give some time to process
  						// Now FT800 can accept commands at up to 30MHz clock on SPI bus
  						//   This application leaves the SPI bus at 4MHz
	Serial.write("Check FT800!\r\n");		// Send an error message on the UART
  	while (ft800HAL.ft800memRead8(REG_ID) != 0x7C)		// Read ID register - is it 0x7C?
  	{
    	Serial.write("System Halted!\r\n");		// Send an error message on the UART
    	//while(1);					// If we don't get 0x7C, the ineface isn't working - halt with infinite loop
  	}  
  
  	ft800HAL.ft800memWrite8(REG_PCLK, ZERO);		// Set PCLK to zero - don't clock the LCD until later
  	ft800HAL.ft800memWrite8(REG_PWM_DUTY, ZERO);		// Turn off backlight
  
	// End of Wake-up FT800
	//***************************************

   //***************************************
	// Initialize Display
  	ft800HAL.ft800memWrite16(REG_HSIZE,   LCDWIDTH);	// active display width
  	ft800HAL.ft800memWrite16(REG_HCYCLE,  LCDHCYCLE);	// total number of clocks per line, incl front/back porch
  	ft800HAL.ft800memWrite16(REG_HOFFSET, LCDHOFFSET);	// start of active line
  	ft800HAL.ft800memWrite16(REG_HSYNC0,  LCDHSYNC0);	// start of horizontal sync pulse
  	ft800HAL.ft800memWrite16(REG_HSYNC1,  LCDHSYNC1);	// end of horizontal sync pulse
  	ft800HAL.ft800memWrite16(REG_VSIZE,   LCDHEIGHT);	// active display height
  	ft800HAL.ft800memWrite16(REG_VCYCLE,  LCDVCYCLE);	// total number of lines per screen, incl pre/post
  	ft800HAL.ft800memWrite16(REG_VOFFSET, LCDVOFFSET);	// start of active screen
  	ft800HAL.ft800memWrite16(REG_VSYNC0,  LCDVSYNC0);	// start of vertical sync pulse
  	ft800HAL.ft800memWrite16(REG_VSYNC1,  LCDVSYNC1);	// end of vertical sync pulse
  	ft800HAL.ft800memWrite8(REG_SWIZZLE,  LCDSWIZZLE);	// FT800 output to LCD - pin order
  	ft800HAL.ft800memWrite8(REG_PCLK_POL, LCDPCLKPOL);	// LCD data is clocked in on this PCLK edge
						// Don't set PCLK yet - wait for just after the first display list
	// End of Initialize Display

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
unsigned int FT800::incCMDOffset(unsigned int currentOffset, unsigned char commandSize)
{
    unsigned int newOffset;			// used to hold new offset
    newOffset = currentOffset + commandSize;	// Calculate new offset
    if(newOffset > 4095)			// If new offset past boundary...
    {
        newOffset = (newOffset - 4096);		// ... roll over pointer
    }
    return newOffset;				// Return new offset
}


void FT800::FT800_setup(){

   //***************************************
	// Configure Touch and Audio - not used in this example, so disable both
  	ft800HAL.ft800memWrite8(REG_TOUCH_MODE, ZERO);		// Disable touch
  	ft800HAL.ft800memWrite16(REG_TOUCH_RZTHRESH, ZERO);	// Eliminate any false touches
  
  	ft800HAL.ft800memWrite8(REG_VOL_PB, ZERO);		// turn recorded audio volume down
 	ft800HAL.ft800memWrite8(REG_VOL_SOUND, ZERO);		// turn synthesizer volume down
  	ft800HAL.ft800memWrite16(REG_SOUND, 0x6000);		// set synthesizer to mute
  
	// End of Configure Touch and Audio
	//***************************************

	//***************************************
	// Write Initial Display List & Enable Display

  	ramDisplayList = RAM_DL;			// start of Display List
  	ft800HAL.ft800memWrite32(ramDisplayList, DL_CLEAR_RGB); // Clear Color RGB   00000010 RRRRRRRR GGGGGGGG BBBBBBBB  (R/G/B = Colour values) default zero / black
  	ramDisplayList += 4;				// point to next location
  	ft800HAL.ft800memWrite32(ramDisplayList, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));	// Clear 00100110 -------- -------- -----CST  (C/S/T define which parameters to clear)
  	ramDisplayList += 4;				// point to next location
  	ft800HAL.ft800memWrite32(ramDisplayList, DL_DISPLAY);	// DISPLAY command 00000000 00000000 00000000 00000000 (end of display list)

  	ft800HAL.ft800memWrite32(REG_DLSWAP, DLSWAP_FRAME);	// 00000000 00000000 00000000 000000SS  (SS bits define when render occurs)
						// Nothing is being displayed yet... the pixel clock is still 0x00
  	ramDisplayList = RAM_DL;			// Reset Display List pointer for next list
  
  	ft800Gpio = ft800HAL.ft800memRead8(REG_GPIO);		// Read the FT800 GPIO register for a read/modify/write operation
  	ft800Gpio = ft800Gpio | 0x80;			// set bit 7 of FT800 GPIO register (DISP) - others are inputs
  	ft800HAL.ft800memWrite8(REG_GPIO, ft800Gpio);		// Enable the DISP signal to the LCD panel
  	ft800HAL.ft800memWrite8(REG_PCLK, LCDPCLK);		// Now start clocking data to the LCD panel
  	for(int duty = 128; duty > 0; duty--)
  	{
    	ft800HAL.ft800memWrite8(REG_PWM_DUTY, duty);		// Turn on backlight - ramp up slowly to full brighness
    	delay(10);
  	}

}


void FT800::FT800_test(){

   // Wait for graphics processor to complete executing the current command list
  	// This happens when REG_CMD_READ matches REG_CMD_WRITE, indicating that all commands
  	// have been executed.  The next commands get executed when REG_CMD_WRITE is updated again...
  	// then REG_CMD_READ again catches up to REG_CMD_WRITE
  	// This is a 4Kbyte ring buffer, so keep pointers within the 4K roll-over
 	 do
  	{
    	cmdBufferRd = ft800HAL.ft800memRead16(REG_CMD_READ);	// Read the graphics processor read pointer
    	cmdBufferWr = ft800HAL.ft800memRead16(REG_CMD_WRITE); // Read the graphics processor write pointer
  	}while (cmdBufferWr != cmdBufferRd);		// Wait until the two registers match
  
  	cmdOffset = cmdBufferWr;			// The new starting point the first location after the last command

  	if (color != WHITE)				// If a red dot was just drawn (or first time through)...
    	color = WHITE;				// change color to white
  	else						// Otherwise...
    	color = RED;				// change the color to red
    
  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (CMD_DLSTART));
						// Start the display list
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer
  

  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR_RGB | BLACK));
						// Set the default clear color to black
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
						// Clear the screen - this and the previous prevent artifacts between lists
						// Attributes are the color, stencil and tag buffers
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_COLOR_RGB | color));
						// Set the color of the following item(s) - toggle red/white from above
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_POINT_SIZE | point_size));
						// Select the size of the dot to draw
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_BEGIN | FTPOINTS));
						// Indicate to draw a point (dot)
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_VERTEX2F | (point_x << 15) | point_y));
						// Set the point center location
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_END));
						// End the point
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_DISPLAY));
						// Instruct the graphics processor to show the list
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (CMD_SWAP));
						// Make this list active
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite16(REG_CMD_WRITE, (cmdOffset));	// Update the ring buffer pointer so the graphics processor starts executing

}
 
FT800::~FT800()
 {
 }