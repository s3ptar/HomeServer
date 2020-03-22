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
#include "FT800_IMP.h"				// FT800 register, memory and command values
#include <PubSubClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include "datastore.h"
#include <WiFiClient.h>
#include <esp_wifi.h>
#include <FS.h>
#include <SPIFFS.h>
#include "ArduinoJson.h"
#include "datastore.h"

/***********************************************************************
*                 Global Variablen
***********************************************************************/


SSD1306Wire *display;
FT800_IMP eve_display(18,19,23,13,12,11);
WiFiClient ethClient;
PubSubClient client(ethClient);


const char* mqttServer = "192.168.0.222";
const int mqttPort = 1883;

char str_display[32];

/* 63 Char max and 17 missign for the mac */
TaskHandle_t Task1;
TaskHandle_t MQTTTaskHandle;

/***********************************************************************
*! \fn          void callback(char* topic, byte* payload, unsigned int length) 
*  \brief       callback for MQTT Client
*  \param       har* topic, byte* payload, unsigned int length
*  \exception   none
*  \return      none
***********************************************************************/
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

/***********************************************************************
*! \fn          void MQTT_Task( void* prarm )
*  \brief       MQTT Task
*  \param       void* prarm 
*  \exception   none
*  \return      none
***********************************************************************/
void MQTT_Task( void* prarm ){
    //const size_t capacity = JSON_OBJECT_SIZE(4);
    //DynamicJsonBuffer jsonBuffer(capacity);
    //https://arduinojson.org/v6/doc/upgrade/
    DynamicJsonDocument jsonBuffer(1024);
    String JsonString = "";
    uint32_t ulNotificationValue;
    //int32_t last_message = millis();
                         
    Serial.println("MQTT Thread Start");
    WiFiClient espClient;                       // WiFi ESP Client  
    PubSubClient client(espClient);             // MQTT Client 
    client.setCallback(callback);               // define Callback function
    while(1==1){

    /* if settings have changed we need to inform this task that a reload and reconnect is requiered */ 
    ulNotificationValue = ulTaskNotifyTake( pdTRUE, 0 );

    if( (ulNotificationValue & 0x01) != 0 ){
        Serial.println("Reload MQTT Settings");
        /* we need to reload the settings and do a reconnect */
        if(true == client.connected() ){
            client.disconnect();
        }
        //Settings = eepread_mqttsettings();
    }

    if(!client.connected()) {             
        /* sainity check */
        if( (struct_mqtt.mqtt_port!=0) && ((struct_mqtt.mqtt_prim_server_ipV4[0])!=0) ){
      
            Serial.print("Connecting to MQTT...");  // connect to MQTT
            client.setServer(struct_mqtt.mqtt_prim_server_ipV4, struct_mqtt.mqtt_port); // Init MQTT     
            if (client.connect(struct_mqtt.mqtthostname)) {
                Serial.println("connected");          // successfull connected  
                client.subscribe(struct_mqtt.mqtttopic_TimeAndDate);             // subscibe MQTT Topic
            } else {
                Serial.print("failed with state ");   // MQTT not connected       
            }
        }
    } else{
        client.loop();                            // loop on client
    }
     delay(10);
    }
}



/***********************************************************************
*! \fn          void reconnect()
*  \brief       reconnect for MQTT Client
*  \param       har* topic, byte* payload, unsigned int length
*  \exception   none
*  \return      none
***********************************************************************/
void reconnect() {
    // Loop until we're reconnected, Check if WLan enable
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("arduinoClient")) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic","hello world");
            // ... and resubscribe
            client.subscribe("inTopic");
        }else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
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
    WiFi.mode(WIFI_STA);
    WiFi.begin(struct_wlan.ssid, struct_wlan.password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        struct_wlan.wifi_is_connected = true;
  	}

    struct_wlan.wifi_is_connected = WiFi.isConnected();
    if (struct_wlan.wifi_is_connected) {
        Serial.println("Wifi connected");
    }
    else{
        Serial.println("Wifi NOT connected");
    }

    /************************** Ping ****************************************/

	struct_ft800.ft800_ready = eve_display.FT800_Init();
    if (struct_ft800.ft800_ready)
	    eve_display.FT800_setup();

	  //eve_display.CalibrateTouchPanel();
    if (struct_ft800.ft800_ready)
	      eve_display.Cmd_Logo();
	ArduinoOTA.setHostname("ESP_EVE");
	client.setServer(mqttServer, mqttPort);
	client.setCallback(callback);


    /************************** Create Tasks ********************************/
    xTaskCreatePinnedToCore(
        MQTT_Task,
        "MQTT_Task",
        10000,
        NULL,
        1,
        &MQTTTaskHandle,
        1
    );

	while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
 
        if (client.connect("ESP32Client")) {
            Serial.println("connected");  
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }  
    client.subscribe("System/DateAndTime");
 
    //client.publish("esp/test", "Hello from ESP8266");


    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname("myesp32");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
    ArduinoOTA.setHostname("EVEESP");
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