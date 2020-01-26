/*******************************************************************************
* File Name: PSoCEve_Hal.c  
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

#include <project.h>
#include "PSoCEve_Hal.h"

// Define SS bus pin delay in uS.
#define SS_DELAY        1

#define CONCATENATE(name, function)                 CONCAT(name, function)
#define CONCAT(name, function)                      name##function

#if (CY_PSOC4)
    
    #define mmSPI_SS(value)                             CONCATENATE(SPI_SS_NAME, _Write(value))
    #define mmSPI_SpiUartClearTxBuffer                  CONCATENATE(SPI_NAME, _SpiUartClearTxBuffer)
    #define mmSPI_SpiUartClearRxBuffer                  CONCATENATE(SPI_NAME, _SpiUartClearRxBuffer)    
    #define mmSPI_SpiUartPutArray(a, b)                 CONCATENATE(SPI_NAME, _SpiUartPutArray(a, b)) 
    #define mmSPI_SpiUartWriteTxData(value)             CONCATENATE(SPI_NAME, _SpiUartWriteTxData(value))
    #define mmSPI_SpiUartReadRxData                     CONCATENATE(SPI_NAME, _SpiUartReadRxData)
    #define mmSPI_GetMasterInterruptSource              CONCATENATE(SPI_NAME, _GetMasterInterruptSource)
    #define mmSPI_ClearMasterInterruptSource(value)     CONCATENATE(SPI_NAME, _ClearMasterInterruptSource(value)) 
    #define mmSPI_INTR_MASTER_SPI_DONE                  CONCATENATE(SPI_NAME, _INTR_MASTER_SPI_DONE)  
    
    #define mSPI_WAIT_TXDONE()  while(0u == (mmSPI_GetMasterInterruptSource() & mmSPI_INTR_MASTER_SPI_DONE)) {} \
                                mmSPI_ClearMasterInterruptSource(mmSPI_INTR_MASTER_SPI_DONE);    

#endif
        
#if (CY_PSOC5LP) 

    #define mmSPI_SS(value)                             CONCATENATE(SPI_SS_NAME, _Write(value))
    #define mmSPI_SpiUartClearTxBuffer                  CONCATENATE(SPI_NAME, _ClearTxBuffer)
    #define mmSPI_SpiUartClearRxBuffer                  CONCATENATE(SPI_NAME, _ClearRxBuffer)    
    #define mmSPI_SpiUartPutArray(a, b)                 CONCATENATE(SPI_NAME, _PutArray(a, b)) 
    #define mmSPI_SpiUartWriteTxData(value)             CONCATENATE(SPI_NAME, _WriteTxData(value))
    #define mmSPI_SpiUartReadRxData                     CONCATENATE(SPI_NAME, _ReadRxData)
    #define mmSPI_GetMasterInterruptSource              CONCATENATE(SPI_NAME, _GetMasterInterruptSource)
    #define mmSPI_ClearMasterInterruptSource(value)     CONCATENATE(SPI_NAME, _ClearMasterInterruptSource(value)) 
    #define mmSPI_INTR_MASTER_SPI_DONE                  CONCATENATE(SPI_NAME, _INTR_MASTER_SPI_DONE) 
    #define mmSPI_GetTxBufferSize                       CONCATENATE(SPI_NAME, _GetTxBufferSize)
    
    #define mSPI_WAIT_TXDONE()  while (mmSPI_GetTxBufferSize()) {};    
    
#endif 



// FT800 Memory Commands
#define MEMORY_WRITE    0x80			// FT800 Host Memory Write 
#define MEMORY_READ		0x00			// FT800 Host Memory Read  

unsigned char spiTransferInProgress = 0;

/*******************************************************************************
* Function Name: FTCommandWrite
********************************************************************************
*
* Summary:
*  Send a command to FT chip.
*  Every command is 1 byte long and have to be followed by 2 bytes with 0 value.
*
* Parameters:
*  command: command to be sent.
*
* Return:
*  none
*
*******************************************************************************/
void FTCommandWrite(uint8 command)
{
    unsigned char tobesent[3] = {command, 0x00, 0x00};
    
    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);

    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(tobesent, 3);
    
    mSPI_WAIT_TXDONE();
    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);
}

