/***********************************************************************
*! \file:                   FT800.c
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
#include "FT800_IMP.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Declarations
***********************************************************************/
// FT800 Chip Commands - use with cmdWrite
#define FT800_ACTIVE	0x00			// Initializes FT800
#define FT800_STANDBY	0x41			// Place FT800 in Standby (clk running)
#define FT800_SLEEP	0x42			// Place FT800 in Sleep (clk off)
#define FT800_PWRDOWN	0x50			// Place FT800 in Power Down (core off)
#define FT800_CLKEXT	0x44			// Select external clock source
#define FT800_CLK48M	0x62			// Select 48MHz PLL
#define FT800_CLK36M	0x61			// Select 36MHz PLL
#define FT800_CORERST	0x68			// Reset core - all registers default

// FT800 Memory Commands - use with ft800memWritexx and ft800memReadxx
#define MEM_WRITE	0x80			// FT800 Host Memory Write 
#define MEM_READ	0x00			// FT800 Host Memory Read

// Colors - fully saturated colors defined here
#define RED		0xFF0000UL		// Red
#define GREEN		0x00FF00UL		// Green
#define BLUE		0x0000FFUL		// Blue
#define WHITE		0xFFFFFFUL		// White
#define BLACK		0x000000UL		// Black
/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* local Variable
***********************************************************************/
FT800_HAL ft800HAL;
  	
/***********************************************************************
* Local Funtions
***********************************************************************/

/***********************************************************************
*! \fn			FT800_IMP::FT800(int8_t sck, int8_t miso, int8_t mosi, int8_t ss, int8_t pwrdn, int8_t irqpin)
*  \brief		Cunstructor from SPI Transferclass
*  \param		int8_t sck - serial clock pin
*  \param		int8_t miso - master in slave out pin
*  \param		int8_t mosi - master out slave in pin
*  \param		int8_t ss - Slave select pin
*  \param		int8_t pwrdn - powerdown pin
*  \param		int8_t irqpin - Interruptpin
*  \exception	none
*  \return		none
***********************************************************************/
FT800_IMP::FT800_IMP(int8_t sck, int8_t miso, int8_t mosi, int8_t ss, int8_t pwrdn, int8_t irqpin)
 {
    /* SPI library specific */
    this->ft800pwrPin = pwrdn;
    ft800HAL.ft800init(sck, miso, mosi, ss);
    ft800HAL.SetDisplayEnablePin(ft800pwrPin);
    delay(20);					// Wait a few MS before waking the FT800	
    
 }

/***********************************************************************
*! \fn			bool FT800_Init(void)
*  \brief		Wake the Ft800 up
*  \param		none
*  \exception	none
*  \return		true if okay, else 0
***********************************************************************/
bool FT800_IMP::FT800_Init( ){
  
  	digitalWrite(ft800pwrPin, LOW);		// 1) lower PD#
  	delay(20);					// 2) hold for 20ms
  	digitalWrite(ft800pwrPin, HIGH);		// 3) raise PD#
  	delay(20);					// 4) wait for another 20ms before sending any commands

  	ft800HAL.ft800cmdWrite(FT800_ACTIVE);			// Start FT800
  	delay(5);					// Give some time to process
  	ft800HAL.ft800cmdWrite(FT800_CLKEXT);			// Set FT800 for external clock
  	delay(5);					// Give some time to process
  	ft800HAL.ft800cmdWrite(FT800_CLK48M);			// Set FT800 for 48MHz PLL
  	delay(5);					// Give some time to process
  						// Now FT800 can accept commands at up to 30MHz clock on SPI bus
  						//   This application leaves the SPI bus at 4MHz
	Serial.write("Check FT800!\r\n");		// Send an error message on the UART
  	while (ft800HAL.ft800memRead8(REG_ID) != 0x7C)		// Read ID register - is it 0x7C?
  	{
    	//Serial.write("System Halted!\r\n");		// Send an error message on the UART
    	//while(1);					// If we don't get 0x7C, the ineface isn't working - halt with infinite loop
		return false;
  	}  
  
  	ft800HAL.ft800memWrite8(REG_PCLK, ZERO);		// Set PCLK to zero - don't clock the LCD until later
  	ft800HAL.ft800memWrite8(REG_PWM_DUTY, ZERO);		// Turn off backlight
  
	// End of Wake-up FT800
	//***************************************

   //***************************************
	// Initialize Display
  	ft800HAL.ft800memWrite16(REG_HSIZE,   LCDWIDTH);	// active display width
  	ft800HAL.ft800memWrite16(REG_HCYCLE,  LCDHCYCLE);	// total number of clocks per line, incl front/back porch
  	ft800HAL.ft800memWrite16(REG_HOFFSET, LCDHOFFSET);	// start of active line
  	ft800HAL.ft800memWrite16(REG_HSYNC0,  LCDHSYNC0);	// start of horizontal sync pulse
  	ft800HAL.ft800memWrite16(REG_HSYNC1,  LCDHSYNC1);	// end of horizontal sync pulse
  	ft800HAL.ft800memWrite16(REG_VSIZE,   LCDHEIGHT);	// active display height
  	ft800HAL.ft800memWrite16(REG_VCYCLE,  LCDVCYCLE);	// total number of lines per screen, incl pre/post
  	ft800HAL.ft800memWrite16(REG_VOFFSET, LCDVOFFSET);	// start of active screen
  	ft800HAL.ft800memWrite16(REG_VSYNC0,  LCDVSYNC0);	// start of vertical sync pulse
  	ft800HAL.ft800memWrite16(REG_VSYNC1,  LCDVSYNC1);	// end of vertical sync pulse
  	ft800HAL.ft800memWrite8(REG_SWIZZLE,  LCDSWIZZLE);	// FT800 output to LCD - pin order
  	ft800HAL.ft800memWrite8(REG_PCLK_POL, LCDPCLKPOL);	// LCD data is clocked in on this PCLK edge
						// Don't set PCLK yet - wait for just after the first display list
	// End of Initialize Display
	return true;
}


/******************************************************************************
 * Function:        void incCMDOffset(currentOffset, commandSize)
 * PreCondition:    None
 *                    starting a command list
 * Input:           currentOffset = graphics processor command list pointer
 *                  commandSize = number of bytes to increment the offset
 * Output:          newOffset = the new ring buffer pointer after adding the command
 * Side Effects:    None
 * Overview:        Adds commandSize to the currentOffset.  
 *                  Checks for 4K ring-buffer offset roll-over 
 * Note:            None
 *****************************************************************************/
unsigned int FT800_IMP::incCMDOffset(unsigned int currentOffset, unsigned char commandSize)
{
    unsigned int newOffset;			// used to hold new offset
    newOffset = currentOffset + commandSize;	// Calculate new offset
    if(newOffset > 4095)			// If new offset past boundary...
    {
        newOffset = (newOffset - 4096);		// ... roll over pointer
    }
    return newOffset;				// Return new offset
}


