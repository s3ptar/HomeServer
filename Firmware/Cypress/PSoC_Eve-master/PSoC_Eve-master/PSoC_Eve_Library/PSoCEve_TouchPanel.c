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

#include "PSoCEve.h"  
#include "PSoCEve_TouchPanel.h"

/*******************************************************************************
* Function Name: FT_Touch_Enable
********************************************************************************
*
* Summary:
*  Enable the touch panel.
*  Value of REG_TOUCH_RZTHRESH depends on touch panel requirement and is derived 
*  by experimentation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

void FT_Touch_Enable()
{
    //FT_Register_Write(REG_TOUCH_MODE, TOUCHMODE_CONTINUOUS);
    FT_Register_Write(REG_TOUCH_MODE, TOUCHMODE_FRAME);
    FT_Register_Write(REG_TOUCH_RZTHRESH, 1200);    
}

/*******************************************************************************
* Function Name: FT_Touch_Disable
********************************************************************************
*
* Summary:
*  Disable the touch panel. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

void FT_Touch_Disable()
{
    FT_Register_Write(REG_TOUCH_MODE, 0);
    FT_Register_Write(REG_TOUCH_RZTHRESH, 0);    
}

/*******************************************************************************
* Function Name: FT_Touch_Calibrate
********************************************************************************
*
* Summary:
*  Start touch calibration procedure.
*  FT chip doesn´t permanently store calibration values internally, so they are
*  lost every time the FT chip is power cycle or reseted.
*  Look at FT_Touch_ReadCalibrationValues and FT_Touch_WriteCalibrationValues.
*
* Parameters:
*  None
*
* Return:
*  0, if can not start calibration because there is a list (display or 
*     coprocessor in progress).
*  1, for successful calibration. 
*
*******************************************************************************/

uint8 FT_Touch_Calibrate()
{
    //if (listInProgress != NONE) return 0;
    
    FT_ListStart(DLIST);
    CMDCalibrate();
    FT_ListEnd(END_DL_NOSWAP);  
    
    while (!FTIsCoproccesorReady()) {};
    
    return 1;
}

void FT_Touch_WaitCalibrationEnds()
{
    while (!FTIsCoproccesorReady()) {};
}

/*******************************************************************************
* Function Name: FT_Touch_ReadCalibrationValues
********************************************************************************
*
* Summary:
*  This function reads touch calibration values from registers in FT chip.
*  Then you can store those values in flash rom so they can be used in
*  future.
*  Until now, this library doesn´t store values in flash, you have to implement
*  it by yourself.
*
* Parameters:
*  values: pointer to variable where to store calibration values.
*          Look at definition of "TouchCalibrationValues" enum in PSocEve.h
*
* Return:
*  none
*
*******************************************************************************/

void FT_Touch_ReadCalibrationValues(TouchCalibrationValues* values)
{
    uint8 loop;
    uint32 ptr = REG_TOUCH_TRANSFORM_A;
        
    for (loop = 0; loop < 6; loop++)
    {
        values->TouchTransform_X[loop] = FT_Read_UINT32(ptr | MEMORY_READ);
        ptr += 4;
    }
}

/*******************************************************************************
* Function Name: FT_Touch_WriteCalibrationValues
********************************************************************************
*
* Summary:
*  This function writes touch calibration values to registers in FT chip.
*  Can be used to write previouly stored values.
*  Until now, this library doesn´t store value in flash, you have to implement
*  it by yourself.
*
* Parameters:
*  values: pointer to variable where to calibration values are stored.
*          Look at definition of "TouchCalibrationValues" enum in PSocEve.h
*
* Return:
*  none
*
*******************************************************************************/

void FT_Touch_WriteCalibrationValues(TouchCalibrationValues* values)
{
    uint8 loop;
    uint32 ptr = REG_TOUCH_TRANSFORM_A;
    
    for (loop = 0; loop < 6; loop++)
    {
        FT_Register_Write(ptr, values->TouchTransform_X[loop]);
        ptr += 4;
    }
}


/* [] END OF FILE */
