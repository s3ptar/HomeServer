/*******************************************************************************
* File Name: EVE_SPI.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the SPI Master component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EVE_SPI_PVT.h"

#if(EVE_SPI_TX_SOFTWARE_BUF_ENABLED)
    volatile uint8 EVE_SPI_txBuffer[EVE_SPI_TX_BUFFER_SIZE];
    volatile uint8 EVE_SPI_txBufferFull;
    volatile uint8 EVE_SPI_txBufferRead;
    volatile uint8 EVE_SPI_txBufferWrite;
#endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED) */

#if(EVE_SPI_RX_SOFTWARE_BUF_ENABLED)
    volatile uint8 EVE_SPI_rxBuffer[EVE_SPI_RX_BUFFER_SIZE];
    volatile uint8 EVE_SPI_rxBufferFull;
    volatile uint8 EVE_SPI_rxBufferRead;
    volatile uint8 EVE_SPI_rxBufferWrite;
#endif /* (EVE_SPI_RX_SOFTWARE_BUF_ENABLED) */

uint8 EVE_SPI_initVar = 0u;

volatile uint8 EVE_SPI_swStatusTx;
volatile uint8 EVE_SPI_swStatusRx;


/*******************************************************************************
* Function Name: EVE_SPI_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EVE_SPI_Init(void) 
{
    /* Initialize the Bit counter */
    EVE_SPI_COUNTER_PERIOD_REG = EVE_SPI_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != EVE_SPI_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (EVE_SPI_TX_ISR_NUMBER);
        CyIntSetPriority     (EVE_SPI_TX_ISR_NUMBER,  EVE_SPI_TX_ISR_PRIORITY);
        (void) CyIntSetVector(EVE_SPI_TX_ISR_NUMBER, &EVE_SPI_TX_ISR);
    #endif /* (0u != EVE_SPI_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != EVE_SPI_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (EVE_SPI_RX_ISR_NUMBER);
        CyIntSetPriority     (EVE_SPI_RX_ISR_NUMBER,  EVE_SPI_RX_ISR_PRIORITY);
        (void) CyIntSetVector(EVE_SPI_RX_ISR_NUMBER, &EVE_SPI_RX_ISR);
    #endif /* (0u != EVE_SPI_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    EVE_SPI_ClearFIFO();

    #if(EVE_SPI_RX_SOFTWARE_BUF_ENABLED)
        EVE_SPI_rxBufferFull  = 0u;
        EVE_SPI_rxBufferRead  = 0u;
        EVE_SPI_rxBufferWrite = 0u;
    #endif /* (EVE_SPI_RX_SOFTWARE_BUF_ENABLED) */

    #if(EVE_SPI_TX_SOFTWARE_BUF_ENABLED)
        EVE_SPI_txBufferFull  = 0u;
        EVE_SPI_txBufferRead  = 0u;
        EVE_SPI_txBufferWrite = 0u;
    #endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED) */

    (void) EVE_SPI_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) EVE_SPI_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    EVE_SPI_TX_STATUS_MASK_REG = EVE_SPI_TX_INIT_INTERRUPTS_MASK;
    EVE_SPI_RX_STATUS_MASK_REG = EVE_SPI_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: EVE_SPI_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void EVE_SPI_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    EVE_SPI_COUNTER_CONTROL_REG |= EVE_SPI_CNTR_ENABLE;
    EVE_SPI_TX_STATUS_ACTL_REG  |= EVE_SPI_INT_ENABLE;
    EVE_SPI_RX_STATUS_ACTL_REG  |= EVE_SPI_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != EVE_SPI_INTERNAL_CLOCK)
        EVE_SPI_IntClock_Enable();
    #endif /* (0u != EVE_SPI_INTERNAL_CLOCK) */

    EVE_SPI_EnableTxInt();
    EVE_SPI_EnableRxInt();
}