void FT800_IMP::FT800_setup(){

   //***************************************
	// Configure Touch and Audio - not used in this example, so disable both
  	ft800HAL.ft800memWrite8(REG_TOUCH_MODE, ZERO);		// Disable touch
  	ft800HAL.ft800memWrite16(REG_TOUCH_RZTHRESH, ZERO);	// Eliminate any false touches
  
  	ft800HAL.ft800memWrite8(REG_VOL_PB, ZERO);		// turn recorded audio volume down
 	ft800HAL.ft800memWrite8(REG_VOL_SOUND, ZERO);		// turn synthesizer volume down
  	ft800HAL.ft800memWrite16(REG_SOUND, 0x6000);		// set synthesizer to mute
  
	// End of Configure Touch and Audio
	//***************************************

	//***************************************
	// Write Initial Display List & Enable Display

  	ramDisplayList = RAM_DL;			// start of Display List
  	ft800HAL.ft800memWrite32(ramDisplayList, DL_CLEAR_RGB); // Clear Color RGB   00000010 RRRRRRRR GGGGGGGG BBBBBBBB  (R/G/B = Colour values) default zero / black
  	ramDisplayList += 4;				// point to next location
  	ft800HAL.ft800memWrite32(ramDisplayList, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));	// Clear 00100110 -------- -------- -----CST  (C/S/T define which parameters to clear)
  	ramDisplayList += 4;				// point to next location
  	ft800HAL.ft800memWrite32(ramDisplayList, DL_DISPLAY);	// DISPLAY command 00000000 00000000 00000000 00000000 (end of display list)

  	ft800HAL.ft800memWrite32(REG_DLSWAP, DLSWAP_FRAME);	// 00000000 00000000 00000000 000000SS  (SS bits define when render occurs)
						// Nothing is being displayed yet... the pixel clock is still 0x00
  	ramDisplayList = RAM_DL;			// Reset Display List pointer for next list
  
  	ft800Gpio = ft800HAL.ft800memRead8(REG_GPIO);		// Read the FT800 GPIO register for a read/modify/write operation
  	ft800Gpio = ft800Gpio | 0x80;			// set bit 7 of FT800 GPIO register (DISP) - others are inputs
  	ft800HAL.ft800memWrite8(REG_GPIO, ft800Gpio);		// Enable the DISP signal to the LCD panel
  	ft800HAL.ft800memWrite8(REG_PCLK, LCDPCLK);		// Now start clocking data to the LCD panel
  	for(int duty = 128; duty > 0; duty--)
  	{
    	ft800HAL.ft800memWrite8(REG_PWM_DUTY, duty);		// Turn on backlight - ramp up slowly to full brighness
    	delay(10);
  	}

}


void FT800_IMP::FT800_test(){

   // Wait for graphics processor to complete executing the current command list
  	// This happens when REG_CMD_READ matches REG_CMD_WRITE, indicating that all commands
  	// have been executed.  The next commands get executed when REG_CMD_WRITE is updated again...
  	// then REG_CMD_READ again catches up to REG_CMD_WRITE
  	// This is a 4Kbyte ring buffer, so keep pointers within the 4K roll-over
 	 do
  	{
    	cmdBufferRd = ft800HAL.ft800memRead16(REG_CMD_READ);	// Read the graphics processor read pointer
    	cmdBufferWr = ft800HAL.ft800memRead16(REG_CMD_WRITE); // Read the graphics processor write pointer
  	}while (cmdBufferWr != cmdBufferRd);		// Wait until the two registers match
  
  	cmdOffset = cmdBufferWr;			// The new starting point the first location after the last command

  	if (color != WHITE)				// If a red dot was just drawn (or first time through)...
    	color = WHITE;				// change color to white
  	else						// Otherwise...
    	color = RED;				// change the color to red
    
  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (CMD_DLSTART));
						// Start the display list
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer
  

  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR_RGB | BLACK));
						// Set the default clear color to black
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
						// Clear the screen - this and the previous prevent artifacts between lists
						// Attributes are the color, stencil and tag buffers
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_COLOR_RGB | color));
						// Set the color of the following item(s) - toggle red/white from above
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_POINT_SIZE | point_size));
						// Select the size of the dot to draw
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_BEGIN | FTPOINTS));
						// Indicate to draw a point (dot)
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_VERTEX2F | (point_x << 15) | point_y));
						// Set the point center location
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_END));
						// End the point
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (DL_DISPLAY));
						// Instruct the graphics processor to show the list
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite32(RAM_CMD + cmdOffset, (CMD_SWAP));
						// Make this list active
  	cmdOffset = incCMDOffset(cmdOffset, 4);	// Update the command pointer


  	ft800HAL.ft800memWrite16(REG_CMD_WRITE, (cmdOffset));	// Update the ring buffer pointer so the graphics processor starts executing

}	


FT800_IMP::~FT800_IMP()
 {
 }

/***********************************************************************
*! \fn          FT_Status FT800_IMP::Exit()
*  \brief       Exit SPI channel
*  \param       none
*  \exception   none
*  \return      FT_Status
***********************************************************************/
FT_Status FT800_IMP::Exit()
{
	/* Cross check whether SPI channel is used by others modules */
	SPI.end();
	return FT_OK;
}

/***********************************************************************
*! \fn          void FT800_IMP::GetVersion(uint8_t &Major, uint8_t &Minor, uint8_t &Build);
*  \brief       Api to get version of the library
*  \param       uint8_t &Major - Pointer to variable
*  \param       uint8_t &Minor - Pointer to variable
*  \param       uint8_t &Build - Pointer to variable
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::GetVersion(uint8_t &Major, uint8_t &Minor, uint8_t &Build){

	/* Update the version numbers */
	Major = FT_GC_MAJOR;
	Minor = FT_GC_MINOR;
	Build = FT_GC_BUILD;

}

/***********************************************************************
*! \fn          void FT800_IMP::ResetCopro(void)
*  \brief       api to reset only coprocessor	
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::ResetCopro(){

    /* first set the reset bit high */
	ft800HAL.ft800memWrite8(REG_CPURESET,FT_RESET_HOLD_COPROCESSOR);//first hold the coprocessor in reset
	//make the cmd read write pointers to 0
	CmdFifoWp = 0;
	FreeSpace = FT_CMDFIFO_SIZE - 4;
	ft800HAL.ft800memWrite16(REG_CMD_READ,0);
	ft800HAL.ft800memWrite16(REG_CMD_WRITE,0);
	delay(10);//just to make sure reset is fine
	/* release the coprocessors from reset */
	ft800HAL.ft800memWrite8(REG_CPURESET,FT_RESET_RELEASE_COPROCESSOR);
	/* ideally delay of 25ms is required for audio engine to playback mute sound to avoid pop sound */

}
	void Reset(void);//api to reset whole FT_GC via pdn - if pdn is not assigned then reset is not successful
	/* Apis related to power up/power down funcationality */
	void DisplayConfigExternalClock(uint8_t ResType);
