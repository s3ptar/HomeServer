/***********************************************************************
*! \file:                   FT800_HAL.c
*  \projekt:                FT800_ESP
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/

/***********************************************************************
* Includes
***********************************************************************/
#include "FT800_HAL.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Declarations
***********************************************************************/
#define Backlight_off HIGH
#define Backlight_on  LOW
#define Audio_off HIGH
#define Audio_on  LOW
/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* local Variable
***********************************************************************/

/***********************************************************************
* Local Funtions
***********************************************************************/

/***********************************************************************
*! \fn			FT800_HAL::ft800init(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
*  \brief		Cunstructor from SPI Transferclass
*  \param		int8_t sck - serial clock pin
*  \param		int8_t miso - master in slave out pin
*  \param		int8_t mosi - master out slave in pin
*  \param		int8_t ss - Slave select pin
*  \exception	none
*  \return		none
***********************************************************************/
void FT800_HAL::ft800init(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
 {
    /* SPI library specific */
	  SPI.begin(sck, miso, mosi, ss);
    SPI.setBitOrder(MSBFIRST);			// Send data most significant bit first
  	SPI.setDataMode(SPI_MODE0);			// Clock idle zero, clock data into FT800 on rising edge
  	SPI.setClockDivider(SPI_CLOCK_DIV4);		// Set rate at 4MHz (16MHz OSC / 4)

    this->ft800csPin = ss;
    
  	pinMode(ft800csPin, OUTPUT);			// FT800 SPI bus CS# input
  
  	digitalWrite(ft800csPin, HIGH);		// Set CS# high to start - SPI inactive
  	
    
 }
 
 FT800_HAL::FT800_HAL()
 {
 }
FT800_HAL::~FT800_HAL()
 {
 }


 /******************************************************************************
 * Function:        void ft800memWritexx(ftAddress, ftDataxx, ftLength)
 * PreCondition:    None
 * Input:           ftAddress = FT800 memory space address
 *                  ftDataxx = a byte, int or long to send
 * Output:          None
 * Side Effects:    None
 * Overview:        Writes FT800 internal address space
 * Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
void FT800_HAL::ft800memWrite8(unsigned long ftAddress, unsigned char ftData8)
{
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ftData8);			// Send data byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}

void FT800_HAL::ft800memWrite8Array(unsigned long ftAddress, uint8_t *Src, uint32_t NBytes)
{
  uint32_t i;
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  for(i=0;i<NBytes;i++)
			SPI.transfer(*Src++);
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}


void FT800_HAL::ft800memWrite16(unsigned long ftAddress, unsigned int ftData16)
{
//  digitalWrite(triggerPin, HIGH);		// Toggle a pin to trigger the oscilloscope
//  digitalWrite(triggerPin, LOW);		// Toggle a pin to trigger the oscilloscope
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer((char)(ftData16));		// Send data low byte
  SPI.transfer((char)(ftData16 >> 8));		// Send data high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}


void FT800_HAL::ft800memWrite32(unsigned long ftAddress, unsigned long ftData32)
{
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer((char)(ftData32));		// Send data low byte
  SPI.transfer((char)(ftData32 >> 8));
  SPI.transfer((char)(ftData32 >> 16));
  SPI.transfer((char)(ftData32 >> 24));		// Send data high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}
/******************************************************************************
 * Function:        unsigned char ft800memReadxx(ftAddress, ftLength)
 * PreCondition:    None
 * Input:           ftAddress = FT800 memory space address
 * Output:          ftDataxx (byte, int or long)
 * Side Effects:    None
 * Overview:        Reads FT800 internal address space
 * Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
unsigned char FT800_HAL::ft800memRead8(unsigned long ftAddress)
{
  unsigned char ftData8 = ZERO;
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ZERO);				// Send dummy byte
ftData8 = SPI.transfer(ZERO);		// Read data byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
  return ftData8;				// Return byte read
}

unsigned int FT800_HAL::ft800memRead16(unsigned long ftAddress)
{
  unsigned int ftData16;
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ZERO);				// Send dummy byte
    ftData16 = (SPI.transfer(ZERO));		// Read low byte
    ftData16 = (SPI.transfer(ZERO) << 8) | ftData16; // Read high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS#hHigh
  return ftData16;				// Return integer read
}

void FT800_HAL::ft800memRead8Array(uint32_t Addr, uint8_t *Src, uint32_t NBytes){

    uint32_t i;
		StartRead(Addr);
		for(i=0;i<NBytes;i++)
			*Src++ = SPI.transfer(0x00);
    digitalWrite(ft800csPin, HIGH);		// Set CS# high

}

unsigned long FT800_HAL::ft800memRead32(unsigned long ftAddress)
{
  unsigned long ftData32;
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ); // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 8));		// Send middle address byte
  SPI.transfer((char)(ftAddress));		// Send low address byte
  SPI.transfer(ZERO);				// Send dummy byte
    ftData32 = (SPI.transfer(ZERO));		// Read low byte
    ftData32 = (SPI.transfer(ZERO) << 8) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 16) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 24) | ftData32; // Read high byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
  return ftData32;				// Return long read
}
/******************************************************************************
 * Function:        void ft800cmdWrite(ftCommand)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Sends FT800 command
 * Note:            None
 *****************************************************************************/
void FT800_HAL::ft800cmdWrite(unsigned char ftCommand)
{
  digitalWrite(ft800csPin, LOW);		// Set CS# low
  SPI.transfer(ftCommand);			// Send command
  SPI.transfer(0x00);				// Commands consist of two more zero bytes
  SPI.transfer(0x00);				// Send last zero byte
  digitalWrite(ft800csPin, HIGH);		// Set CS# high
}


/***********************************************************************
*! \fn          void FT800_IMP::SetDisplayEnablePin(uint8_t GpioBit)
*  \brief       FT_GC gpio bit for display enable/disable
*  \param       uint8_t GpioBit
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::SetDisplayEnablePin(uint8_t GpioBit){

    pinMode(GpioBit, OUTPUT);			// FT800 Power Down (reset) input
    digitalWrite(GpioBit, HIGH);		// Set PD# high to start
	
}


/***********************************************************************
*! \fn          void FT800_HAL::SetAudioEnablePin(uint8_t GpioBit)
*  \brief       FT_GC gpio bit for audio enable/disable
*  \param       uint8_t GpioBit
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::SetAudioEnablePin(uint8_t GpioBit){

    pinMode(GpioBit, OUTPUT);			// FT800 Power Down (reset) input
    digitalWrite(GpioBit, HIGH);		// 
  
}

/***********************************************************************
*! \fn          void FT800_IMP::DisplayOn(uint8_t GpioBit)
*  \brief       Apis to enable backlight
*  \param       uint8_t GpioBit
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::DisplayOn(uint8_t GpioBit){

    digitalWrite(GpioBit, Backlight_on);
	
}
/***********************************************************************
*! \fn          void FT800_IMP::DisplayOff(uint8_t GpioBit)
*  \brief       Apis to disable backlight
*  \param       uint8_t GpioBit
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::DisplayOff(uint8_t GpioBit){

    digitalWrite(GpioBit, Backlight_off);

}


/***********************************************************************
*! \fn          void AudioOn(uint8_t GpioBit)
*  \brief       Apis to enable Audio
*  \param       uint8_t GpioBit
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::AudioOn(uint8_t GpioBit){

    digitalWrite(GpioBit, Audio_on);

}
/***********************************************************************
*! \fn          void FT800_HAL::AudioOff(uint8_t GpioBit)
*  \brief       Apis to edisble Audio
*  \param       uint8_t GpioBit
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::AudioOff(uint8_t GpioBit){

    digitalWrite(GpioBit, Audio_off);

}

/***********************************************************************
*! \fn          void FT800_HAL::SetInterruptPin(uint16_t Intpin)
*  \brief       apis to set interrupt pin
*  \param       uint16_t Intpin
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::SetInterruptPin(uint16_t Intpin){
  
    /* update the interrupt pin */
	  IntPin = Intpin;

}

