/***********************************************************************
*! \file:                   datastrore.c
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
#include "datastore.h"

/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Declarations
***********************************************************************/
 
/***********************************************************************
* Global Variable
***********************************************************************/


struct ft800_struct_tag struct_ft800 = {
    .ft800_ready = false
};

struct struct_wlan_typ struct_wlan = {

    .wifi_is_connected = false,
    .wait_for_wifi_connect_1000ms = 1,
    .ssid = "chilihotdog",
    .password = "bxJHckMMkGqEPfY3Jf3nZnAn5FtGYwKZSkzVvbzFHNbpUZfv79GXm8afDuNu"

};


struct mqtt_struct_tag struct_mqtt = {

    .mqtt_prim_server_ipV4  = "192.168.0.222",
    .mqtt_sec_server_ipV4  = "192.168.0.222",
    .mqtthostname = "ESP32",
    .mqtt_port = 1883,
    .mqttsubtopics = {"ServerStatus"},
    .mqttpubtopics = {"ClientStatus"},

};

String CurrentServerTimeStamp;
String CurrentIPAdress;


/***********************************************************************
* Constant
***********************************************************************/
 
/***********************************************************************
* Local Funtions
***********************************************************************/
 
/***********************************************************************
*! \fn          int16_t create_fb(char *dataPtr, byte *fb)
*  \brief       reset the Framebuffer
*  \param       dataPtr String to scroll across
*  \param       fb Pointer to the frame buffer array
*  \exception   none
*  \return      length of frame buffer
***********************************************************************/