/***********************************************************************
*! \fn          void FT800_IMP::ActiveInternalClock(void);
*  \brief       API to set active command, set internal clock and download first DL
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::ActiveInternalClock(void){
	
	uint8_t FT_DLCODE_BOOTUP_index = 0;
    uint8_t FT_DLCODE_BOOTUP[12] ={
        0,0,0,2,//GPU instruction CLEAR_COLOR_RGB - black color
	    7,0,0,38, //GPU instruction CLEAR
	    0,0,0,0,  //GPU instruction DISPLAY
	};
	/* change the SPI clock to <11MHz */
	ft800HAL.ChangeClock(FT_SPI_CLK_FREQ_MIN);
	HostCommand(FT_ACTIVE);//wake up the processor from sleep state
	delay(20);
	/* download the first display list */
	for (FT_DLCODE_BOOTUP_index = 0; FT_DLCODE_BOOTUP_index < 12; FT_DLCODE_BOOTUP_index++)
		ft800HAL.ft800memWrite8(FT_RAM_DL,FT_DLCODE_BOOTUP[FT_DLCODE_BOOTUP_index]);
	/* perform first swap command */
	ft800HAL.ft800memWrite8(REG_DLSWAP,FT_DLSWAP_FRAME);
	//make the cmd read write pointers to 0
	CmdFifoWp = 0;
	FreeSpace = FT_CMDFIFO_SIZE - 4;	
}
/***********************************************************************
*! \fn          void FT800_IMP::HostCommand(uint32_t HostCommand)
*  \brief       construct host command and send to graphics controller
*  \param       uint32_t HostCommand
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::HostCommand(uint32_t HostCommand){
	uint32_t Addr;
    Addr = HostCommand<<16;
	ft800HAL.ft800memRead8(Addr);//ideally sending 3 bytes is sufficient
	delay(20);//worst scenario
}	
/***********************************************************************
*! \fn          void FT800_IMP::PDN_Cycle(void)
*  \brief       Powerdown Cycle	
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::PDN_Cycle(void){

	/* do a power cycle by toggling power down pin followed by active command followed by display list initialization sequence */
	digitalWrite(ft800pwrPin, HIGH);
	delay(20);
	digitalWrite(ft800pwrPin, LOW);
	delay(20);
	digitalWrite(ft800pwrPin, HIGH);
	delay(20);

}
	
/***********************************************************************
*! \fn          void FT800_IMP::EnableInterrupts(uint8_t GEnable,uint8_t Mask)
*  \brief       enable or disable interrupts
*  \param       uint8_t GEnable
*  \param       uint8_t Mask
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::EnableInterrupts(uint8_t GEnable,uint8_t Mask){

	ft800HAL.ft800memWrite8(REG_INT_EN,GEnable);//1 means enable global interrupts, 0 means disable global interrupts
	ft800HAL.ft800memWrite8(REG_INT_MASK,Mask);//0 means interrupts are masked, 1 means interrupts are not masked

}
/***********************************************************************
*! \fn          uint8_t FT800_IMP::ReadIntReg(void)
*  \brief       read the interrupt flag register - note that on FT_GC the interrupts are clear by read
*  \param       none
*  \exception   none
*  \return      interrupt status from ft
***********************************************************************/
uint8_t FT800_IMP::ReadIntReg(void){
	
	return (ft800HAL.ft800memRead8(REG_INT_FLAGS));

}

/***********************************************************************
*! \fn          void FT800_IMP::CalibrateTouchPanel(void)
*  \brief       Calibrate TouchScreen
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::CalibrateTouchPanel(void){

	/* Construct the display list with grey as background color, informative string "Please Tap on the dot" followed by inbuilt calibration command */
	FT_Touch_Enable();
	DLStart();
	ClearColorRGB(64,64,64);
	Clear(1,1,1);    
	ColorRGB(0xff, 0xff, 0xff);
	Cmd_Text((LCDWIDTH /2), (LCDHEIGHT/2), 27, FT_OPT_CENTER, "Please Tap on the dot");
	Cmd_Calibrate(0);
	//Wait for the completion of calibration - either finish can be used for flush and check can be used */
	Finish();

}

