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
#include "FT800_HAL.h"

const char* ssid = "chilihotdog";
const char* password = "bxJHckMMkGqEPfY3Jf3nZnAn5FtGYwKZSkzVvbzFHNbpUZfv79GXm8afDuNu";
char str_display[32];
SSD1306Wire *display;
FT800 eve_display(18,19,23,13,12);

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
	Heltec.begin(true, false, true);
	Serial.println("Booting...");
	
	Wire.begin(SDA_OLED, SCL_OLED); //Scan OLED's I2C address via I2C0
	//Wire1.begin(SDA, SCL);        //If there have other device on I2C1, scan the device address via I2C1
	WiFi.mode(WIFI_STA);
  	WiFi.begin(ssid, password);
  	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    	Serial.println("Connection Failed! Rebooting...");
    	delay(5000);
    	ESP.restart();
  	}
	eve_display.FT800_Init();
	eve_display.FT800_setup();
	

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

	eve_display.FT800_test();

  	delay(500);					// Wait a half-second to observe the changing color
}