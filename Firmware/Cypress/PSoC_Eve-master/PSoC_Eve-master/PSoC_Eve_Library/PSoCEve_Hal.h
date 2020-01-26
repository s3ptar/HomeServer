/*******************************************************************************
* File Name: PSoCEve_Hal.h  
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

#if !defined(PSOC_EVE_HAL_H)
#define PSOC_EVE_HAL_H
    
#include <cytypes.h>
#include "PSoCEve_Config.h"    
    


    
void FTCommandWrite(uint8 command);   
void FT_Write_Byte(uint32 address, uint8 data);
uint8 FT_Read_Byte(uint32 address);
void FT_Write_UINT32(uint32 address, uint32 data);    
uint32 FT_Read_UINT32(uint32 address);
void FT_Send_ByteArray_S(const uint8 *data, uint32 size);
    
void FT_Transfer_Start(uint32 address);
void FT_Transfer_End();
void FT_Send_Byte(uint8 data);
void FT_Send_ByteArray(const uint8 *data, uint32 size);
void FT_Send_UINT32(uint32 data);

//uint32 FTMemoryReadUint32(uint32 address);

    
#endif /* End PSOC_EVE_HAL_H */    

/* [] END OF FILE */
