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
*! \file:                   EVE_HAL.h
*  \projekt:                PSOC_EVE_Sensorboard
*  \created on:             2020-02-12
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   HAL - Hardware abstraction Layer, define Low Level
                            Funtion here
***********************************************************************/

/***********************************************************************
* Includes
***********************************************************************/
#include "EVE_HAL.h"
#include "SPI_EVE.h"
#include "Utils.h"
#include "SS_EVE.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Local Funtions
***********************************************************************/
void enable_EVE(eve_enable_t enable);
void SPI_WRITE (U8 wert);
/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* Constant
***********************************************************************/

////////////// Values for MEMORY_ACCESS_CONTROL register ///////////////////////
#define DEG0_NORMAL                 0x08
#define DEG0_BMP                    0x88
#define DEG90_NORMAL                0xA8
#define DEG90_BMP                   0xE8
#define DEG180_NORMAL               0xC8      
#define DEG180_BMP                  0x48
#define DEG270_NORMAL               0x68
#define DEG270_BMP                  0x28



#define ft_pgm_read_byte(x)         (*(x))

#define Pi                          3.14159
//////////////////// SPI Transfer description byte /////////////////////////////

//#define ID 1					// Use this if BS0 is connected to High
#define ID 0					// Use this if BS0 is connected to low

///////////////////////////////////////////////////////////////////////////////



////////////////////////////// Display Variables ///////////////////////////////


U8 fdhdgg[]={60,80,62,80,64,80,65,80,67,80,0,0};
unsigned int                       dli,cli;
extern U32 currfreq;// = 40;


/***********************************************************************
*! \fn			void enable_EVE(eve_enable_t enable)
*  \brief		Set or Reset PowerDown Pin
*  \param		eve_enable_t enable - 0 = Powerdowen
*  \exception	none
*  \return		none
***********************************************************************/
void enable_EVE(eve_enable_t enable){
    
    if(enable){
        //Power Up FT Pin
    }else{
        //Power Down FT Pin
    }
    
}
    

/***********************************************************************
*! \fn			void enable_EVE(eve_enable_t enable)
*  \brief		SelectChip (switch CSN Signal)
*  \param		U8 ChipOnOff
*  \exception	none
*  \return		none
***********************************************************************/
void SelectChipOnOff(U8 ChipOnOff){
    unsigned char go=1;
    U32 timeout=1000000;
        if (ChipOnOff == ChipOn){
            //Select SPI Slave
            //EVE_CS_on;
            SS_EVE_Write(1);
        }
        else if (ChipOnOff == ChipOff){
            while(go){
                timeout--;
                if(timeout==0)
                    go=0;    
            } 
        //deselect SPI Slave
        SS_EVE_Write(0);
    } 
        
}

/***********************************************************************
*! \fn			void SPI_WRITE (U8 wert)
*  \brief		Transmit an 8 Bit value
*  \param		U8 wert - value to send
*  \exception	none
*  \return		none
***********************************************************************/
void SPI_WRITE (U8 wert){
    //send 8 BIT SPI here
    SPI_EVE_WriteTxData(wert);
    //read 8 BIT SPI here
    SPI_EVE_ReadRxData();
}  

/***********************************************************************
*! \fn			U8 SPI_READ(void)
*  \brief		read an 8 Bit value
*  \param		U8 wert - value to send
*  \exception	none
*  \return		none
***********************************************************************/
U8 SPI_READ(void){
   
   //send 8 BIT SPI here
   SPI_EVE_WriteTxData(0xFF);
   //read 8 BIT SPI here
   return (SPI_EVE_ReadRxData());
}  


/***********************************************************************
*! \fn			void SPI2TransmitWord(U16 wert)
*  \brief		send an 16 Bit value
*  \param		U16 wert - value to send
*  \exception	none
*  \return		none
***********************************************************************/
void SPI2TransmitWord(U16 wert){
     //Low byte first
    SPI_EVE_WriteTxData(Low_Byte(wert));
    SPI_EVE_WriteTxData(High_Byte(wert));
}

/***********************************************************************
*! \fn			void SPI2TransmitByte(U8 wert)
*  \brief		send an read an 8Bit value
*  \param		U8 wert - value to send
*  \exception	none
*  \return		none
***********************************************************************/
void SPI2TransmitByte(U8 wert){
    SPI_WRITE (wert);
}

