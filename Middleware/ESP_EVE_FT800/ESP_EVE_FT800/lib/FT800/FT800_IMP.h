/***********************************************************************
*! \file:                   FT800_IMP.h
*  \projekt:                FT800_ESP
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   /*****************************************************************************
*                            Copyright (c) Future Technology Devices International 2014
*                            propriety of Future Technology devices International.
*
*                           Software License Agreement
*
*                           This code is provided as an example only and is not guaranteed by FTDI. 
*                           FTDI accept no responsibility for any issues resulting from its use. 
*                           The developer of the final application incorporating any parts of this 
*                           sample project is responsible for ensuring its safe and correct operation 
*                           and for any consequences resulting from its use.
***********************************************************************/
 
#ifndef _FT800_IMP_H_
#define _FT800_IMP_H_
 
/***********************************************************************
 * Includes
 **********************************************************************/
 #include "FT800.h"
 #include "FT800_Config.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 /************************************************************************************************************************************************************
Display parameters used for various options are

FT800_DisplayResolution			Width 	Height	Swizzle	Polarity	PClk	HCycle	Hoffset		Hsync0		Hsync1		VCycle	Voffset		Vsync0	Vsync1	
FT_DISPLAY_QVGA_320x240   		320		240		3		0			8		408		70			0			10			263			13		0		2
FT_DISPLAY_WQVGA_480x272		480		272		0		1			5		548		43			0			41			292			12		0		10
*************************************************************************************************************************************************************/
/***********************************************************************
 * Declarations
 **********************************************************************/
 /* Macros for FT800Impl library version */
#define FT800IMPL_MAJOR 				(1)
#define FT800IMPL_MINOR 				(0)
#define FT800IMPL_BUILD 				(0)
/* Definitions used for debug. Uncomment the below to enable debug from graphics controller library */
#define FT_GCDEBUG0 255 //switchoff debug
#define FT_GCDEBUG1 1 //most critical debug information
#define FT_GCDEBUG2 2 //mid critical debug information
#define FT_GCDEBUG3 3 //least critical debug information
/* Change the below statement wrt debug criticality */
#define FT_GCDEBUG FT_GCDEBUG0

/* Version number of FT_GC */
#define FT_GC_MAJOR				1
#define FT_GC_MINOR				2
#define FT_GC_BUILD				0

/* FT_GC status enum - used for api return type, error type etc */
typedef enum FT_Status
{	
	/* Common enums */
	FT_OK = 0,
	FT_ERROR = 1,
	FT_WARNING = 2,
	FT_ERROR_INIT = 3,
	FT_ERROR_CHIPID = 4,
	
	/* Library related enums */
	FT_ERROR_NOPINASSIGNED = 50,
}FT_Status;

/* Audio coprocessor related enums */
typedef enum FT_AEStatus
{		
	FT_AE_OK = 0,	
	FT_AE_ERROR_FORMAT = 1,
	FT_AE_ERROR_SAMPLINGFREQ_OUTOFRANGE = 2,	//assert for boundary
	FT_AE_PLAYBACK_STOPPED = 3,
	FT_AE_PLAYBACK_CONTINUE = 4,
}FT_AEStatus;

/* Status enums for graphics engine */
typedef enum FT_GEStatus
{	
	FT_GE_OK = 0,	
	FT_GE_BUSY = 1,
	FT_GE_FINISHED = 2,
	
	/* Graphics related error enums */
	FT_GE_ERROR_INVALID_PRIMITIVE = 20,
	FT_GE_ERROR_INVALID_BITMAP_FORMAT = 21,
	FT_GE_ERROR_INVALID_BITMAP_HANDLENUM = 22,
	FT_GE_ERROR_VERTEX_OUTOFRANGE = 23,

	/* Coprocessor related enums */
	FT_GE_ERROR = 50,						//undefined error
	FT_GE_ERROR_JPEG = 51,					//erranious jpeg data
	FT_GE_ERROR_DEFLATE = 52,				//erranious deflated data
	FT_GE_ERROR_DISPLAYLIST_OVERFLOW = 53,	//DL buffer overflow
	FT_GE_ERROR_INVALID_WIDGET_PARAMS = 54,	//invalid input parameters - out of bound
	
	/* Display parameters error */
	FT_GE_ERROR_DISPLAYPARAMS = 100,//error in the display parameters
}FT_GEStatus;

