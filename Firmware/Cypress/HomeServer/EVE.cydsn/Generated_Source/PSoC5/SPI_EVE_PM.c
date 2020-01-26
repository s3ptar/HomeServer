/*******************************************************************************
* File Name: SPI_EVE_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_EVE_PVT.h"

static SPI_EVE_BACKUP_STRUCT SPI_EVE_backup =
{
    SPI_EVE_DISABLED,
    SPI_EVE_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_EVE_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_EVE_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_EVE_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_EVE_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_EVE_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_EVE_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_EVE_Sleep(void) 
{
    /* Save components enable state */
    SPI_EVE_backup.enableState = ((uint8) SPI_EVE_IS_ENABLED);

    SPI_EVE_Stop();
}


/*******************************************************************************
* Function Name: SPI_EVE_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_EVE_backup - used when non-retention registers are restored.
*  SPI_EVE_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_EVE_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_EVE_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_EVE_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_EVE_Wakeup(void) 
{
    #if(SPI_EVE_RX_SOFTWARE_BUF_ENABLED)
        SPI_EVE_rxBufferFull  = 0u;
        SPI_EVE_rxBufferRead  = 0u;
        SPI_EVE_rxBufferWrite = 0u;
    #endif /* (SPI_EVE_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_EVE_TX_SOFTWARE_BUF_ENABLED)
        SPI_EVE_txBufferFull  = 0u;
        SPI_EVE_txBufferRead  = 0u;
        SPI_EVE_txBufferWrite = 0u;
    #endif /* (SPI_EVE_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_EVE_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_EVE_backup.enableState)
    {
        SPI_EVE_Enable();
    }
}


/* [] END OF FILE */
