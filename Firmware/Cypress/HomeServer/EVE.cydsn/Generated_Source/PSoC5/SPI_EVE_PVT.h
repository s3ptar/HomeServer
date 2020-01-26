/*******************************************************************************
* File Name: .h
* Version 2.50
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_SPI_EVE_H)
#define CY_SPIM_PVT_SPI_EVE_H

#include "SPI_EVE.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 SPI_EVE_swStatusTx;
extern volatile uint8 SPI_EVE_swStatusRx;

#if(SPI_EVE_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPI_EVE_txBuffer[SPI_EVE_TX_BUFFER_SIZE];
    extern volatile uint8 SPI_EVE_txBufferRead;
    extern volatile uint8 SPI_EVE_txBufferWrite;
    extern volatile uint8 SPI_EVE_txBufferFull;
#endif /* (SPI_EVE_TX_SOFTWARE_BUF_ENABLED) */

#if(SPI_EVE_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 SPI_EVE_rxBuffer[SPI_EVE_RX_BUFFER_SIZE];
    extern volatile uint8 SPI_EVE_rxBufferRead;
    extern volatile uint8 SPI_EVE_rxBufferWrite;
    extern volatile uint8 SPI_EVE_rxBufferFull;
#endif /* (SPI_EVE_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_SPI_EVE_H */


/* [] END OF FILE */