/* Touch coprocessor related enums */
typedef enum FT_TEStatus
{	
	FT_TE_OK = 0,	
	FT_TE_ERROR_RZTHRESHOLD = 1,		//threshold out of bound
	FT_TE_ERROR_FILTERPARAM = 2,		//filter out of bound
	FT_TE_ERROR_MODE = 3,				//mode out of range
	FT_TE_ERROR_INVALIDPARAM = 4,		//generic invalid param
}FT_TEStatus;

/* FT_GC options */
typedef enum FT_GCOptions{
	FT_INTERNAL_CLOCK_SOURCE = 1,
}FT_GCOptions;
/*
typedef enum FT_GC_DispRes
{
	FT_DISPLAY_QVGA_320x240 = 0,
	FT_DISPLAY_WQVGA_480x272 = 1,
}FT_GC_DispRes;
*/
#define		FT_DISPLAY_QVGA_320x240		0UL
#define		FT_DISPLAY_WQVGA_480x272 	1UL
/* Structure definitions */

typedef struct sTagXY
{
	int16_t y;		//y coordinate of touch object
	int16_t x;		//x coordinate of touch object
	uint16_t tag;	//TAG value of the object
}sTagXY;

typedef struct sTrackTag
{
	uint16_t tag;	//TAG value of the object
	uint16_t track;	//track value of the object	
}sTrackTag;

/* FT80x font table structure */
/* Font table address in ROM can be found by reading 32bit value from FT_FONT_TABLE_POINTER location. */
/* 16 font tables are present at the address read from location FT_FONT_TABLE_POINTER */
typedef struct FT_Fonts
{
	/* All the values are in bytes */
	/* Width of each character font from 0 to 127 */
	uint8_t	FontWidth[FT_NUMCHAR_PERFONT];
	/* Bitmap format of font wrt bitmap formats supported by FT800 - L1, L4, L8 */
	uint32_t	FontBitmapFormat;
	/* Font line stride in FT800 ROM */
	uint32_t	FontLineStride;
	/* Font width in pixels */
	uint32_t	FontWidthInPixels;
	/* Font height in pixels */
	uint32_t	FontHeightInPixels;
	/* Pointer to font graphics raw data */
	uint32_t	PointerToFontGraphicsData;
}FT_Fonts_t;
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
 
 
class FT800_IMP
{
 private:
     /* data */
    int8_t ft800pwrPin;
    unsigned long ramDisplayList = RAM_DL;		// Set beginning of display list memory 
    unsigned long ramCommandBuffer = RAM_CMD;	// Set beginning of graphics command memory
    uint8_t  GInit,PDNPin,TrnsFlag;//Global flag to indicate that initialization is done

    unsigned int cmdBufferRd = 0x0000;		// Used to navigate command ring buffer
    unsigned int cmdBufferWr = 0x0000;		// Used to navigate command ring buffer
    unsigned int cmdOffset = 0x0000;		// Used to navigate command rung buffer
    unsigned int point_size = 0x0100;		// Define a default dot size
    unsigned long point_x = (96 * 16);		// Define a default point x-location (1/16 anti-aliased)
    unsigned long point_y = (136 * 16);		// Define a default point y-location (1/16 anti-aliased)
    unsigned long color;				// Variable for chanign colors
    unsigned char ft800Gpio;			// Used for FT800 GPIO register

    unsigned int incCMDOffset(unsigned int currentOffset, unsigned char commandSize);
    /* APIS related to power modes */
	void HostCommand(uint32_t HostCommand);	
    
 public:
    //Constructor
    FT800_IMP(int8_t sck, int8_t miso, int8_t mosi, int8_t ss, int8_t pwrdn, int8_t irqpin);
    //Destructor
    ~FT800_IMP();
    void FT800_Init();
    void FT800_setup();
    void FT800_test();

