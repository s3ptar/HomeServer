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
//#include <WiFi.h>
//#include <ESPmDNS.h>
//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>
#include <SPI.h>				// Arduino SPI Library definitions
#include "FT800_IMP.h"				// FT800 register, memory and command values
#include <PubSubClient.h>
//#include <Ethernet.h>
//#include <Dns.h>
#include "datastore.h"
//#include <WiFiClient.h>
//#include <esp_wifi.h>
#include <FS.h>
#include <SPIFFS.h>
#include "ArduinoJson.h"
#include "datastore.h"
#include "connectivity.h"

/***********************************************************************
*                 Global Variablen
***********************************************************************/

SSD1306Wire *display;
FT800_IMP eve_display(18,19,23,13,12,11);

//const char* mqttServer = "192.168.0.222";
//const int mqttPort = 1883;

char str_display[32];

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
  
/***********************************************************************
*! \fn          void setup()
*  \brief       Arduino Setup - Routine
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void setup(){


    //Init Heltec Libary
    Heltec.begin(true, false, true);
	  Serial.println("Booting...");
    //init Onboard OLED
    Wire.begin(SDA_OLED, SCL_OLED); //Scan OLED's I2C address via I2C0
	  //Wire1.begin(SDA, SCL);        //If there have other device on I2C1, scan the device address via I2C1
    SPIFFS.begin();

    /************************** WLAN ****************************************/
    run_connectivity();

    /************************** Ping ****************************************/

	  struct_ft800.ft800_ready = eve_display.FT800_Init();
    if (struct_ft800.ft800_ready)
	      eve_display.FT800_setup();

	     //eve_display.CalibrateTouchPanel();
    if (struct_ft800.ft800_ready)
	      eve_display.Cmd_Logo();
	  

  	Serial.println("Ready");
  	Serial.print("IP address: ");
  	Serial.println(WiFi.localIP());
}

/***********************************************************************
*! \fn          void loop(){
*  \brief       construct host command and send to graphics controller
*  \param       uint32_t HostCommand
*  \exception   none
*  \return      none
***********************************************************************/
void loop(){

    //check WiFi Status

    //Serial.println("Next Round");
	  ArduinoOTA.handle();
    Heltec.display->clear();
	  Heltec.display->setFont(ArialMT_Plain_10);
	  Heltec.display->drawString(0, 0, WiFi.localIP().toString());
  	Heltec.display->display();


	  //eve_display.FT800_test();

  	//delay(5000);					// Wait a half-second to observe the changing color
}