/***********************************************************************
*! \fn          void FT800_IMP::DLStart()
*  \brief       //inserts cmd_dlstart() followed by clear(1,1,1) graphics command
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::DLStart(){

	Cmd_DLStart();
	Clear(1,1,1);

}

/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::StartTransferCmd()
*  \brief       assert CSpin and send write command
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::StartTransferCmd(){

	//start write transaction
	ft800HAL.StartWrite(FT_RAM_CMD + CmdFifoWp);
	TrnsFlag = 1;
	return FT_GE_OK;

}

/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::WriteCmd(uint32_t Cmd)
*  \brief       write new List to FT
*  \param       uint32_t Cmd)
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::WriteCmd(uint32_t Cmd){

    FT_GEStatus Status;
#if 0	
	if(0 == TrnsFlag)
		StartTransferCmd();
	Status = TransferCmd(Cmd);
	if(0 == TrnsFlag)
		EndTransferCmd();
#else
	if(0 == TrnsFlag)
	{
		StartTransferCmd();
		Status = TransferCmd(Cmd);
		EndTransferCmd();
	}
	else
	{
		Status = TransferCmd(Cmd);
	}

#endif
	return Status;

}
/***********************************************************************
*! \fn          FT_GEStatus WriteCmd(uint8_t *Src,uint32_t NBytes)
*  \brief       api to send N bytes to command
*  \param       uint8_t *Src
*  \param       uint32_t NBytes
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::WriteCmd(uint8_t *Src,uint32_t NBytes){
	
    FT_GEStatus Status;
	if(0 == TrnsFlag){
		StartTransferCmd();
		Status = TransferCmd(Src, NBytes);
		EndTransferCmd();//here transflag is made to 0
	}
	else
		Status = TransferCmd(Src,NBytes);
		
	return Status;

}

/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Cmd_DLStart(void)
*  \brief       write new List to FT
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Cmd_DLStart(void){
	return ( WriteCmd(CMD_DLSTART) );
}
	
/***********************************************************************
*! \fn          AlphaFunc(uint8_t Func, uint8_t Ref)
*  \brief       APIs related to graphics engine
*  \param       uint8_t Func
*  \param       uint8_t Ref
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::AlphaFunc(uint8_t Func, uint8_t Ref){
	return ( WriteCmd((9UL << 24) | ((Func & 7L) << 8) | ((Ref & 0xFFL) << 0)) );
}

/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Begin(uint8_t Prim)
*  \brief       APIs related to graphics engine
*  \param       uint8_t Prim
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Begin(uint8_t Prim){
	return ( WriteCmd((31UL << 24) | Prim) );
}

/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapHandle(uint8_t Handle)
*  \brief       APIs related to graphics engine
*  \param       uint8_t Handle
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapHandle(uint8_t Handle){
    return ( WriteCmd((5UL << 24) | Handle) );
}

/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapLayout(uint8_t Format, uint16_t Linestride, uint16_t Height)
*  \brief       APIs related to graphics engine
*  \param       uint8_t Format
*  \param       uint16_t Linestride
*  \param       uint16_t Height
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapLayout(uint8_t Format, uint16_t Linestride, uint16_t Height){
    
    // WriteCmd((7UL << 24) | ((format & 0x1FL) << 19) | ((linestride & 0x3FFL) << 9) | ((height & 0x1FFL) << 0));
  	union {
    	uint32_t UUInt32;
    	uint8_t A[4];
    };
    A[0] = Height;
    A[1] = (1 & (Height >> 8)) | (Linestride << 1);
    A[2] = (7 & (Linestride >> 7)) | (Format << 3);
    A[3] = 7;
    return ( WriteCmd(UUInt32) );

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapSize(uint8_t Filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height)
*  \brief       APIs related to graphics engine
*  \param       uint8_t filter
*  \param       uint8_t wrapx
*  \param       uint8_t wrapy
*  \param       uint16_t width
*  \param       uint16_t height
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapSize(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height){

    uint8_t fxy = (filter << 2) | (wrapx << 1) | (wrapy);
    // WriteCmd((8UL << 24) | ((uint32_t)fxy << 18) | ((width & 0x1FFL) << 9) | ((height & 0x1FFL) << 0));
    union {
        uint32_t UUInt32;
        uint8_t A[4];
    };
    A[0] = height;
    A[1] = (1 & (height >> 8)) | (width << 1);
    A[2] = (3 & (width >> 7)) | (fxy << 2);
    A[3] = 8;
    return ( WriteCmd(UUInt32) );

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapSource(uint32_t Addr)
*  \brief       APIs related to graphics engine
*  \param       uint32_t Addr
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapSource(uint32_t Addr){
    return ( WriteCmd((1UL << 24) | ((Addr & 0xFFFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapTransformA(int32_t A)
*  \brief       APIs related to graphics engine
*  \param       int32_t A
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapTransformA(int32_t A){
    return ( WriteCmd((21UL << 24) | ((A & 0x1FFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapTransformB(int32_t B)
*  \brief       APIs related to graphics engine
*  \param       int32_t B
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapTransformB(int32_t B){
    return ( WriteCmd((22UL << 24) | ((B & 0x1FFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapTransformC(int32_t C)
*  \brief       APIs related to graphics engine
*  \param       int32_t C
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapTransformC(int32_t C){
    return ( WriteCmd((23UL << 24) | ((C & 0xFFFFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapTransformD(int32_t D)
*  \brief       APIs related to graphics engine
*  \param       int32_t D
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapTransformD(int32_t D){
	return ( WriteCmd((24UL << 24) | ((D & 0x1FFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapTransformE(int32_t E)
*  \brief       APIs related to graphics engine
*  \param       int32_t E
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapTransformE(int32_t E){
    return ( WriteCmd((25UL << 24) | ((E & 0x1FFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BitmapTransformF(int32_t F)
*  \brief       APIs related to graphics engine
*  \param       int32_t F
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BitmapTransformF(int32_t F){
    return ( WriteCmd((26UL << 24) | ((F & 0xFFFFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::BlendFunc(uint8_t Src, uint8_t Dst)
*  \brief       APIs related to graphics engine
*  \param       uint8_t Src
*  \param       uint8_t Dst
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::BlendFunc(uint8_t Src, uint8_t Dst){
    return ( WriteCmd((11UL << 24) | ((Src & 7L) << 3) | ((Dst & 7L) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Call(uint16_t Dest)
*  \brief       APIs related to graphics engine
*  \param       uint16_t Dest
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Call(uint16_t Dest){
    return ( WriteCmd((29UL << 24) | ((Dest & 0xFFFFL) << 0)) );
}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Cell(uint8_t Cell)
*  \brief       APIs related to graphics engine
*  \param       uint8_t Cell
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Cell(uint8_t Cell){
    return ( WriteCmd((6UL << 24) | ((Cell & 0x7FL) << 0)) );
}
FT_GEStatus FT800_IMP::ClearColorA(uint8_t Alpha) 
{
    return ( WriteCmd((15UL << 24) | ((Alpha & 0xFFL) << 0)) );
}
FT_GEStatus FT800_IMP::ClearColorRGB(uint8_t red, uint8_t green, uint8_t blue) 
{
    return ( WriteCmd((2UL << 24) | ((red & 0xFFL) << 16) | ((green & 0xFFL) << 8) | ((blue & 0xFFL) << 0)) );
}
FT_GEStatus FT800_IMP::ClearColorRGB(uint32_t rgb) 
{
    return ( WriteCmd((2UL << 24) | (rgb & 0xFFFFFFL)) );
}    
FT_GEStatus FT800_IMP::Clear(uint8_t c, uint8_t s, uint8_t t) 
{
    uint8_t m = (c << 2) | (s << 1) | t;
    return ( WriteCmd((38UL << 24) | m) );
}    
FT_GEStatus FT800_IMP::Clear(void) 
{
    return ( WriteCmd((38UL << 24) | 7) );
}    
FT_GEStatus FT800_IMP::ClearStencil(uint8_t s) 
{
    return ( WriteCmd((17UL << 24) | ((s & 0xFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::ClearTag(uint8_t s) 
{
    return ( WriteCmd((18UL << 24) | ((s & 0xFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::ColorA(uint8_t Alpha) 
{
    return ( WriteCmd((16UL << 24) | ((Alpha & 0xFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::ColorMask(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
{
    return ( WriteCmd((32UL << 24) | ((r & 1L) << 3) | ((g & 1L) << 2) | ((b & 1L) << 1) | ((a & 1L) << 0)) );
}    
FT_GEStatus FT800_IMP::ColorRGB(uint8_t red, uint8_t green, uint8_t blue) {
    // WriteCmd((4UL << 24) | ((red & 0xFFL) << 16) | ((green & 0xFFL) << 8) | ((blue & 0xFFL) << 0)) );
    union {
        uint32_t UUInt32;
         uint8_t A[4];
    };
    A[0] = blue;
    A[1] = green;
    A[2] = red;
    A[3] = 4;
    return ( WriteCmd(UUInt32) );
}    
FT_GEStatus FT800_IMP::Display(void) 
{
    return ( WriteCmd((0UL << 24)) );
}    
FT_GEStatus FT800_IMP::End(void) {
    return ( WriteCmd((33UL << 24)) );
}    
FT_GEStatus FT800_IMP::Jump(uint16_t Dest) 
{
    return ( WriteCmd((30UL << 24) | ((Dest & 0x7FFL) << 0)) );
}    
FT_GEStatus FT800_IMP::LineWidth(uint16_t Width) 
{
    return ( WriteCmd((14UL << 24) | ((Width & 0xFFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::Macro(uint8_t m) 
{
    return ( WriteCmd((37UL << 24) | ((m & 1L) << 0)) );
}   
FT_GEStatus FT800_IMP::PointSize(uint16_t Size) 
{
    return ( WriteCmd((13UL << 24) | ((Size & 0x1FFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::RestoreContext(void) 
{
    return ( WriteCmd((35UL << 24)) );
}    
FT_GEStatus FT800_IMP::Return(void) 
{
    return ( WriteCmd((36UL << 24)) );
}    
FT_GEStatus FT800_IMP::SaveContext(void) 
{
    return ( WriteCmd((34UL << 24)) );
}    
FT_GEStatus FT800_IMP::ScissorSize(uint16_t Width, uint16_t Height) 
{
    return ( WriteCmd((28UL << 24) | ((Width & 0x3FFL) << 10) | ((Height & 0x3FFL) << 0)) );
}    
FT_GEStatus FT800_IMP::ScissorXY(uint16_t x, uint16_t y)
{
    return ( WriteCmd((27UL << 24) | ((x & 0x1FFL) << 9) | ((y & 0x1FFL) << 0)) );
}    
FT_GEStatus FT800_IMP::StencilFunc(uint8_t Func, uint8_t Ref, uint8_t Mask) 
{
    return ( WriteCmd((10UL << 24) | ((Func & 7L) << 16) | ((Ref & 0xFFL) << 8) | ((Mask & 0xFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::StencilMask(uint8_t Mask) 
{
    return ( WriteCmd((19UL << 24) | ((Mask & 0xFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::StencilOp(uint8_t Sfail, uint8_t Spass) 
{
    return ( WriteCmd((12UL << 24) | ((Sfail & 7L) << 3) | ((Spass & 7L) << 0)) );
}   
FT_GEStatus FT800_IMP::TagMask(uint8_t Mask) 
{
    return ( WriteCmd((20UL << 24) | ((Mask & 1L) << 0)) );
}    
FT_GEStatus FT800_IMP::Tag(uint8_t s) 
{
    return ( WriteCmd((3UL << 24) | ((s & 0xFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::Vertex2f(int16_t x, int16_t y) 
{
    // x = int(16 * x);
    // y = int(16 * y);
    return ( WriteCmd((1UL << 30) | ((x & 0x7FFFL) << 15) | ((y & 0x7FFFL) << 0)) );
}    
FT_GEStatus FT800_IMP::Vertex2ii(uint16_t x, uint16_t y, uint8_t Handle, uint8_t Cell) 
{
    //return (WriteCmd((2UL << 30) | ((x & 0x1FFL) << 21) | ((y & 0x1FFL) << 12) | ((handle & 0x1FL) << 7) | ((cell & 0x7FL) << 0)) ));
    /* Generates invalid graphics instruction due to shift operation of -ve coefficient */
    x &= 0x1FFL;//error handling of -ve coefficients
    y &= 0x1FFL;
    union {
        uint32_t UUInt32;
        uint8_t A[4];
    };
    A[0] = Cell | ((Handle & 1) << 7);
    A[1] = (Handle >> 1) | (y << 4);
    A[2] = (y >> 4) | (x << 5);
    A[3] = (2 << 6) | (x >> 3);
    return ( WriteCmd(UUInt32) );
}  