/*******************************************************************************
* Function Name: EVE_SPI_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EVE_SPI_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EVE_SPI_Start(void) 
{
    if(0u == EVE_SPI_initVar)
    {
        EVE_SPI_Init();
        EVE_SPI_initVar = 1u;
    }

    EVE_SPI_Enable();
}


/*******************************************************************************
* Function Name: EVE_SPI_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void EVE_SPI_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    EVE_SPI_TX_STATUS_ACTL_REG &= ((uint8) ~EVE_SPI_INT_ENABLE);
    EVE_SPI_RX_STATUS_ACTL_REG &= ((uint8) ~EVE_SPI_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != EVE_SPI_INTERNAL_CLOCK)
        EVE_SPI_IntClock_Disable();
    #endif /* (0u != EVE_SPI_INTERNAL_CLOCK) */

    EVE_SPI_DisableTxInt();
    EVE_SPI_DisableRxInt();
}


/*******************************************************************************
* Function Name: EVE_SPI_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void EVE_SPI_EnableTxInt(void) 
{
    #if(0u != EVE_SPI_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(EVE_SPI_TX_ISR_NUMBER);
    #endif /* (0u != EVE_SPI_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: EVE_SPI_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void EVE_SPI_EnableRxInt(void) 
{
    #if(0u != EVE_SPI_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(EVE_SPI_RX_ISR_NUMBER);
    #endif /* (0u != EVE_SPI_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: EVE_SPI_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void EVE_SPI_DisableTxInt(void) 
{
    #if(0u != EVE_SPI_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(EVE_SPI_TX_ISR_NUMBER);
    #endif /* (0u != EVE_SPI_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: EVE_SPI_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void EVE_SPI_DisableRxInt(void) 
{
    #if(0u != EVE_SPI_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(EVE_SPI_RX_ISR_NUMBER);
    #endif /* (0u != EVE_SPI_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: EVE_SPI_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void EVE_SPI_SetTxInterruptMode(uint8 intSrc) 
{
    EVE_SPI_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: EVE_SPI_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void EVE_SPI_SetRxInterruptMode(uint8 intSrc) 
{
    EVE_SPI_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: EVE_SPI_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  EVE_SPI_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 EVE_SPI_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(EVE_SPI_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        EVE_SPI_DisableTxInt();

        tmpStatus = EVE_SPI_GET_STATUS_TX(EVE_SPI_swStatusTx);
        EVE_SPI_swStatusTx = 0u;

        EVE_SPI_EnableTxInt();

    #else

        tmpStatus = EVE_SPI_TX_STATUS_REG;

    #endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: EVE_SPI_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  EVE_SPI_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 EVE_SPI_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(EVE_SPI_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        EVE_SPI_DisableRxInt();

        tmpStatus = EVE_SPI_GET_STATUS_RX(EVE_SPI_swStatusRx);
        EVE_SPI_swStatusRx = 0u;

        EVE_SPI_EnableRxInt();

    #else

        tmpStatus = EVE_SPI_RX_STATUS_REG;

    #endif /* (EVE_SPI_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: EVE_SPI_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  EVE_SPI_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  EVE_SPI_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  EVE_SPI_txBuffer[EVE_SPI_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EVE_SPI_WriteTxData(uint8 txData) 
{
    #if(EVE_SPI_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = EVE_SPI_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (EVE_SPI_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == EVE_SPI_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        EVE_SPI_DisableTxInt();

        tempStatus = EVE_SPI_GET_STATUS_TX(EVE_SPI_swStatusTx);
        EVE_SPI_swStatusTx = tempStatus;


        if((EVE_SPI_txBufferRead == EVE_SPI_txBufferWrite) &&
           (0u != (EVE_SPI_swStatusTx & EVE_SPI_STS_TX_FIFO_NOT_FULL)))
        {
            /* Put data element into the TX FIFO */
            CY_SET_REG8(EVE_SPI_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            EVE_SPI_txBufferWrite++;
            if(EVE_SPI_txBufferWrite >= EVE_SPI_TX_BUFFER_SIZE)
            {
                EVE_SPI_txBufferWrite = 0u;
            }

            if(EVE_SPI_txBufferWrite == EVE_SPI_txBufferRead)
            {
                EVE_SPI_txBufferRead++;
                if(EVE_SPI_txBufferRead >= EVE_SPI_TX_BUFFER_SIZE)
                {
                    EVE_SPI_txBufferRead = 0u;
                }
                EVE_SPI_txBufferFull = 1u;
            }

            EVE_SPI_txBuffer[EVE_SPI_txBufferWrite] = txData;

            EVE_SPI_TX_STATUS_MASK_REG |= EVE_SPI_STS_TX_FIFO_NOT_FULL;
        }

        EVE_SPI_EnableTxInt();

    #else
        /* Wait until TX FIFO has a place */
        while(0u == (EVE_SPI_TX_STATUS_REG & EVE_SPI_STS_TX_FIFO_NOT_FULL))
        {
        }

        /* Put data element into the TX FIFO */
        CY_SET_REG8(EVE_SPI_TXDATA_PTR, txData);

    #endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: EVE_SPI_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  EVE_SPI_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  EVE_SPI_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  EVE_SPI_rxBuffer[EVE_SPI_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 EVE_SPI_ReadRxData(void) 
{
    uint8 rxData;

    #if(EVE_SPI_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        EVE_SPI_DisableRxInt();

        if(EVE_SPI_rxBufferRead != EVE_SPI_rxBufferWrite)
        {
            if(0u == EVE_SPI_rxBufferFull)
            {
                EVE_SPI_rxBufferRead++;
                if(EVE_SPI_rxBufferRead >= EVE_SPI_RX_BUFFER_SIZE)
                {
                    EVE_SPI_rxBufferRead = 0u;
                }
            }
            else
            {
                EVE_SPI_rxBufferFull = 0u;
            }
        }

        rxData = EVE_SPI_rxBuffer[EVE_SPI_rxBufferRead];

        EVE_SPI_EnableRxInt();

    #else

        rxData = CY_GET_REG8(EVE_SPI_RXDATA_PTR);

    #endif /* (EVE_SPI_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: EVE_SPI_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  EVE_SPI_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  EVE_SPI_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 EVE_SPI_GetRxBufferSize(void) 
{
    uint8 size;

    #if(EVE_SPI_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        EVE_SPI_DisableRxInt();

        if(EVE_SPI_rxBufferRead == EVE_SPI_rxBufferWrite)
        {
            size = 0u;
        }
        else if(EVE_SPI_rxBufferRead < EVE_SPI_rxBufferWrite)
        {
            size = (EVE_SPI_rxBufferWrite - EVE_SPI_rxBufferRead);
        }
        else
        {
            size = (EVE_SPI_RX_BUFFER_SIZE - EVE_SPI_rxBufferRead) + EVE_SPI_rxBufferWrite;
        }

        EVE_SPI_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (EVE_SPI_RX_STATUS_REG & EVE_SPI_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: EVE_SPI_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  EVE_SPI_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  EVE_SPI_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  EVE_SPI_GetTxBufferSize(void) 
{
    uint8 size;

    #if(EVE_SPI_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        EVE_SPI_DisableTxInt();

        if(EVE_SPI_txBufferRead == EVE_SPI_txBufferWrite)
        {
            size = 0u;
        }
        else if(EVE_SPI_txBufferRead < EVE_SPI_txBufferWrite)
        {
            size = (EVE_SPI_txBufferWrite - EVE_SPI_txBufferRead);
        }
        else
        {
            size = (EVE_SPI_TX_BUFFER_SIZE - EVE_SPI_txBufferRead) + EVE_SPI_txBufferWrite;
        }

        EVE_SPI_EnableTxInt();

    #else

        size = EVE_SPI_TX_STATUS_REG;

        if(0u != (size & EVE_SPI_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & EVE_SPI_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = EVE_SPI_FIFO_SIZE;
        }

    #endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: EVE_SPI_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EVE_SPI_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  EVE_SPI_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EVE_SPI_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(EVE_SPI_RX_STATUS_REG & EVE_SPI_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(EVE_SPI_RXDATA_PTR);
    }

    #if(EVE_SPI_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        EVE_SPI_DisableRxInt();

        EVE_SPI_rxBufferFull  = 0u;
        EVE_SPI_rxBufferRead  = 0u;
        EVE_SPI_rxBufferWrite = 0u;

        EVE_SPI_EnableRxInt();
    #endif /* (EVE_SPI_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: EVE_SPI_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EVE_SPI_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  EVE_SPI_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EVE_SPI_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    EVE_SPI_AUX_CONTROL_DP0_REG |= ((uint8)  EVE_SPI_TX_FIFO_CLR);
    EVE_SPI_AUX_CONTROL_DP0_REG &= ((uint8) ~EVE_SPI_TX_FIFO_CLR);

    #if(EVE_SPI_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        EVE_SPI_AUX_CONTROL_DP1_REG |= ((uint8)  EVE_SPI_TX_FIFO_CLR);
        EVE_SPI_AUX_CONTROL_DP1_REG &= ((uint8) ~EVE_SPI_TX_FIFO_CLR);
    #endif /* (EVE_SPI_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(EVE_SPI_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        EVE_SPI_DisableTxInt();

        EVE_SPI_txBufferFull  = 0u;
        EVE_SPI_txBufferRead  = 0u;
        EVE_SPI_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        EVE_SPI_TX_STATUS_MASK_REG &= ((uint8) ~EVE_SPI_STS_TX_FIFO_NOT_FULL);

        EVE_SPI_EnableTxInt();
    #endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != EVE_SPI_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: EVE_SPI_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void EVE_SPI_TxEnable(void) 
    {
        EVE_SPI_CONTROL_REG |= EVE_SPI_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: EVE_SPI_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void EVE_SPI_TxDisable(void) 
    {
        EVE_SPI_CONTROL_REG &= ((uint8) ~EVE_SPI_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != EVE_SPI_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: EVE_SPI_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EVE_SPI_PutArray(const uint8 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        EVE_SPI_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: EVE_SPI_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void EVE_SPI_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(EVE_SPI_RX_STATUS_REG & EVE_SPI_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG8(EVE_SPI_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    EVE_SPI_AUX_CONTROL_DP0_REG |= ((uint8)  EVE_SPI_TX_FIFO_CLR);
    EVE_SPI_AUX_CONTROL_DP0_REG &= ((uint8) ~EVE_SPI_TX_FIFO_CLR);

    #if(EVE_SPI_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        EVE_SPI_AUX_CONTROL_DP1_REG |= ((uint8)  EVE_SPI_TX_FIFO_CLR);
        EVE_SPI_AUX_CONTROL_DP1_REG &= ((uint8) ~EVE_SPI_TX_FIFO_CLR);
    #endif /* (EVE_SPI_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: EVE_SPI_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void EVE_SPI_EnableInt(void) 
{
    EVE_SPI_EnableRxInt();
    EVE_SPI_EnableTxInt();
}


/*******************************************************************************
* Function Name: EVE_SPI_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void EVE_SPI_DisableInt(void) 
{
    EVE_SPI_DisableTxInt();
    EVE_SPI_DisableRxInt();
}


/*******************************************************************************
* Function Name: EVE_SPI_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void EVE_SPI_SetInterruptMode(uint8 intSrc) 
{
    EVE_SPI_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~EVE_SPI_STS_SPI_IDLE));
    EVE_SPI_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: EVE_SPI_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  EVE_SPI_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 EVE_SPI_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(EVE_SPI_TX_SOFTWARE_BUF_ENABLED || EVE_SPI_RX_SOFTWARE_BUF_ENABLED)

        EVE_SPI_DisableInt();

        tmpStatus  = EVE_SPI_GET_STATUS_RX(EVE_SPI_swStatusRx);
        tmpStatus |= EVE_SPI_GET_STATUS_TX(EVE_SPI_swStatusTx);
        tmpStatus &= ((uint8) ~EVE_SPI_STS_SPI_IDLE);

        EVE_SPI_swStatusTx = 0u;
        EVE_SPI_swStatusRx = 0u;

        EVE_SPI_EnableInt();

    #else

        tmpStatus  = EVE_SPI_RX_STATUS_REG;
        tmpStatus |= EVE_SPI_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~EVE_SPI_STS_SPI_IDLE);

    #endif /* (EVE_SPI_TX_SOFTWARE_BUF_ENABLED || EVE_SPI_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/* [] END OF FILE */