/***********************************************************************
*! \fn			U8 SPI2ReceiveByte(void)
*  \brief		read an 8Bit value
*  \param		none
*  \exception	none
*  \return		8 Bit SPI value
***********************************************************************/
U8 SPI2ReceiveByte(void){
    SPI_WRITE (0xFF);
    return (SPI_EVE_ReadRxData());
} 

/***********************************************************************
*! \fn			void host_command(U8 cmd)
*  \brief		send an command
*  \param		U8 cmd - command value
*  \exception	none
*  \return		none
***********************************************************************/
void host_command(U8 cmd){
    SelectChipOnOff(ChipOn);
    SPI_WRITE(cmd); 
    SPI_WRITE(0);
    SPI_WRITE(0);
    SelectChipOnOff(ChipOff);
}

/***********************************************************************
*! \fn			void wr8(U32 address, U8 data)
*  \brief		write an 8 Bit value to an adress
*  \param		U32 address - address for the value
*  \param		U8 data - data for the address
*  \exception	none
*  \return		none
***********************************************************************/
void wr8(U32 address, U8 data){
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x80|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI_WRITE (data);
    SelectChipOnOff(ChipOff);
}

/***********************************************************************
*! \fn			void wr16(U32 address, U16 data)
*  \brief		write an 16 Bit value to an adress
*  \param		U32 address - address for the value
*  \param		U16 data - data for the address
*  \exception	none
*  \return		none
***********************************************************************/
void wr16(U32 address, U16 data){
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x80|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI2TransmitWord(data);
    SelectChipOnOff(ChipOff);
}

/***********************************************************************
*! \fn			void wr16(U32 address, U32 data)
*  \brief		write an 32 Bit value to an adress
*  \param		U32 address - address for the value
*  \param		U32 data - data for the address
*  \exception	none
*  \return		none
***********************************************************************/
void wr32(U32 address, U32 data){
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x80|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI2TransmitWord(data);
    SPI2TransmitWord(data>>16);
  
    SelectChipOnOff(ChipOff);
}

/***********************************************************************
*! \fn			U8 rd8(U32 address)
*  \brief		read an 8 Bit value to an adress
*  \param		U32 address - address for the value	
*  \exception	none
*  \return		U8 data - data from the address
***********************************************************************/
U8 rd8(U32 address){
    U8 data=0;
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x00|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI_WRITE (0x0);    //Dummybyte
    data=SPI_READ();
  
    SelectChipOnOff(ChipOff);
    return (data);
}

/***********************************************************************
*! \fn			U16 rd16(U32 address)
*  \brief		read an 16 Bit value to an adress
*  \param		U16 address - address for the value	
*  \exception	none
*  \return		U16 data - data from the address
***********************************************************************/
U16 rd16(U32 address){
    U32 data=0;
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x00|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI_WRITE (0x0);    //Dummybyte
    data=SPI_READ();
    data|=SPI_READ()<<8;;
  
    SelectChipOnOff(ChipOff);
    return (data);
}

/***********************************************************************
*! \fn			U32 rd32(U32 address)
*  \brief		read an 32 Bit value to an adress
*  \param		U16 address - address for the value	
*  \exception	none
*  \return		U32 data - data from the address
***********************************************************************/
U32 rd32(U32 address){
    U32 data=0;
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x00|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI_WRITE (0xFF);    //Dummybyte
    data=SPI_READ();
    data|=SPI_READ()<<8;;
    data|=SPI_READ()<<16;
    data|=SPI_READ()<<24;
  
    SelectChipOnOff(ChipOff);
    return (data);
}

/***********************************************************************
*! \fn			init_EVE_IO(void)
*  \brief		init Eve Hardware here
*  \param		none	
*  \exception	none
*  \return		none
***********************************************************************/
void init_EVE_IO(void){
    
}

/***********************************************************************
*! \fn			void EVE_Power(U8 on_off)
*  \brief		disable or enable the t800
*  \param		none	
*  \exception	none
*  \return		none
***********************************************************************/
void EVE_Power(U8 on_off){
    
    nPD_EVE_Write(on_off&0x01);
    
}


/* [] END OF FILE */