/*******************************************************************************
* Function Name: FT_Write_Byte
********************************************************************************
*
* Summary:
*  Send a byte to specific address of FT chip.
*
* Parameters:
*  address: memory address of FT chip.
*  data: byte to be written in FT chip.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Write_Byte(uint32 address, uint8 data)
{
    unsigned char tobesent[4] = { (address >> 16) | MEMORY_WRITE, (address >> 8), address, data };
    
    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);

    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(tobesent, 4);
    
    mSPI_WAIT_TXDONE();    

    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);
}

/*******************************************************************************
* Function Name: FT_Read_Byte
********************************************************************************
*
* Summary:
*  Read a byte from specific address of FT chip.
*
* Parameters:
*  address: memory address of FT chip.
*
* Return:
*  byte readed from FT
*
*******************************************************************************/
uint8 FT_Read_Byte(uint32 address)
{
    unsigned char data = 0x00;
    unsigned char tobesent[4] = { (address >> 16) | MEMORY_READ, (address >> 8), address,  0x00 };

    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);

    mmSPI_SpiUartClearTxBuffer();
    
    mmSPI_SpiUartPutArray(tobesent, 4);
    mSPI_WAIT_TXDONE(); 
    
    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartClearRxBuffer();
    mmSPI_SpiUartWriteTxData(0x00);
    mSPI_WAIT_TXDONE();
        
    data = mmSPI_SpiUartReadRxData();

    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);
        
    return data;
}

/*******************************************************************************
* Function Name: FT_Write_UINT32
********************************************************************************
*
* Summary:
*  Write an integer to specific address of FT chip.
*
* Parameters:
*  address: memory address of FT chip.
*  data: integer to be written in FT chip.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Write_UINT32(uint32 address, uint32 data)
{
    unsigned char tobesent[7] =  { (address >> 16), (address >> 8), address,  data, (data >> 8), (data >> 16), (data >> 24) };
        
    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);

    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(tobesent, 7);
    
    mSPI_WAIT_TXDONE();    

    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);
}

/*******************************************************************************
* Function Name: FT_Read_UINT32
********************************************************************************
*
* Summary:
*  Read an integer from specific address of FT chip.
*
* Parameters:
*  address: memory address of FT chip.
*
* Return:
*  integer readed from FT
*
*******************************************************************************/
uint32 FT_Read_UINT32(uint32 address)
{
    uint32 data = 0x00000000;
    unsigned char tobesent[4] = { (address >> 16), (address >> 8), address,  0x00 };
        
    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);

    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(tobesent, 4);
    mSPI_WAIT_TXDONE(); 
    
    mmSPI_SpiUartClearRxBuffer();
    mmSPI_SpiUartWriteTxData(0x00);
    mmSPI_SpiUartWriteTxData(0x00); 
    mmSPI_SpiUartWriteTxData(0x00);
    mmSPI_SpiUartWriteTxData(0x00);
    mSPI_WAIT_TXDONE();  
    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);
    
    data = mmSPI_SpiUartReadRxData() | (mmSPI_SpiUartReadRxData() << 8)| (mmSPI_SpiUartReadRxData() << 16)| (mmSPI_SpiUartReadRxData() << 24);

    return data;
}

//uint32 FTMemoryReadUint32(uint32 address)
//{
//    uint32 data = 0x0000;
//    unsigned char tobesent[4] = { (address >> 16) | MEMORY_READ, (address >> 8), address,  0x00 };
//    
//    SPI_EVE_ss0_m_Write(0);
//    CyDelay(5);
//
//    SPI_EVE_SpiUartClearTxBuffer();
//    SPI_EVE_SpiUartPutArray(tobesent, 4);
//    mSPI_EVE_WAIT_TXDONE();  
//    
//    SPI_EVE_SpiUartClearRxBuffer();
//    SPI_EVE_SpiUartWriteTxData(0x00);
//    SPI_EVE_SpiUartWriteTxData(0x00);
//    SPI_EVE_SpiUartWriteTxData(0x00);
//    SPI_EVE_SpiUartWriteTxData(0x00);    
//    mSPI_EVE_WAIT_TXDONE();  
//    CyDelay(5);
//    SPI_EVE_ss0_m_Write(1);
//
//    data = SPI_EVE_SpiUartReadRxData() | (SPI_EVE_SpiUartReadRxData() << 8) | (SPI_EVE_SpiUartReadRxData() << 16) | (SPI_EVE_SpiUartReadRxData() << 24);
//    
//    return data;    
//}

