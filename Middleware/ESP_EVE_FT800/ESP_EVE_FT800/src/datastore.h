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

struct mqtt_struct_tag {

    char* mqtt_prim_server_ipV4;
    char* mqtt_sec_server_ipV4;
    char* mqtthostname;
    int mqtt_port;
    char* mqtttopic_TimeAndDate;
   
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
};
extern struct struct_wlan_typ struct_wlan;
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