/***********************************************************************
*! \fn          void FT800_HAL::ChangeClock(uint32_t ClockValue)
*  \brief       For due it is straight forward, for others only a switch
*  \param       uint32_t ClockValue
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::ChangeClock(uint32_t ClockValue){
    SPI.setClockDivider(ClockValue);
}


/***********************************************************************
*! \fn          void FT800_HAL::StartRead(uint32_t Addr)
*  \brief       APIs related to memory read & write/transport
*  \param       uint32_t Addr
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::StartRead(uint32_t Addr){
    //make sure the union array indexes are modified for big endian usecase
		union{
			  uint32_t UUint32;
			  uint8_t  A[4];
		};
		UUint32 = Addr;
		digitalWrite(ft800csPin, LOW);
		SPI.transfer(A[2]);
		SPI.transfer(A[1]);
		SPI.transfer(A[0]);
		SPI.transfer(0x00);//dummy byte for read
	}

/***********************************************************************
*! \fn          void FT800_HAL::StartWrite(uint32_t Addr)
*  \brief       APIs related to memory read & write/transport
*  \param       uint32_t Addr
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::StartWrite(uint32_t Addr){
    //make sure the union array indexes are modified for big endian usecase
		union{
			  uint32_t UUint32;
			  uint8_t  A[4];
		};
		UUint32 = Addr;
		digitalWrite(ft800csPin, LOW);
		SPI.transfer(A[2] | 0x80);
		SPI.transfer(A[1]);
		SPI.transfer(A[0]);
	}

/***********************************************************************
*! \fn          void FT800_HAL::Transfer(uint8_t *Buff,uint32_t NBytes)
*  \brief       transfer n bytes
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::Transfer(uint8_t *Buff,uint32_t NBytes){

    uint32_t i;
		for(i=0;i<NBytes;i++){
			  SPI.transfer(*Buff++);
		}

}
/***********************************************************************
*! \fn          void FT800_HAL::Transfer32(uint32_t Value32)
*  \brief       transfer 4 bytes
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::Transfer32(uint32_t Value32){

    union{
			  uint32_t UUint32;
			  uint8_t  A[4];
		};
		UUint32 = Value32;		
		SPI.transfer(A[0]);
		SPI.transfer(A[1]);
		SPI.transfer(A[2]);
		SPI.transfer(A[3]);

}
/***********************************************************************
*! \fn          void FT800_HAL::Transfer16(uint16_t Value16)
*  \brief       transfer 2 bytes
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::Transfer16(uint16_t Value16){

    union{
			  uint16_t UUint16;
			  uint8_t  A[2];
		};
		UUint16 = Value16;
		A[0] = SPI.transfer(A[0]);
		A[1] = SPI.transfer(A[1]);

}

/***********************************************************************
*! \fn          void FT800_HAL::Transfer8(uint8_t Value8)
*  \brief       transfer a single byte
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::Transfer8(uint8_t Value8){

    SPI.transfer(Value8);

}
/***********************************************************************
*! \fn          void FT800_HAL::EndTransfer()
*  \brief       end transfer
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::EndTransfer(){

    digitalWrite(ft800csPin, HIGH);

}
/***********************************************************************
*! \fn          void FT800_HAL::StartTransfer(uint32_t Addr)
*  \brief       start transfer
*  \param       uint32_t Addr
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_HAL::StartTransfer(uint32_t Addr){

    union{
			  uint32_t UUint32;
			  uint8_t  A[4];
		};
		UUint32 = Addr;
		digitalWrite(ft800csPin, LOW);
		SPI.transfer(A[2] | 0x80);
		SPI.transfer(A[1]);
		SPI.transfer(A[0]);  

}