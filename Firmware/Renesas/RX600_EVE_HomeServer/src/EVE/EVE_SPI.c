/* THIS SAMPLE CODE IS PROVIDED AS IS        */
/* AND IS SUBJECT TO ALTERATIONS. GLYN       */
/* ACCEPTS NO RESPONSIBILITY OR LIABILITY    */
/* FOR ANY ERRORS OR                         */
/* ELIGIBILITY FOR ANY PURPOSES.             */
/* (C) GLYN GmbH & Co. KG                    */
/*********************************************
Project : EVE_SPI.c
          Graphic library
          EVE with EFM32GG980
          SPI-Bus
          Display: ET0430G0DH6
Version : 0.1 Status: Demo
Date    : 21.08.2013

Author  : P.Doerwald / GLYN GmbH & Co. KG

Releases: V0.1

Info    : O
*********************************************/

#include "EVE_SPI.h" 
#include "EVE.h" 

#include "copro_commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "iodefine.h"                  


#include "FT_Gpu.h"

#include <math.h>





////////////// Values for MEMORY_ACCESS_CONTROL register ///////////////////////
#define DEG0_NORMAL     0x08
#define DEG0_BMP        0x88
#define DEG90_NORMAL    0xA8
#define DEG90_BMP       0xE8
#define DEG180_NORMAL   0xC8      
#define DEG180_BMP      0x48
#define DEG270_NORMAL   0x68
#define DEG270_BMP      0x28

#define U8              unsigned char
#define U16             unsigned int
#define U32             unsigned long int
#define I32             long int

#define ft_pgm_read_byte(x)        (*(x))

#define Pi  3.14159
//////////////////// SPI Transfer description byte /////////////////////////////

//#define ID 1					// Use this if BS0 is connected to High
  #define ID 0					// Use this if BS0 is connected to low

///////////////////////////////////////////////////////////////////////////////

#define INDEX_WRITE       (0x70 | DisplayID)
#define REG_WRITE         (0x72 | DisplayID)
#define REG_READ          (0x73 | DisplayID)
#define INDEX_WRITE_WORD  ((INDEX_WRITE)<<8)
#define REG_WRITE_WORD    ((REG_WRITE)<<8)
#define REG_READ_WORD     ((REG_READ)<<8)

#define ChipOn  1      
#define ChipOff 0 

////////////////////////////// Display Variables ///////////////////////////////




U8 fdhdgg[]={60,80,62,80,64,80,65,80,67,80,0,0};

/*------------------------------------------------------------------------------
//::FUNCTION:SelectChip (switch CSN Signal)
------------------------------------------------------------------------------*/
void SelectChipOnOff(U8 ChipOnOff){

    unsigned char go=1;
    U32 timeout=1000000;
        if (ChipOnOff == ChipOn)
            EVE_CS_on;
        else if (ChipOnOff == ChipOff){
            while(go){
                timeout--;
             if(timeout==0)
            	 go=0;
             if(USART_StatusGet())
            	 go=0;  //wait until TXC
            }
            EVE_CS_off;
    }
        
}

void SPI_WRITE (U8 wert){

    send_SPI(wert);
    read_SPI();
}  

U8 SPI_READ(void){
   
     send_SPI(0xFF);
     return (read_SPI());
}  

void SPI2TransmitWord(U16 wert){
    //Low byte first
    SPI_WRITE (Low_Byte(wert));
    SPI_WRITE (High_Byte(wert));
}
void SPI2TransmitByte(U8 wert){
    SPI_WRITE (wert);
}

U8 SPI2ReceiveByte(void){
    SPI_WRITE (0xFF);
    return (read_SPI());
} 

void host_command(U8 cmd){
    SelectChipOnOff(ChipOn);
    SPI_WRITE(cmd);
    SPI_WRITE(0);
    SPI_WRITE(0);
    SelectChipOnOff(ChipOff);
}

void wr8(U32 address, U8 data){
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x80|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI_WRITE (data);
    SelectChipOnOff(ChipOff);
}

void wr16(U32 address, U16 data){
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x80|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI2TransmitWord(data);
    SelectChipOnOff(ChipOff);
}
void wr32(U32 address, U32 data){
    SelectChipOnOff(ChipOn);
    SPI_WRITE (0x80|(address>>16));
    SPI_WRITE (address>>8);
    SPI_WRITE (address);
    SPI2TransmitWord(data);
    SPI2TransmitWord(data>>16);
  
    SelectChipOnOff(ChipOff);
}
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
U32 rd16(U32 address){
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