    //FT_Status Init(uint8_t ResType,uint16_t options1);//API to set the resolution of output display	
	//FT_Status Init(uint16_t hperiod,uint16_t vperiod,uint16_t hfrontporch,uint16_t hbackporch,uint16_t hpulsewidth,
	//			   uint16_t vfrontporch,uint16_t vbackporch,uint16_t vpulsewidth,
    //				   uint8_t polarity,uint8_t swizzle,uint8_t fps, uint16_t options1);/* Api to program the output display wrt display parameters */
	FT_Status Exit();//exit state of graphics processor
	void GetVersion(uint8_t &Major, uint8_t &Minor, uint8_t &Build);/* Api to get version of the library */
	void ResetCopro(void);//api to reset only coprocessor	
	void Reset(void);//api to reset whole FT_GC via pdn - if pdn is not assigned then reset is not successful
	/* Apis related to power up/power down funcationality */
	void DisplayConfigExternalClock(uint8_t ResType);
	void ActiveInternalClock(void);
	void PDN_Cycle(void);
	/* Apis related to graphics processor */	
	//enable or disable interrupts
	void EnableInterrupts(uint8_t GEnable,uint8_t Mask);
	uint8_t ReadIntReg(void);//read the interrupt flag register - note that on FT_GC the interrupts are clear by read
	//void AssignCBFunPtr(void *CBFunPtr);//api to register callback function from application using interrupt mechanism	
    void CalibrateTouchPanel(void);
	
