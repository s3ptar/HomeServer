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
    void ft800memWrite16(unsigned long ftAddress, unsigned int ftData16);
    void ft800memWrite32(unsigned long ftAddress, unsigned long ftData32);
    unsigned char ft800memRead8(unsigned long ftAddress);
    unsigned int ft800memRead16(unsigned long ftAddress);
    unsigned long ft800memRead32(unsigned long ftAddress);
    void ft800cmdWrite(unsigned char ftCommand);
 };
 
extern FT800_HAL ft800HAL;

#endif /* _FT800_HAL.h_H_ */