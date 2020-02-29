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
 
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
 * Declarations
 **********************************************************************/
 
/***********************************************************************
 * Global Variable
 **********************************************************************/
 
/***********************************************************************
 * Constant
 **********************************************************************/
 
/***********************************************************************
 * Macros
 **********************************************************************/
 
/***********************************************************************
 * Funtions
 **********************************************************************/

class datastore
{
private:
    
public:
    
    //functions
    datastore(/* args */);
    ~datastore();
    //variablen
    uint8_t wait_for_wifi_connect_1000ms;
    bool wifi_is_connected;
    bool ft800_ready;
    char mqtt_prim_server_ipV4[15];
    char mqtt_sec_server_ipV4[15];
    char mqtthostname[15];
	int mqtt_port;
    char mqtttopic[127];

};

datastore::datastore(/* args */)
{
    wait_for_wifi_connect_1000ms = 10;
    wifi_is_connected = false;
    sprintf(mqtt_prim_server_ipV4, "%s", "192.168.0.222") ;
    sprintf(mqtt_prim_server_ipV4, "%s", "192.168.0.222") ;
    sprintf(mqtthostname, "%s", "ESP32");
    sprintf(mqtttopic, "%s", "System/DateAndTime");
    mqtt_port = 1883;
    
}

datastore::~datastore()
{
}

 

 
#endif /* _datastore_H_ */
