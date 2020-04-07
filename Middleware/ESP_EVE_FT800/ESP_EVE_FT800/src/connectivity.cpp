/***********************************************************************
*! \file:                   connectivity.c
*  \projekt:                EVE_ESP
*  \created on:             06.03.2020
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/
 
/***********************************************************************
* Includes
***********************************************************************/
#include "connectivity.h"

/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
//https://arduinojson.org/v5/example/parser/
/***********************************************************************
* Declarations
***********************************************************************/
enum connectivity_states{
    connectivity_setup = 0,
    connectivity_connected,
    connectivity_broken
};
/***********************************************************************
* Global Variable
***********************************************************************/
 
/***********************************************************************
* Local Variable
***********************************************************************/
WiFiClient ethClient;
PubSubClient client(ethClient);
uint8_t connectivity_state = connectivity_setup;
char str_display_con[32];
/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Local Funtions
***********************************************************************/

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
    DynamicJsonDocument jsonBuffer(1024);
    deserializeJson(jsonBuffer, payload, length);

    //if (topic == (const char*)"ServerStatus"){
        const char* ServerZeit = jsonBuffer["ServerTime"];
        Serial.print(ServerZeit);
        CurrentServerTimeStamp = ServerZeit;
        Serial.println();
    //}
    //Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.println();
    //Serial.print("] ");
    //for (int i=0;i<length;i++) {
    //    Serial.print((char)payload[i]);
    //}
    //parse in string
    
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
    //DynamicJsonDocument jsonBuffer(1024);
    //String JsonString = "";
    uint32_t ulNotificationValue;
    //int32_t last_message = millis();
    uint8_t scubsriptions = 0;                     
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
                //subscripe to Topics
				//sizeof(struct_mqtt.mqtttopics);
				for(scubsriptions=0; scubsriptions < Max_MQTT_Topics  ; scubsriptions++){
					
					//subscrip to all nessessary 
					client.subscribe(struct_mqtt.mqtttopics[scubsriptions]);
					
				}
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
*! \fn          void run_connectivity()
*  \brief       run Connectivity
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void run_connectivity(){

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
    
    ArduinoOTA.setHostname("ESP_EVE");
     /************************** MQTT Client *********************************/
	  client.setServer( struct_mqtt.mqtt_prim_server_ipV4, struct_mqtt.mqtt_port);
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
    client.subscribe("ServerStatus");
 
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
		    sprintf(&str_display_con[0], "Progress: %u%%\r", (progress / (total / 100)));
    	    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            //Refresh upload
		    Heltec.display->clear();
		    Heltec.display->setFont(Normal_Font);
		    Heltec.display->drawString(ip_start_x_start, ip_start_y_start, &str_display_con[0]);
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
}
 

/***********************************************************************
*! \fn          void reconnect_mqtt()
*  \brief       reconnect for MQTT Client
*  \param       har* topic, byte* payload, unsigned int length
*  \exception   none
*  \return      none
***********************************************************************/
void reconnect_mqtt() {
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
/***********************************************************************
*! \fn          void setup_wlan(){
*  \brief       setup Wlan Connection
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void setup_wlan(){
  
    
}