/* graphics helper apis */
FT_GEStatus FT800_IMP::ColorRGB(uint32_t rgb) 
{
    return ( WriteCmd((4UL << 24) | (rgb & 0xFFFFFFL)) );
}    
/* Form two commands, one for rgb and the other for a */
FT_GEStatus FT800_IMP::ColorARGB(uint32_t argb) 
{
	  FT_GEStatus Status;
	  WriteCmd((4UL << 24) | (argb & 0xFFFFFFL));
	  Status = WriteCmd((16UL << 24) | ((argb>>24) & 0xFFL) );
  
  return Status;
} 

FT_GEStatus FT800_IMP::Cmd_Logo(void)
{
	FT_GEStatus Status;	
	Status = WriteCmd(CMD_LOGO);		
	return Status;
}    
FT_GEStatus FT800_IMP::Cmd_Append(uint32_t Ptr, uint32_t Num)
{
	FT_GEStatus Status;
	WriteCmd(CMD_APPEND);
	WriteCmd(Ptr);
	Status = WriteCmd(Num);//checking only for the last command
	
	return Status;
}    
FT_GEStatus FT800_IMP::Cmd_BGColor(uint32_t c)
{
	FT_GEStatus Status;
	WriteCmd(CMD_BGCOLOR);
	Status = WriteCmd(c);
	
	return Status;
}    

/* make sure the string is in ram instead of program memory. it is assumed that *s is valid string and if null then it should contain \0 */
FT_GEStatus FT800_IMP::Cmd_Button(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t Font, uint16_t Options, const char *s)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_BUTTON);
	WriteCmd(((y & 0xFFFFL) <<16) | (x & 0xFFFFL));
	WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
	WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
	Status = WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);//make sure last byte is added into the
	
	return Status;
}    
FT_GEStatus FT800_IMP::Cmd_Calibrate(uint32_t Result)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_CALIBRATE);
	Status = WriteCmd(Result);//write extra word for result
	
	return Status;
}    
FT_GEStatus FT800_IMP::Cmd_Clock(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_CLOCK);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((Options & 0xFFFFL)<<16) | (r & 0xFFFFL));
	WriteCmd(((m & 0xFFFFL)<<16) | (h & 0xFFFFL));
	Status = WriteCmd(((ms & 0xFFFFL)<<16) | (s & 0xFFFFL));
	
	return Status;
}    
FT_GEStatus FT800_IMP::Cmd_ColdStart(void)
{
	//should we even change command read and write pointers
	return ( WriteCmd(CMD_COLDSTART) );
}   
FT_GEStatus FT800_IMP::Cmd_Dial(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t Val)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_DIAL);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((Options & 0xFFFFL)<<16) | (r & 0xFFFFL));
	Status = WriteCmd(Val);
	
	return Status;
}    
  