	/* APIs related to graphics engine */
	FT_GEStatus AlphaFunc(uint8_t Func, uint8_t Ref);
	FT_GEStatus Begin(uint8_t Prim);
	FT_GEStatus BitmapHandle(uint8_t Handle);
	FT_GEStatus BitmapLayout(uint8_t Format, uint16_t Linestride, uint16_t Height);
	FT_GEStatus BitmapSize(uint8_t Filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height);
	FT_GEStatus BitmapSource(uint32_t Addr);
	FT_GEStatus BitmapTransformA(int32_t A);
	FT_GEStatus BitmapTransformB(int32_t B);
	FT_GEStatus BitmapTransformC(int32_t C);
	FT_GEStatus BitmapTransformD(int32_t D);
	FT_GEStatus BitmapTransformE(int32_t E);
	FT_GEStatus BitmapTransformF(int32_t F);
	FT_GEStatus BlendFunc(uint8_t Src, uint8_t Dst);
	FT_GEStatus Call(uint16_t Dest);
	FT_GEStatus Cell(uint8_t Cell);
	FT_GEStatus ClearColorA(uint8_t Alpha);
	FT_GEStatus ClearColorRGB(uint8_t red, uint8_t green, uint8_t blue);
	FT_GEStatus ClearColorRGB(uint32_t rgb);
	FT_GEStatus Clear(uint8_t c, uint8_t s, uint8_t t);
	FT_GEStatus Clear(void);
	FT_GEStatus ClearStencil(uint8_t s);
	FT_GEStatus ClearTag(uint8_t s);
	FT_GEStatus ColorA(uint8_t Alpha);
	FT_GEStatus ColorMask(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	FT_GEStatus ColorRGB(uint8_t red, uint8_t green, uint8_t blue);
	FT_GEStatus Display(void);
	FT_GEStatus End(void);
	FT_GEStatus Jump(uint16_t Dest);
	FT_GEStatus LineWidth(uint16_t Width);
	FT_GEStatus Macro(uint8_t m);
	FT_GEStatus PointSize(uint16_t Size);
	FT_GEStatus RestoreContext(void);
	FT_GEStatus Return(void);
	FT_GEStatus SaveContext(void);
	FT_GEStatus ScissorSize(uint16_t width, uint16_t height);
	FT_GEStatus ScissorXY(uint16_t x, uint16_t y);
	FT_GEStatus StencilFunc(uint8_t Func, uint8_t Ref, uint8_t Mask);
	FT_GEStatus StencilMask(uint8_t Mask);
	FT_GEStatus StencilOp(uint8_t Sfail, uint8_t Spass);
	FT_GEStatus TagMask(uint8_t Mask);
	FT_GEStatus Tag(uint8_t s);
	FT_GEStatus Vertex2f(int16_t x, int16_t y);
	FT_GEStatus Vertex2ii(uint16_t x, uint16_t y, uint8_t Handle = 0, uint8_t Cell = 0);

	/* graphics helper apis */
	FT_GEStatus ColorRGB(uint32_t rgb);
	FT_GEStatus ColorARGB(uint32_t argb);

	/* APIs related to coprocessor commands, widgets etc */
	FT_GEStatus Cmd_Logo(void);
	FT_GEStatus Cmd_Append(uint32_t Ptr, uint32_t Num);
	FT_GEStatus Cmd_BGColor(uint32_t c);
	FT_GEStatus Cmd_Button(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t Font, uint16_t Options, const char *s);
	FT_GEStatus Cmd_Calibrate(uint32_t Result);
	FT_GEStatus Cmd_Clock(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms);
	FT_GEStatus Cmd_ColdStart(void);
	FT_GEStatus Cmd_Dial(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t Val);
	FT_GEStatus Cmd_DLStart(void);
	FT_GEStatus Cmd_FGColor(uint32_t c);
	FT_GEStatus Cmd_Gauge(int16_t x, int16_t y, int16_t r, uint16_t Options, uint16_t Major, uint16_t Minor, uint16_t Val, uint16_t Range);
	FT_GEStatus Cmd_GetMatrix(void);
	FT_GEStatus Cmd_GetProps(uint32_t &Ptr, uint32_t &w, uint32_t &h);
	FT_GEStatus Cmd_GetPtr(uint32_t Result);
	FT_GEStatus Cmd_GradColor(uint32_t c);
	FT_GEStatus Cmd_Gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1);
	FT_GEStatus Cmd_Inflate(uint32_t Ptr);
	FT_GEStatus Cmd_Interrupt(uint32_t ms);
	FT_GEStatus Cmd_Keys(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t Font, uint16_t Options, const char *s);
	FT_GEStatus Cmd_LoadIdentity(void);
	FT_GEStatus Cmd_LoadImage(uint32_t Ptr, int32_t Options);
	FT_GEStatus Cmd_Memcpy(uint32_t Dest, uint32_t Src, uint32_t Num);
	FT_GEStatus Cmd_Memset(uint32_t Ptr, uint8_t Value, uint32_t Num);
	FT_GEStatus Cmd_Memcrc(uint32_t Ptr, uint32_t Num,uint32_t &Result);
	FT_GEStatus Cmd_Memwrite(uint32_t Ptr, uint32_t Num);
	FT_GEStatus Cmd_Memzero(uint32_t Ptr, uint32_t Num);
	FT_GEStatus Cmd_Number(int16_t x, int16_t y, uint8_t Font, uint16_t Options, uint32_t n);
	FT_GEStatus Cmd_Progress(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t Options, uint16_t Val, uint16_t Range);
	FT_GEStatus Cmd_RegRead(uint32_t Ptr,uint32_t Result);
	FT_GEStatus Cmd_Rotate(int32_t a);
	FT_GEStatus Cmd_Scale(int32_t sx, int32_t sy);
	FT_GEStatus Cmd_ScreenSaver(void);
	FT_GEStatus Cmd_Scrollbar(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t Options, uint16_t Val, uint16_t Size, uint16_t Range);
	FT_GEStatus Cmd_SetFont(uint8_t Font, uint32_t Ptr);
	FT_GEStatus Cmd_SetMatrix(void);
	FT_GEStatus Cmd_Sketch(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t Ptr, uint16_t Format);
	FT_GEStatus Cmd_Slider(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t Options, uint16_t val, uint16_t Range);
	FT_GEStatus Cmd_Snapshot(uint32_t OutputAddr);
	FT_GEStatus Cmd_Spinner(int16_t x, int16_t y, uint8_t Style, uint8_t Scale);
	FT_GEStatus Cmd_Stop(void);
	FT_GEStatus Cmd_Swap(void);
	FT_GEStatus Cmd_Text(int16_t x, int16_t y, uint8_t font, uint16_t Options, const char *s);
	FT_GEStatus Cmd_Toggle(int16_t x, int16_t y, int16_t w, uint8_t font, uint16_t Options, uint16_t State, const char *s);
	FT_GEStatus Cmd_Track(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t Tag);
	FT_GEStatus Cmd_Translate(int32_t tx, int32_t ty);  

