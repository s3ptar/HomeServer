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
*! \file:                   timing.c
*  \projekt:                PSOC_EVE_Sensorboard
*  \created on:             2020-02-12
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   timing and delay functions
***********************************************************************/

/***********************************************************************
* Includes
***********************************************************************/
#include "timing.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Local Funtions
***********************************************************************/
void SysTickISRCallback(void);
/***********************************************************************
* Global Variable
***********************************************************************/
uint32_t msCount;
uint32_t secCount;
uint32_t minCount;
volatile uint32_t tickStatus;
/***********************************************************************
* Constant
***********************************************************************/
#define TIME_MIN_IN_HR      (60u)
#define TIME_SEC_IN_MIN     (60u)
#define TIME_MS_IN_SEC      (1000u)




/***********************************************************************
*! \fn			void SysTickISRCallback(void)
*  \brief		This API is called from SysTick timer interrupt handler to update the
*               millisecond counter.
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void SysTickISRCallback(void){
    tickStatus = 1u;

    /* Counts the number of milliseconds in one second */
    if(msCount != 0u)
    {
        --msCount;
    }
    else
    {
        /* Counts from 999 down to 0 */
        //msCount = TIME_MS_IN_SEC - 1u;
    }
}

/***********************************************************************
*! \fn			void init_Delay(void)
*  \brief		init the systickcounter
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void init_Delay(void){

    CySysTickStart();
    uint32 i;

    /* Find unused callback slot and assign the callback. */
    for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; ++i)
    {
        if (CySysTickGetCallback(i) == NULL)
        {
            /* Set callback */
            CySysTickSetCallback(i, SysTickISRCallback);
            break;
        }
    }
}

/***********************************************************************
*! \fn			void delay_ms(uint32_t ms_to_delay)
*  \brief		delay ms
*  \param		uint32_t dms_to_delay - ticks to delay
*  \exception	none
*  \return		none
***********************************************************************/
void delay_ms(uint32_t ms_to_delay){
    
    msCount = ms_to_delay;
    
    while(msCount);
    
    
}
/* [] END OF FILE */


