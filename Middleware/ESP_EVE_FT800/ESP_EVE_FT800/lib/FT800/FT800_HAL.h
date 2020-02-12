/***********************************************************************
*! \file:                   FT800_HAL.h
*  \projekt:                FT800_ESP
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/

#ifndef _FT800_HAL_H_
#define _FT800_HAL_H_

/***********************************************************************
 * Includes
 **********************************************************************/
#include "SPI.h"
#include "Arduino.h"
#include "FT800.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array

/***********************************************************************
 * Declarations
 **********************************************************************/
// FT800 Memory Commands - use with ft800memWritexx and ft800memReadxx
#define MEM_WRITE	0x80			// FT800 Host Memory Write 
#define MEM_READ	0x00			// FT800 Host Memory Read
/***********************************************************************
 * Global Variable
 **********************************************************************/

/***********************************************************************
 * Constant
 **********************************************************************/

/***********************************************************************
 * Macros
 **********************************************************************/

/***********************************************************************
 * Funtions
 **********************************************************************/

 class FT800_HAL 
 {
 private:
     /* data */
    int8_t ft800csPin;
    
 public:
    //Constructor
    //FT800_HAL(int8_t sck, int8_t miso, int8_t mosi, int8_t ss);
    FT800_HAL();
    //Destructor
    ~FT800_HAL();
    void ft800init(int8_t sck, int8_t miso, int8_t mosi, int8_t ss);
    void ft800memWrite8(unsigned long ftAddress, unsigned char ftData8);
    void ft800memWrite8Array(unsigned long ftAddress, uint8_t *Src, uint32_t NBytes);
    void ft800memWrite16(unsigned long ftAddress, unsigned int ftData16);
    void ft800memWrite32(unsigned long ftAddress, unsigned long ftData32);
    unsigned char ft800memRead8(unsigned long ftAddress);
    void ft800memRead8Array(uint32_t Addr, uint8_t *Src, uint32_t NBytes);
    unsigned int ft800memRead16(unsigned long ftAddress);
    unsigned long ft800memRead32(unsigned long ftAddress);
    void ft800cmdWrite(unsigned char ftCommand);
    void SetDisplayEnablePin(uint8_t GpioBit);//FT_GC gpio bit for display enable/disable
	 void SetAudioEnablePin(uint8_t GpioBit);//FT_GC gpio bit for audio enable/disable
    void DisplayOn(uint8_t GpioBit);
    void DisplayOff(uint8_t GpioBit);
    void AudioOn(uint8_t GpioBit);
	 void AudioOff(uint8_t GpioBit);
	 void SetInterruptPin(uint16_t Intpin);//apis to set interrupt pin
    void ChangeClock(uint32_t ClockValue);
    void StartRead(uint32_t Addr);
    void StartWrite(uint32_t Addr);
    void Transfer(uint8_t *Buff,uint32_t NBytes);
    void Transfer32(uint32_t Value32);
    void Transfer16(uint16_t Value16);
    void Transfer8(uint8_t Value8);
    void EndTransfer(void);
    void StartTransfer(uint32_t Addr);

    /* Global variables */	
	 uint8_t  GInit,PDNPin,TrnsFlag;//Global flag to indicate that initialization is done
	 int32_t  GError;//Global error flag
	 uint16_t IntPin;//interrupt pin	
	 uint8_t  DispGpioPin,AudioGpioPin;//default FT_GC pin assignments for diaplay and audio control	
	
	 uint16_t CmdFifoWp,FreeSpace;//command fifo write pointer
 };
 
extern FT800_HAL ft800HAL;

#endif /* _FT800_HAL.h_H_ */