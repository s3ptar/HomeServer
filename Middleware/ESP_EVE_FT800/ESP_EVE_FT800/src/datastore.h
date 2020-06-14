/***********************************************************************
*! \file:                   datastoreL.h
*  \projekt:                globale Variablen
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/
 
#ifndef _datastore_H_
#define _datastore_H_
 
/***********************************************************************
* Includes
**********************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "Arduino.h"
/***********************************************************************
* Informations
**********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
* Declarations
**********************************************************************/

#define Max_MQTT_Subscription_Topics                  1
#define Max_MQTT_Publish_Topics                       1
#define MQTT_PubTopic_ClientStatus                    0

#define MAX_MQTT_Message_Buffer                       1024
 
/***********************************************************************
* Global Variable
**********************************************************************/

struct mqtt_struct_tag {

    char* mqtt_prim_server_ipV4;
    char* mqtt_sec_server_ipV4;
    int mqtt_port;
    char* project_name;
    char* mqtt_server_status_topic;
    char* mqtt_device_root_topic;
    char* mqtt_device_information_topic;
    char* mqtt_device_status_topic;
    char* mqtt_device_value_topic;
    char* mqtt_device_ctrl_topic;


};
extern struct mqtt_struct_tag struct_mqtt;

struct ft800_struct_tag {

    bool ft800_ready;
   
};
extern struct ft800_struct_tag struct_ft800;

struct struct_wlan_typ {
    bool wifi_is_connected;
    uint8_t wait_for_wifi_connect_1000ms;
    const char* ssid;
    const char* password;
    const char* dns;
};
extern struct struct_wlan_typ struct_wlan;

extern String CurrentServerTimeStamp;
extern String CurrentIPAdress;

extern char* MQTT_Subscripes[];
/***********************************************************************
 * Constant
 **********************************************************************/
 
/***********************************************************************
 * Macros
 **********************************************************************/
 
/***********************************************************************
 * Funtions
 **********************************************************************/
#endif /* _datastore_H_ */
