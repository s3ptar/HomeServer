/***********************************************************************
*! \file:                   connectivityL.h
*  \projekt:                globale Variablen
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/
 
#ifndef _connectivity_H_
#define _connectivity_H_
 
/***********************************************************************
* Includes
**********************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "datastore.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <esp_wifi.h>
#include "ArduinoJson.h"
#include <heltec.h>
#include "Config.h"
#include "datastore.h"
/***********************************************************************
* Informations
**********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
* Declarations
**********************************************************************/
 enum check_Wlan_return_codex {

     no_error = 0,
     wlan_disconnected

 };
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
void run_connectivity();
uint8_t check_Wlan();
uint8_t publish_Status_information(uint8_t topic_number);
char* build_client_status(char* StatusMSG);

#endif /* _connectivity_H_ */
