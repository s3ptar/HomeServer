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

#if !defined(PSOC_EVE_TOUCH_H)
#define PSOC_EVE_TOUCH_H
    
#include <cytypes.h>    
    
/*******************************************************************************
*   Definitions 
*******************************************************************************/    

#define TOUCHMODE_OFF         0
#define TOUCHMODE_ONESHOT     1  
#define TOUCHMODE_FRAME       2
#define TOUCHMODE_CONTINUOUS  3      
    
/*******************************************************************************
*   Typedefs.
*******************************************************************************/

/* Typedef name : TouchCalibrationValues.
   Description: It is a union of uint8 array and uint32 array.
                uint32 array stores values about calibration of touch panel.
                uint8 array gives access to those values in bytes form to ease
                storing in flash rom.
*/
typedef union
{
    uint8 TouchTransform_Bytes[24] ;
    uint32 TouchTransform_X[6];
} TouchCalibrationValues;    
 
/*******************************************************************************
*   Function prototypes.
*******************************************************************************/

void FT_Touch_Enable();
void FT_Touch_Disable();
uint8 FT_Touch_Calibrate();
void FT_Touch_WaitCalibrationEnds();
void FT_Touch_ReadCalibrationValues(TouchCalibrationValues* values);
void FT_Touch_WriteCalibrationValues(TouchCalibrationValues* values);
    
#endif /* End PSOC_EVE_TOUCH_H */  

/* [] END OF FILE */
