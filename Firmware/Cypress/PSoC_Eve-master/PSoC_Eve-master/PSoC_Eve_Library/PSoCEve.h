/*******************************************************************************
* File Name: PSoCEve.h  
* Version 0.1 Alpha
*
* Description:
*  PSoCEveLibrary project.
*  Library to easy the use of the FT EVE graphic chips from FTDI.
*  Actually in development and in a very early stage there is support only
*  for the FT800 chip with Cypress PSOC4 family microcontrollers.
*  It is planned to had support for FT801 and newer TF81x chips, also for
*  other PSOC families (PSOC3 and PSCO5).
*
* Note:
*
********************************************************************************
* Copyright (c) 2015 Jesús Rodríguez Cacabelos
* This library is dual licensed under the MIT and GPL licenses.
* http:
*******************************************************************************/

#if !defined(PSOCEVE_LIBRARY_H)
#define PSOCEVE_LIBRARY_H
    
#define PSOCEVE_LIBRARY_VERSION     "0.1.0" 
    
#include <cytypes.h> 
#include "PSoCEve_Config.h"
#include "PSoCEve_Hal.h"
#include "PSoCEve_List.h"  
    
#ifdef USE_TOUCHPANEL
    #include "PSoCEve_TouchPanel.h"
#endif    
    
#ifdef USE_AUDIO
    #include "PSoCEve_Audio.h"       
#endif    

/*******************************************************************************
*   Function prototypes.
*******************************************************************************/

void FT_Register_Write(uint32 everegister, uint32 value);
uint32 FT_Register_Read(uint32 everegister);

uint8 FT_Init();
void FT_Display_ON();
void FT_Display_OFF();

/* *** Related to display list. *** */


/* *** Other. *** */



    
#endif /* End PSOCEVE_LIBRARY */    

/* [] END OF FILE */