FT_GEStatus FT800_IMP::Cmd_FGColor(uint32_t c)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_FGCOLOR);
	Status = WriteCmd(c);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Gauge(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t Major, uint16_t Minor, uint16_t Val, uint16_t Range)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_GAUGE);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((Options & 0xFFFFL)<<16) | (r & 0xFFFFL));
	WriteCmd(((Minor & 0xFFFFL)<<16) | (Major & 0xFFFFL));
	Status = WriteCmd(((Range & 0xFFFFL)<<16) | (Val & 0xFFFFL));
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_GetMatrix(void)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_GETMATRIX);
	WriteCmd(0);
	WriteCmd(0);
	WriteCmd(0);
	WriteCmd(0);
	WriteCmd(0);
	Status = WriteCmd(0);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_GetProps(uint32_t &Ptr, uint32_t &w, uint32_t &h)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_GETPROPS);
	WriteCmd(0);
	WriteCmd(0);
	Status = WriteCmd(0);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_GetPtr(uint32_t Result)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_GETPTR);
	Status = WriteCmd(Result);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_GradColor(uint32_t c)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_GRADCOLOR);
	Status = WriteCmd(c);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_GRADIENT);
	WriteCmd(((y0 & 0xFFFFL)<<16)|(x0 & 0xFFFFL));
	WriteCmd(rgb0);
	WriteCmd(((y1 & 0xFFFFL)<<16)|(x1 & 0xFFFFL));
	Status = WriteCmd(rgb1);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Inflate(uint32_t Ptr)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_INFLATE);
	Status = WriteCmd(Ptr);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Interrupt(uint32_t ms)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_INTERRUPT);
	Status = WriteCmd(ms);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Keys(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t Font, uint16_t Options, const char *s)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_KEYS);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
	WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
	Status = WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);	
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_LoadIdentity(void)
{
	return ( WriteCmd(CMD_LOADIDENTITY) );
}    
FT_GEStatus FT800_IMP::Cmd_LoadImage(uint32_t Ptr, int32_t Options)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_LOADIMAGE);
	WriteCmd(Ptr);
	Status = WriteCmd(Options);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Memcpy(uint32_t Dest, uint32_t Src, uint32_t Num)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_MEMCPY);
	WriteCmd(Dest);
	WriteCmd(Src);
	Status = WriteCmd(Num);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Memset(uint32_t Ptr, uint8_t Value, uint32_t Num)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_MEMSET);
	WriteCmd(Ptr);
	WriteCmd(Value);	
	Status = WriteCmd(Num);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Memcrc(uint32_t Ptr, uint32_t Num,uint32_t &Result)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_MEMCRC);
	WriteCmd(Ptr);
	WriteCmd(Num);
	Status = WriteCmd(Result);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Memwrite(uint32_t Ptr, uint32_t Num)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_MEMWRITE);
	WriteCmd(Ptr);
	Status = WriteCmd(Num);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Memzero(uint32_t Ptr, uint32_t Num)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_MEMZERO);
	WriteCmd(Ptr);
	Status = WriteCmd(Num);
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Number(int16_t x, int16_t y, uint8_t Font, uint16_t Options, uint32_t n)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_NUMBER);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
	Status = WriteCmd(n);
	
	return Status;	
}   
FT_GEStatus FT800_IMP::Cmd_Progress(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t Options, uint16_t Val, uint16_t Range)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_PROGRESS);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
	WriteCmd(((Val & 0xFFFFL)<<16) | (Options & 0xFFFFL));
	Status = WriteCmd(Range);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_RegRead(uint32_t Ptr,uint32_t Result)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_REGREAD);
	WriteCmd(Ptr);
	Status = WriteCmd(Result);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Rotate(int32_t a)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_ROTATE);
	Status = WriteCmd(a);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Scale(int32_t sx, int32_t sy)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_SCALE);
	WriteCmd(sx);
	Status = WriteCmd(sy);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_ScreenSaver(void)
{
	return ( WriteCmd(CMD_SCREENSAVER) );
}    
FT_GEStatus FT800_IMP::Cmd_Scrollbar(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t Options, uint16_t Val, uint16_t Size, uint16_t Range)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_SCROLLBAR);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
	WriteCmd(((Val & 0xFFFFL)<<16) | (Options & 0xFFFFL));
	Status = WriteCmd(((Range & 0xFFFFL)<<16) | (Size & 0xFFFFL));
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_SetFont(uint8_t Font, uint32_t Ptr)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_SETFONT);
	WriteCmd(Font);
	Status = WriteCmd(Ptr);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_SetMatrix(void)
{
	return ( WriteCmd(CMD_SETMATRIX) );	
}    
FT_GEStatus FT800_IMP::Cmd_Sketch(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t Ptr, uint16_t Format)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_SKETCH);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
	WriteCmd(Ptr);
	Status = WriteCmd(Format);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Slider(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t Options, uint16_t Val, uint16_t Range)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_SLIDER);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
	WriteCmd(((Val & 0xFFFFL)<<16) | (Options & 0xFFFFL));
	Status = WriteCmd(Range);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Snapshot(uint32_t OutputAddr)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_SNAPSHOT);
	Status = WriteCmd(OutputAddr);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Spinner(int16_t x, int16_t y, uint8_t Style, uint8_t Scale)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_SPINNER);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	Status = WriteCmd(((Scale & 0xFFFFL)<<16) | (Style & 0xFFFFL));
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Stop(void)
{
	return ( WriteCmd(CMD_STOP) );
}    
FT_GEStatus FT800_IMP::Cmd_Swap(void)
{
	return ( WriteCmd(CMD_SWAP) );
}    
FT_GEStatus FT800_IMP::Cmd_Text(int16_t x, int16_t y, uint8_t Font, uint16_t Options, const char *s)
{
	FT_GEStatus Status;
	WriteCmd(CMD_TEXT);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((Options & 0xFFFFL)<<16) | (Font & 0xFFFFL));
	Status = WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Toggle(int16_t x, int16_t y, int16_t w, uint8_t Font, uint16_t Options, uint16_t State, const char *s)
{
	FT_GEStatus Status;
	WriteCmd(CMD_TOGGLE);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((Font & 0xFFFFL)<<16) | (w & 0xFFFFL));
	WriteCmd(((State & 0xFFFFL)<<16) | (Options & 0xFFFFL));
	Status = WriteCmd((uint8_t *)s,strlen((const char *)s) + 1);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Track(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t Tag)
{
	FT_GEStatus Status;
	WriteCmd(CMD_TRACK);
	WriteCmd(((y & 0xFFFFL)<<16) | (x & 0xFFFFL));
	WriteCmd(((h & 0xFFFFL)<<16) | (w & 0xFFFFL));
	Status = WriteCmd(Tag);
	
	return Status;	
}    
FT_GEStatus FT800_IMP::Cmd_Translate(int32_t tx, int32_t ty)
{
	FT_GEStatus Status;
	
	WriteCmd(CMD_TRANSLATE);
	WriteCmd(tx);
	Status = WriteCmd(ty);
	
	return Status;	
}      


/* Apis related to audio engine */
FT_AEStatus FT800_IMP::PlaySound(uint8_t Volume,uint16_t SoundNote)
{
	ft800HAL.ft800memWrite8(REG_VOL_SOUND,Volume);//change the volume of synthesized sound, 0 means off, 255 means max on
	ft800HAL.ft800memWrite16(REG_SOUND,SoundNote);
	ft800HAL.ft800memWrite8(REG_PLAY,FT_SOUND_PLAY);
	
	return FT_AE_OK;
}    
//higher byte is the note and lower byte is the sound
FT_AEStatus FT800_IMP::PlaySound(uint16_t SoundNote)
{
	ft800HAL.ft800memWrite16(REG_SOUND,SoundNote);
	ft800HAL.ft800memWrite8(REG_PLAY,FT_SOUND_PLAY);
	return FT_AE_OK;
}    

//volume will not be modified
void FT800_IMP::StopSound(void)
{
	ft800HAL.ft800memWrite16(REG_SOUND,FT_SILENCE);//configure silence
	ft800HAL.ft800memWrite8(REG_PLAY,FT_SOUND_PLAY);//play the silence
}    

void FT800_IMP::SetSoundVolume(uint8_t Volume)
{
	ft800HAL.ft800memWrite8(REG_VOL_SOUND,Volume);
}    

uint8_t FT800_IMP::GetSoundVolume(void)
{
	return ft800HAL.ft800memRead8(REG_VOL_SOUND);
}    