/*******************************************************************************
* Function Name: FT_Transfer_Start
********************************************************************************
*
* Summary:
*  Start a SPI bus transfer sending address where to start writing or reading
*  and placing SS signal to low state.
*  SS signal stays low until FT_Transfer_End is called.
*
* Parameters:
*  address: memory address of FT chip.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Transfer_Start(uint32 address)
{
    unsigned char tobesent[] =  { (address >> 16), (address >> 8), address };
        
    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);

    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(tobesent, 3);
    
    mSPI_WAIT_TXDONE();    
}

/*******************************************************************************
* Function Name: FT_Transfer_End
********************************************************************************
*
* Summary:
*  Finish an SPI bus transfer. Place SS signal to low state.
*
* Parameters:
*  none
*
* Return:
*  none
*
*******************************************************************************/
void FT_Transfer_End()
{
    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);
}

/*******************************************************************************
* Function Name: FT_Send_byte
********************************************************************************
*
* Summary:
*  Send a byte to SPI bus. This function doesn´t manage SS signal. 
*  It is suposed that an SPI bus transfer was initiated previously.
*
* Parameters:
*  data: byte to be sent.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Send_Byte(uint8 data)
{
    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartWriteTxData(data);
    
    mSPI_WAIT_TXDONE();
}

/*******************************************************************************
* Function Name: FT_Send_ByteArray
********************************************************************************
*
* Summary:
*  Send a byte array to SPI bus. This function doesn´t manage SS signal. 
*  It is suposed that an SPI bus transfer was initiated previously.
*
* Parameters:
*  data: pointer to byte array to be sent
*  size: number of byte in array.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Send_ByteArray(const uint8 *data, uint32 size)
{
    uint32 bufIndex = 0;

    mmSPI_SpiUartClearTxBuffer();
    
    while(size > 0u)
    {
        mmSPI_SpiUartWriteTxData(data[bufIndex]);
        bufIndex++;
        size--;
    }
    
    mSPI_WAIT_TXDONE(); 
    
    /* 12/05/2016 Reworked.
       PSoC 4 API uses a uint32 for 'size'; while PSoC 5 API uses a uint8 for 'size'.
    
    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(data, size);
        
    mSPI_WAIT_TXDONE();    
    
    */
}

void FT_Send_ByteArray_S(const uint8 *data, uint32 size)
{
    uint32 bufIndex = 0;
    
    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);
    
    mmSPI_SpiUartClearTxBuffer();
    
    while(size > 0u)
    {
        mmSPI_SpiUartWriteTxData(data[bufIndex]);
        bufIndex++;
        size--;
    }    
        
    mSPI_WAIT_TXDONE();    
    
    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);    
    
    /* 12/05/2016 Reworked.
       PSoC 4 API uses a uint32 for 'size'; while PSoC 5 API uses a uint8 for 'size'.
    
    mmSPI_SS(0);
    CyDelayUs(SS_DELAY);
    
    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(data, size);
        
    mSPI_WAIT_TXDONE();    
    
    CyDelayUs(SS_DELAY);
    mmSPI_SS(1);    
    
    */
}

/*******************************************************************************
* Function Name: FT_Send_UINT32
********************************************************************************
*
* Summary:
*  Send an integer to SPI bus. This function doesn´t manage SS signal. 
*  It is suposed that an SPI bus transfer was initiated previously.
*
* Parameters:
*  data: integer to be sent.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Send_UINT32(uint32 data)
{
    unsigned char tobesent[4] =  { data, (data >> 8), (data >> 16), (data >> 24) };

    mmSPI_SpiUartClearTxBuffer();
    mmSPI_SpiUartPutArray(tobesent, 4);
    
    mSPI_WAIT_TXDONE();  
}

/* [] END OF FILE */
