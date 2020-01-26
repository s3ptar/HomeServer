/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/***********************************************************************
*! \file:                   EVE_Config.h
*  \projekt:                PSOC_EVE_Sensorboard
*  \created on:             2020-02-12
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   Configuration
***********************************************************************/
#ifndef __EVE_Config_H
#define __EVE_Config_H_
/***********************************************************************
 * Includes
 **********************************************************************/
#include "stdbool.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array

/***********************************************************************
 * Global Variable
 **********************************************************************/

/***********************************************************************
 * Constant
 **********************************************************************/

#define ERROR                           1
#define OK                              0

#define U8                              unsigned char
#define U16                             unsigned int
#define U32                             unsigned long int
#define I32                             long int    
    
    
typedef bool eve_enable_t;

//Controller Typ
#define _FT800_
#define ET0430
//#define ET0350
    
#ifdef ET0350
#define DispWidth        320
#define DispHeight       240
#endif

#ifdef ET0430
#define DispWidth        480
#define DispHeight       272
#endif

/***********************************************************************
 * Functions
 **********************************************************************/



#endif /* _EVE_Config_H_ */

/* [] END OF FILE */