	/* Apis related to audio engine */
	FT_AEStatus PlaySound(uint8_t Volume,uint16_t SoundNote);
	FT_AEStatus PlaySound(uint16_t SoundNote);//higher byte is the note and lower byte is the sound
	void StopSound(void);//volume will not be modified
	void SetSoundVolume(uint8_t Volume);
	uint8_t GetSoundVolume(void);

	FT_AEStatus PlayAudio(uint8_t Volume,uint8_t Format,uint16_t SamplingFreq,uint32_t BufferAddr,uint32_t BufferSize,uint8_t Loop);//one shot or continuous, sampling frequency is from 8k to 48k
	void SetAudioVolume(uint8_t Volume);
	FT_AEStatus GetAudioStats(uint32_t &CurrPlayAddr);//returns playback stopped or continue
	uint8_t GetAudioVolume(void);
	void StopAudio(void);//volume will not be modified

	/* Apis related to touch engine */
	void SetTouchMode(uint8_t TMode);//one of 0ff/oneshot/frame/continuous. default being continuous
	void SetHostTagXY(uint16_t xoffset,uint16_t yoffset);//api to set coordinates for host specific tag query
	uint8_t GetHostTagXY(void);//api to get TAG from FT_GC for coordinates set by  SetHostTagXY() api - host needs to wait for at least 1 frame to get these query values
	void GetTagXY(sTagXY &sTagxy);//get the touched object tag and repective xy coordinates
	void GetTrackTag(sTrackTag &sTracktag);//get the track value and the tag value
	
	
	//void CheckIntFlag(uint8_t IntFlag);	

	/* Special apis */
	//apis for ease of usage in FT_GC
	void DLStart(void);//inserts cmd_dlstart() followed by clear(1,1,1) graphics command
	void DLEnd(void);//inserts display() gpu instruction at the end and inserts cmd_swap() command
	FT_GEStatus CheckLogo(void);//special api to check logo completion

	//apis to render all the commands to hardware
	FT_GEStatus Flush(void);//api to flush out all the commands to FT_GC, does not wait for the completion of the rendering
	FT_GEStatus Finish(void);//flushes out all the commands to FT_GC and waits for the completion of execution
	FT_GEStatus CheckFinish(void);//checks fifo and returns the status	
	uint32_t GetError(void);//error from graphics controller library

	/* API related to coprocessor fifo buffer management */
	//please note that all the below apis are transfer commands
	FT_GEStatus WriteCmd(uint32_t Cmd);
	FT_GEStatus WriteCmd(uint8_t *Src,uint32_t NBytes);//api to send N bytes to command 	
	//FT_GEStatus WriteCmdfromflash(prog_uchar *Src,uint32_t NBytes);	
	//void StartTransferRCmd(uint32_t Addr);//assert CSpin and send read command
	FT_GEStatus TransferCmd(uint32_t Cmd);
	FT_GEStatus TransferCmd(uint8_t *Src,uint32_t NBytes);	
	FT_GEStatus Cmd_GetResult(uint32_t &Result);//reads the result of the previous commands such as cmd_memcrc,cmd_calibration, cmd_regread which has return values. if busy returns busy status
	FT_GEStatus Cmd_GetResults(int8_t *pA,uint16_t NBytes);//reads N bytes of result bytes from current write pointer
	FT_GEStatus UpdateFreeSpace();
	FT_GEStatus ChkGetFreeSpace(uint16_t NBytes);
	FT_GEStatus StartTransferCmd();
    void EndTransferCmd(void);
	
	
	/* Global variables */	
	
	uint16_t CmdFifoWp,FreeSpace;//command fifo write pointer

};

 extern FT800_IMP eve_display;
#endif /* _FT800_IMP_H_ */