//one shot or continuous, sampling frequency is from 8k to 48k
FT_AEStatus FT800_IMP::PlayAudio(uint8_t Volume,uint8_t Format,uint16_t SamplingFreq,uint32_t BufferAddr,uint32_t BufferSize,uint8_t Loop)
{
	if((SamplingFreq*1L < FT_AUDIO_SAMPLINGFREQ_MIN*1L) | (SamplingFreq*1L > FT_AUDIO_SAMPLINGFREQ_MAX*1L))
	{
		return FT_AE_ERROR_SAMPLINGFREQ_OUTOFRANGE;
	}
	if(Format > FT_ADPCM_SAMPLES)
	{
		return FT_AE_ERROR_FORMAT;
	}
	ft800HAL.ft800memWrite8(REG_VOL_PB,Volume);
	ft800HAL.ft800memWrite32(REG_PLAYBACK_START,BufferAddr);
	ft800HAL.ft800memWrite32(REG_PLAYBACK_LENGTH,BufferSize);
	ft800HAL.ft800memWrite32(REG_PLAYBACK_FREQ,SamplingFreq);
	ft800HAL.ft800memWrite8(REG_PLAYBACK_FORMAT,Format);
	ft800HAL.ft800memWrite8(REG_PLAYBACK_LOOP,Loop);//0 means one shot and 1 means loop
	
	ft800HAL.ft800memWrite8(REG_PLAYBACK_PLAY,FT_AUDIO_PLAY);

	return FT_AE_OK;
}    

void FT800_IMP::SetAudioVolume(uint8_t Volume)
{
	ft800HAL.ft800memWrite8(REG_VOL_PB,Volume);
}    

//returns playback stopped or continue
FT_AEStatus FT800_IMP::GetAudioStats(uint32_t &CurrPlayAddr)
{
	CurrPlayAddr = ft800HAL.ft800memRead32(REG_PLAYBACK_READPTR);
	
	//in case of loop, check for the playback status
	if(0 == ft800HAL.ft800memRead8(REG_PLAYBACK_PLAY))
	{
		return FT_AE_PLAYBACK_STOPPED;
	}

	return FT_AE_PLAYBACK_CONTINUE;
}    

uint8_t FT800_IMP::GetAudioVolume(void)
{
	return  (ft800HAL.ft800memRead8(REG_VOL_PB));
}    

//volume will not be modified
void FT800_IMP::StopAudio(void)
{
	//configure audio with length o and play
	ft800HAL.ft800memWrite32(REG_PLAYBACK_LENGTH,0);
	ft800HAL.ft800memWrite8(REG_PLAYBACK_LOOP,0);//0 means one shot and 1 means loop
	
	ft800HAL.ft800memWrite8(REG_PLAYBACK_PLAY,FT_AUDIO_PLAY);	
}    

