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
*! \file:                  TIMING.h
*  \projekt:                PSOC_EVE_Sensorboard
*  \created on:             2020-02-12
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   timing and delay function
***********************************************************************/
#ifndef _TIMING_H
#define _TIMING_H_
/***********************************************************************
 * Includes
 **********************************************************************/
#include "stdbool.h"
#include <stdio.h>
#include "project.h"
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


/***********************************************************************
 * Functions
 **********************************************************************/
void init_Delay(void);
void delay_ms(uint32_t ms_to_delay);


#endif /* _TIMING_H_ */

/* [] END OF FILE */