/***********************************************************************
*! \fn          void FT800_IMP::SetTouchMode(uint8_t TMode)
*  \brief       one of 0ff/oneshot/frame/continuous. default being continuous
*  \param       uint8_t TMode
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::SetTouchMode(uint8_t TMode){
   ft800HAL.ft800memWrite8(REG_TOUCH_MODE,TMode);
}
/***********************************************************************
*! \fn          void FT800_IMP::SetHostTagXY(uint16_t xoffset,uint16_t yoffset){
*  \brief       api to set coordinates for host specific tag query
*  \param       uint16_t xoffset
*  \param       uint16_t yoffset
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::SetHostTagXY(uint16_t xoffset,uint16_t yoffset){

    uint8_t A[6];
	//little endian specific
	A[0] = xoffset & 0xFF;
	A[1] = xoffset >> 8;
	A[4] = yoffset & 0xFF;
	A[5] = yoffset >> 8;
	ft800HAL.ft800memWrite8Array(REG_TAG_X,A,6);

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Call()
*  \brief       api to get TAG from FT_GC for coordinates set by  SetHostTagXY() api - host needs to wait for at least 1 frame to get these query values
*  \param       none
*  \exception   none
*  \return      uint8_t HostTag
***********************************************************************/
uint8_t FT800_IMP::GetHostTagXY(){
	return ( ft800HAL.ft800memRead8(REG_TAG) );
}
/***********************************************************************
*! \fn          void FT800_IMP::GetTagXY(sTagXY &sTagxy)
*  \brief       get the touched object tag and repective xy coordinates
*  \param       sTagXY &sTagxy
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::GetTagXY(sTagXY &sTagxy){
    ft800HAL.ft800memRead8Array(REG_TOUCH_TAG_XY,(uint8_t *)&sTagxy,6);
}
/***********************************************************************
*! \fn          void FT800_IMP::GetTrackTag(sTrackTag &sTracktag)
*  \brief       get the track value and the tag value
*  \param       sTrackTag &sTracktag
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::GetTrackTag(sTrackTag &sTracktag){

    uint32_t *ptr = (uint32_t *)&sTracktag;
	*ptr = ft800HAL.ft800memRead32(REG_TRACKER);

}

/***********************************************************************
*! \fn          void FT800_IMP::DLEnd()
*  \brief       inserts display() gpu instruction at the end and inserts cmd_swap() command
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::DLEnd(){

	Display();
	Cmd_Swap();

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::CheckLogo()
*  \brief       special api to check logo completion
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::CheckLogo(){

    uint16_t ReadCmdPtr = ft800HAL.ft800memRead16(REG_CMD_READ) ;
	if((ReadCmdPtr == ft800HAL.ft800memRead16(REG_CMD_WRITE)) && (ReadCmdPtr == 0))
	{
		CmdFifoWp = 0;
		return FT_GE_FINISHED;
	}
	else if(FT_COPRO_ERROR == ReadCmdPtr)
	{
		return FT_GE_ERROR;
	}
	else
	{
		return FT_GE_BUSY;
	}

}

//apis to render all the commands to hardware
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Flush()
*  \brief       api to flush out all the commands to FT_GC, does not wait for the completion of the rendering
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Flush(){

    if(TrnsFlag)
	{
		EndTransferCmd();
	}	
	ft800HAL.ft800memWrite16(REG_CMD_WRITE,CmdFifoWp);
	
	return FT_GE_OK;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Finish()
*  \brief       flushes out all the commands to FT_GC and waits for the completion of execution
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Finish(){

    uint16_t ReadPrt;
	
	if(TrnsFlag)
	{
		EndTransferCmd();
	}
	ft800HAL.ft800memWrite16(REG_CMD_WRITE,CmdFifoWp);
	
	while((ReadPrt = ft800HAL.ft800memRead16(REG_CMD_READ)) != CmdFifoWp)
	{
		if(FT_COPRO_ERROR == ReadPrt)
		{
			return FT_GE_ERROR;
		}
	}	
	return FT_GE_OK;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::CheckFinish()
*  \brief       checks fifo and returns the status
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::CheckFinish(){
    
    uint16_t ReadPrt = ft800HAL.ft800memRead16(REG_CMD_READ);
	

	if (FT_COPRO_ERROR == ReadPrt)
	{
		return FT_GE_ERROR;
	}
	else if(ReadPrt != CmdFifoWp)
	{
		return FT_GE_BUSY;
	}	//success case return finished
	return FT_GE_FINISHED;

}	
/***********************************************************************
*! \fn          uint32_t FT800_IMP::GetError()
*  \brief       error from graphics controller library
*  \param       none
*  \exception   none
*  \return      uint32_t ErrorCode
***********************************************************************/
uint32_t FT800_IMP::GetError(){

    if(FT_COPRO_ERROR == ft800HAL.ft800memRead16(REG_CMD_READ))
	{
		return FT_GE_ERROR;
	}
	return FT_GE_OK;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::TransferCmd(uint32_t Cmd)
*  \brief       Transfer CMD 
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::TransferCmd(uint32_t Cmd){

	if(FreeSpace <4)
	{
		//blocking call till freespace is available
		if(FT_GE_ERROR == ChkGetFreeSpace(4))
		{
			return FT_GE_ERROR;
		}
	}
	ft800HAL.Transfer32(Cmd);
	CmdFifoWp = (CmdFifoWp + 4)&0xfff;
	FreeSpace -= 4;
	
	return FT_GE_OK;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::TransferCmd(uint8_t *Src,uint32_t NBytes)
*  \brief       Transfer CMD
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::TransferCmd(uint8_t *Src,uint32_t NBytes){
	
    uint32_t i,Count;
	//align the NBytes to multiple of 4
	NBytes = (NBytes + 3)&(~3);
	//transfer the whole buffer into command buffer
	while(NBytes)
	{
		Count = NBytes;
		if(Count > FreeSpace)
		{
			//first update the free space
			UpdateFreeSpace();
			//then transfer the data
			Count = MIN(FreeSpace,Count);
			for(i = 0;i<Count;i++)	
				ft800HAL.Transfer8(*Src++);
			CmdFifoWp = (CmdFifoWp + Count)&0xfff;
			FreeSpace -= Count;
			//get the free space
			NBytes -= Count;
			Count = MIN(NBytes,FT_CMDFIFO_SIZE/2);//atleast wait for half the buffer completion
			if(FT_GE_ERROR == ChkGetFreeSpace(Count))
			{
				return FT_GE_ERROR;
			}
		}
		else
		{
			//transfer of data to command buffer
			for(i = 0;i<Count;i++)	
				ft800HAL.Transfer8(*Src++);
			CmdFifoWp = (CmdFifoWp + Count)&0xfff;
			FreeSpace -= Count;
			NBytes -= Count;
		}		
	}
	return FT_GE_OK;

}	
/***********************************************************************
*! \fn          void FT800_IMP::EndTransferCmd()
*  \brief       end the command transfer
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::EndTransferCmd(){
    
	ft800HAL.EndTransfer();
	//update the write pointer of fifo
	ft800HAL.ft800memWrite32(REG_CMD_WRITE,CmdFifoWp);
	TrnsFlag = 0;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Cmd_GetResult(uint32_t &Result)
*  \brief       reads the result of the previous commands such as 
*				cmd_memcrc,cmd_calibration, cmd_regread which has 
*				return values. if busy returns busy status
*  \param       uint32_t &Result
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Cmd_GetResult(uint32_t &Result){

    ft800HAL.ft800memRead32(FT_RAM_CMD + ((CmdFifoWp - 4)&0xFFC));//make sure no other commands are issued after cmd_memcrc/cmd_calibration/cmd_regread
	
	return FT_GE_OK;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::Cmd_GetResults(int8_t *pA,uint16_t NBytes)
*  \brief       api to read n bytes from the current write pointer location
*				make sure no other commands are issued after 
*				cmd_memcrc/cmd_calibration/cmd_regread
*				assumed that number of bytes in array is allocated/managed 
*				by application
*  \param       int8_t *pA
*  \param       uint16_t NBytes
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::Cmd_GetResults(int8_t *pA,uint16_t NBytes){

    //handling of circular buffer
	if((CmdFifoWp - NBytes) < 0)
	{
		uint16_t ReadLen = NBytes - CmdFifoWp;
		ft800HAL.ft800memRead8Array((FT_RAM_CMD + (FT_CMDFIFO_SIZE - ReadLen)),(uint8_t *)pA,ReadLen);//first read the end bytes
		pA += ReadLen;
		ReadLen = NBytes - ReadLen;
		ft800HAL.ft800memRead8Array(FT_RAM_CMD,(uint8_t *)pA,ReadLen);//later read the starting bytes
	}
	else
	{
		ft800HAL.ft800memRead8Array(FT_RAM_CMD + (CmdFifoWp - NBytes),(uint8_t *)pA,NBytes);
	}
	
	return FT_GE_OK;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::UpdateFreeSpace()
*  \brief       check free space
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::UpdateFreeSpace(){

    if(TrnsFlag)
	{
		EndTransferCmd();
		//update the write pointer
		ft800HAL.ft800memWrite16(REG_CMD_WRITE,CmdFifoWp);
		StartTransferCmd();
	}
	else
	{
		//update the write pointer
		ft800HAL.ft800memWrite16(REG_CMD_WRITE,CmdFifoWp);
	}
	return FT_GE_OK;

}
/***********************************************************************
*! \fn          FT_GEStatus FT800_IMP::ChkGetFreeSpace(uint16_t NBytes)
*  \brief       API related to coprocessor fifo write command
*  \param       none
*  \exception   none
*  \return      FT_GEStatus
***********************************************************************/
FT_GEStatus FT800_IMP::ChkGetFreeSpace(uint16_t NBytes){

    //return busy if no space
	if(FreeSpace < NBytes)
	{
		if(TrnsFlag)
		{
			EndTransferCmd();
			TrnsFlag = 1;//coz EndTransferCmd will make it 0
		}
		//update the write pointer
		ft800HAL.ft800memWrite16(REG_CMD_WRITE,CmdFifoWp);
		while(FreeSpace < NBytes)
		{
			uint16_t rdptr = ft800HAL.ft800memRead16(REG_CMD_READ);
			if(rdptr == FT_COPRO_ERROR)
			{
				return FT_GE_ERROR;
			}
			//update the freespace by reading the register
			FreeSpace = ((CmdFifoWp - rdptr)&0xffc);
			FreeSpace = (FT_CMDFIFO_SIZE - 4) - FreeSpace;
		}		
		if(TrnsFlag)
		{
			StartTransferCmd();
		}
	}
	return FT_GE_OK;

}

/***********************************************************************
*! \fn          void FT800_IMP::FT_Touch_Disable()
*  \brief       disable TouchScreen
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::FT_Touch_Disable(){

	ft800HAL.ft800memWrite32(REG_TOUCH_MODE, 0);
	ft800HAL.ft800memWrite32(REG_TOUCH_RZTHRESH, 0);   

}
/***********************************************************************
*! \fn          void FT800_IMP::FT_Touch_Enable()
*  \brief       enable Touchscreen
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void FT800_IMP::FT_Touch_Enable(){

	ft800HAL.ft800memWrite32(REG_TOUCH_MODE, TOUCHMODE_FRAME);
	ft800HAL.ft800memWrite32(REG_TOUCH_RZTHRESH, 1200);  

}
