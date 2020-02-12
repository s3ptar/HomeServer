/**************************************************************************
 *
 *                  FTDIChip AN_275 FT800 with Arduino - Version 1.0
 *
 **************************************************************************
 * FileName:        FT800.h
 * Purpose:         This header file contains the display list and graphics 
 *                  processor commands for the FTDI FT800 graphics controller chip
 * Dependencies:    See INCLUDES section below
 * Processor:       Atmel Atmega328 @ 16MHz
 * IDE:             Arduino v1.0.5 or later
 * Company:         Future Technology Devices International Ltd.
 *
 * Software License Agreement
 *
 * Software License Agreement
 *
 * This code is provided as an example only and is not guaranteed by FTDI. 
 * FTDI accept no responsibility for any issues resulting from its use. 
 * The developer of the final application incorporating any parts of this 
 * sample project is responsible for ensuring its safe and correct operation 
 * and for any conequences resulting from its use.
 *
 * Author	Date		Rev		Comment
 **************************************************************************
 * BR		2013-11-01	1.0		Initial version
 **************************************************************************

*/

#ifndef FT800_h
#define FT800_h
   
/*****************************************************************************/
/************************* I N C L U D E S ***********************************/
/*****************************************************************************/
#include <Arduino.h>
#include "FT800_HAL.h"
#include "FT800_Config.h"
/*****************************************************************************/
/********************** D E C L A R A T I O N S ******************************/
/*****************************************************************************/

#define FT_DL_SIZE            (8*1024)  //8KB Display List buffer size
#define FT_CMD_FIFO_SIZE      (4*1024)  //4KB coprocessor Fifo size
#define FT_CMD_SIZE           (4)       //4 byte per coprocessor command of EVE

#define FT800_VERSION         "1.9.0"

// Refer to the FT800 Datasheet

// FT800 Memory Map Addresses
#define RAM_CMD               0x108000UL
#define RAM_DL                0x100000UL
#define RAM_G                 0x000000UL
#define RAM_PAL               0x102000UL
#define RAM_REG               0x102400UL

// FT800 Register Addresses
#define REG_CLOCK             0x102408UL
#define REG_CMD_DL            0x1024ecUL
#define REG_CMD_READ          0x1024e4UL
#define REG_CMD_WRITE         0x1024e8UL
#define REG_CPURESET          0x10241cUL
#define REG_CSPREAD           0x102464UL
#define REG_DITHER            0x10245cUL
#define REG_DLSWAP            0x102450UL
#define REG_FRAMES            0x102404UL
#define REG_FREQUENCY         0x10240cUL
#define REG_GPIO              0x102490UL
#define REG_GPIO_DIR          0x10248cUL
#define REG_HCYCLE            0x102428UL
#define REG_HOFFSET           0x10242cUL
#define REG_HSIZE             0x102430UL
#define REG_HSYNC0            0x102434UL
#define REG_HSYNC1            0x102438UL
#define REG_ID                0x102400UL
#define REG_INT_EN            0x10249cUL
#define REG_INT_FLAGS         0x102498UL
#define REG_INT_MASK          0x1024a0UL
#define REG_MACRO_0           0x1024c8UL
#define REG_MACRO_1           0x1024ccUL
#define REG_OUTBITS           0x102458UL
#define REG_PCLK              0x10246cUL
#define REG_PCLK_POL          0x102468UL
#define REG_PLAY              0x102488UL
#define REG_PLAYBACK_FORMAT   0x1024b4UL
#define REG_PLAYBACK_FREQ     0x1024b0UL
#define REG_PLAYBACK_LENGTH   0x1024a8UL
#define REG_PLAYBACK_LOOP     0x1024b8UL
#define REG_PLAYBACK_PLAY     0x1024bcUL
#define REG_PLAYBACK_READPTR  0x1024acUL
#define REG_PLAYBACK_START    0x1024a4UL
#define REG_PWM_DUTY          0x1024c4UL
#define REG_PWM_HZ            0x1024c0UL
#define REG_RENDERMODE        0x102410UL
#define REG_ROTATE            0x102454UL
#define REG_SNAPSHOT          0x102418UL
#define REG_SNAPY             0x102414UL
#define REG_SOUND             0x102484UL
#define REG_SWIZZLE           0x102460UL
#define REG_TAG               0x102478UL
#define REG_TAG_X             0x102470UL
#define REG_TAG_Y             0x102474UL
#define REG_TAP_CRC           0x102420UL
#define REG_TAP_MASK          0x102424UL
#define REG_TOUCH_ADC_MODE    0x1024f4UL
#define REG_TOUCH_CHARGE      0x1024f8UL
#define REG_TOUCH_DIRECT_XY   0x102574UL
#define REG_TOUCH_DIRECT_Z1Z2 0x102578UL
#define REG_TOUCH_MODE        0x1024f0UL
#define REG_TOUCH_OVERSAMPLE  0x102500UL
#define REG_TOUCH_RAW_XY      0x102508UL
#define REG_TOUCH_RZ          0x10250cUL
#define REG_TOUCH_RZTHRESH    0x102504UL
#define REG_TOUCH_SCREEN_XY   0x102510UL
#define REG_TOUCH_SETTLE      0x1024fcUL
#define REG_TOUCH_TAG         0x102518UL
#define REG_TOUCH_TAG_XY      0x102514UL
#define REG_TOUCH_TRANSFORM_A 0x10251cUL
#define REG_TOUCH_TRANSFORM_B 0x102520UL
#define REG_TOUCH_TRANSFORM_C 0x102524UL
#define REG_TOUCH_TRANSFORM_D 0x102528UL
#define REG_TOUCH_TRANSFORM_E 0x10252cUL
#define REG_TOUCH_TRANSFORM_F 0x102530UL
#define REG_TRACKER           0x109000UL
#define REG_VCYCLE            0x10243cUL
#define REG_VOFFSET           0x102440UL
#define REG_VOL_PB            0x10247cUL
#define REG_VOL_SOUND         0x102480UL
#define REG_VSIZE             0x102444UL
#define REG_VSYNC0            0x102448UL
#define REG_VSYNC1            0x10244cUL

/* Macros related to inbuilt font */
#define FT_NUMCHAR_PERFONT 		(128L)		//number of font characters per bitmap handle
#define FT_FONT_TABLE_SIZE 		(148L)		//size of the font table - utilized for loopup by the graphics engine
#define FT_FONT_TABLE_POINTER	(0xFFFFCUL)	//pointer to the inbuilt font tables starting from bitmap handle 16

/* Memory definitions */		
#define FT_RAM_G						0x000000UL
#define FT_ROM_CHIPID					0x0C0000UL	
#define FT_ROM_FONT						0x0BB23CUL
#define FT_ROM_FONT_ADDR				0x0FFFFCUL		
#define FT_RAM_DL						0x100000UL
#define FT_RAM_PAL						0x102000UL
#define FT_RAM_CMD						0x108000UL
#define FT_RAM_SCREENSHOT				0x1C2000UL		

/* Memory buffer sizes */
#define FT_RAM_G_SIZE					256*1024L
#define FT_CMDFIFO_SIZE					4*1024L
#define FT_RAM_DL_SIZE					8*1024L
#define FT_RAM_PAL_SIZE					1*1024L

/* Graphics display list swap macros */
#define FT_DLSWAP_DONE          0
#define FT_DLSWAP_LINE          1
#define FT_DLSWAP_FRAME         2

/* Host command macros */
#define FT_ACTIVE				0x00			// Place FT800 in active state
#define FT_STANDBY				0x41			// Place FT800 in Standby (clk running)
#define FT_SLEEP				0x42			// Place FT800 in Sleep (clk off)
#define FT_PWRDOWN				0x50			// Place FT800 in Power Down (core off)
#define FT_CLKEXT				0x44			// Select external clock source
#define FT_CLKINT				0x48			// Select internal clock source
#define FT_CLK48M				0x62			// Select 48MHz PLL output
#define FT_CLK36M				0x61			// Select 36MHz PLL output
#define FT_CORERST				0x68			// Reset core - all registers default and processors reset


// Graphics Engine Commands
// Refer to the FT800 Programmers Guide
#define CMDBUF_SIZE          4096UL
#define CMD_APPEND           0xffffff1eUL
#define CMD_BGCOLOR          0xffffff09UL
#define CMD_BUTTON           0xffffff0dUL
#define CMD_CALIBRATE        0xffffff15UL
#define CMD_CLOCK            0xffffff14UL
#define CMD_COLDSTART        0xffffff32UL
#define CMD_DIAL             0xffffff2dUL
#define CMD_DLSTART          0xffffff00UL
#define CMD_FGCOLOR          0xffffff0aUL
#define CMD_GAUGE            0xffffff13UL
#define CMD_GETMATRIX        0xffffff33UL
#define CMD_GETPTR           0xffffff23UL
#define CMD_GRADCOLOR        0xffffff34UL
#define CMD_GRADIENT         0xffffff0bUL
#define CMD_INFLATE          0xffffff22UL
#define CMD_INTERRUPT        0xffffff02UL
#define CMD_KEYS             0xffffff0eUL
#define CMD_LOADIDENTITY     0xffffff26UL
#define CMD_LOADIMAGE        0xffffff24UL
#define CMD_LOGO             0xffffff31UL
#define CMD_MEMCPY           0xffffff1dUL
#define CMD_MEMCRC           0xffffff18UL
#define CMD_MEMSET           0xffffff1bUL
#define CMD_MEMWRITE         0xffffff1aUL
#define CMD_MEMZERO          0xffffff1cUL
#define CMD_NUMBER           0xffffff2eUL
#define CMD_PROGRESS         0xffffff0fUL
#define CMD_REGREAD          0xffffff19UL
#define CMD_ROTATE           0xffffff29UL
#define CMD_SCALE            0xffffff28UL
#define CMD_SCREENSAVER      0xffffff2fUL
#define CMD_SCROLLBAR        0xffffff11UL
#define CMD_SETFONT          0xffffff2bUL
#define CMD_SETMATRIX        0xffffff2aUL
#define CMD_SKETCH           0xffffff30UL
#define CMD_SLIDER           0xffffff10UL
#define CMD_SNAPSHOT         0xffffff1fUL
#define CMD_SPINNER          0xffffff16UL
#define CMD_STOP             0xffffff17UL
#define CMD_SWAP             0xffffff01UL
#define CMD_TEXT             0xffffff0cUL
#define CMD_TOGGLE           0xffffff12UL
#define CMD_TRACK            0xffffff2cUL
#define CMD_TRANSLATE        0xffffff27UL
#define CMD_GETPROPS         0xFFFFFF25UL

// Display list commands to be embedded in Graphics Processor
#define DL_ALPHA_FUNC       0x09000000UL // requires OR'd arguments
#define DL_BITMAP_HANDLE    0x05000000UL // requires OR'd arguments
#define DL_BITMAP_LAYOUT    0x07000000UL // requires OR'd arguments
#define DL_BITMAP_SIZE      0x08000000UL // requires OR'd arguments
#define DL_BITMAP_SOURCE    0x01000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_A   0x15000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_B   0x16000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_C   0x17000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_D   0x18000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_E   0x19000000UL // requires OR'd arguments
#define DL_BITMAP_TFORM_F   0x1A000000UL // requires OR'd arguments
#define DL_BLEND_FUNC       0x0B000000UL // requires OR'd arguments
#define DL_BEGIN            0x1F000000UL // requires OR'd arguments
#define DL_CALL             0x1D000000UL // requires OR'd arguments
#define DL_CLEAR            0x26000000UL // requires OR'd arguments
#define DL_CELL             0x06000000UL // requires OR'd arguments
#define DL_CLEAR_RGB        0x02000000UL // requires OR'd arguments
#define DL_CLEAR_STENCIL    0x11000000UL // requires OR'd arguments
#define DL_CLEAR_TAG        0x12000000UL // requires OR'd arguments
#define DL_COLOR_A          0x0F000000UL // requires OR'd arguments
#define DL_COLOR_MASK       0x20000000UL // requires OR'd arguments
#define DL_COLOR_RGB        0x04000000UL // requires OR'd arguments
#define DL_DISPLAY          0x00000000UL
#define DL_END              0x21000000UL
#define DL_JUMP             0x1E000000UL // requires OR'd arguments
#define DL_LINE_WIDTH       0x0E000000UL // requires OR'd arguments
#define DL_MACRO            0x25000000UL // requires OR'd arguments
#define DL_POINT_SIZE       0x0D000000UL // requires OR'd arguments
#define DL_RESTORE_CONTEXT  0x23000000UL
#define DL_RETURN           0x24000000UL
#define DL_SAVE_CONTEXT     0x22000000UL
#define DL_SCISSOR_SIZE     0x1C000000UL // requires OR'd arguments
#define DL_SCISSOR_XY       0x1B000000UL // requires OR'd arguments
#define DL_STENCIL_FUNC     0x0A000000UL // requires OR'd arguments
#define DL_STENCIL_MASK     0x13000000UL // requires OR'd arguments
#define DL_STENCIL_OP       0x0C000000UL // requires OR'd arguments
#define DL_TAG              0x03000000UL // requires OR'd arguments
#define DL_TAG_MASK         0x14000000UL // requires OR'd arguments
#define DL_VERTEX2F         0x40000000UL // requires OR'd arguments
#define DL_VERTEX2II        0x02000000UL // requires OR'd arguments

// Command and register value options
#define CLR_COL              0x4
#define CLR_STN              0x2
#define CLR_TAG              0x1
#define DECR                 4UL
#define DECR_WRAP            7UL
#define DLSWAP_DONE          0UL
#define DLSWAP_FRAME         2UL
#define DLSWAP_LINE          1UL
#define DST_ALPHA            3UL
#define EDGE_STRIP_A         7UL
#define EDGE_STRIP_B         8UL
#define EDGE_STRIP_L         6UL
#define EDGE_STRIP_R         5UL
#define EQUAL                5UL
#define GEQUAL               4UL
#define GREATER              3UL
#define INCR                 3UL
#define INCR_WRAP            6UL
#define INT_CMDEMPTY         32UL
#define INT_CMDFLAG          64UL
#define INT_CONVCOMPLETE     128UL
#define INT_PLAYBACK         16UL
#define INT_SOUND            8UL
#define INT_SWAP             1UL
#define INT_TAG              4UL
#define INT_TOUCH            2UL
#define INVERT               5UL
#define KEEP                 1UL
#define L1                   1UL
#define L4                   2UL
#define L8                   3UL
#define LEQUAL               2UL
#define LESS                 1UL
#define LINEAR_SAMPLES       0UL
#define LINES                3UL
#define LINE_STRIP           4UL
#define NEAREST              0UL
#define NEVER                0UL
#define NOTEQUAL             6UL
#define ONE                  1UL
#define ONE_MINUS_DST_ALPHA  5UL
#define ONE_MINUS_SRC_ALPHA  4UL
#define OPT_CENTER           1536UL    // 0x6000
#define OPT_CENTERX          512UL     // 0x0200
#define OPT_CENTERY          1024UL    // 0x0400
#define OPT_FLAT             256UL     // 0x0100
#define OPT_MONO             1UL
#define OPT_NOBACK           4096UL    // 0x1000
#define OPT_NODL             2UL
#define OPT_NOHANDS          49152UL   // 0xC168
#define OPT_NOHM             16384UL   // 0x4000
#define OPT_NOPOINTER        16384UL   // 0x4000
#define OPT_NOSECS           32768UL   // 0x8000
#define OPT_NOTICKS          8192UL    // 0x2000
#define OPT_RIGHTX           2048UL    // 0x0800
#define OPT_SIGNED           256UL     // 0x0100
#define PALETTED             8UL
#define PLAYCOLOR            0x00a0a080
#define FTPOINTS             2UL       // "POINTS" is a reserved word
#define RECTS                9UL
#define REPEAT               1UL
#define REPLACE              2UL
#define RGB332               4UL
#define RGB565               7UL
#define SRC_ALPHA            2UL
#define TEXT8X8              9UL
#define TEXTVGA              10UL
#define TOUCHMODE_CONTINUOUS 3UL
#define TOUCHMODE_FRAME      2UL
#define TOUCHMODE_OFF        0UL
#define TOUCHMODE_ONESHOT    1UL
#define ULAW_SAMPLES         1UL
#define ZERO                 0UL



/***********************************************************************
 * Macros
 **********************************************************************/
#define RGB(r, g, b)         ((((r) << 16) | (g) << 8) | (b))
#define SQ(v)                ((v) * (v))
#define MIN(x,y)             ((x) > (y) ? (y) : (x))
#define MAX(x,y)             ((x) > (y) ? (x) : (y))
#define NOTE(n, sharp)       (((n) - 'C') + ((sharp) * 128))
#define F16(s)               (((s) * 65536))
#define INVALID_TOUCH_XY     0x8000
#define ABS(x)               ((x) > (0) ? (x) : (-x))


// FT800 Chip Commands - use with cmdWrite
#define FT800_ACTIVE	0x00			// Initializes FT800
#define FT800_STANDBY	0x41			// Place FT800 in Standby (clk running)
#define FT800_SLEEP	    0x42			// Place FT800 in Sleep (clk off)
#define FT800_PWRDOWN	0x50			// Place FT800 in Power Down (core off)
#define FT800_CLKEXT	0x44			// Select external clock source
#define FT800_CLK48M	0x62			// Select 48MHz PLL
#define FT800_CLK36M	0x61			// Select 36MHz PLL
#define FT800_CORERST	0x68			// Reset core - all registers default

/* Widget command macros */
#define FT_OPT_MONO             1
#define FT_OPT_NODL             2
#define FT_OPT_FLAT             256
#define FT_OPT_CENTERX          512
#define FT_OPT_CENTERY          1024
#define FT_OPT_CENTER           (FT_OPT_CENTERX | FT_OPT_CENTERY)
#define FT_OPT_NOBACK           4096
#define FT_OPT_NOTICKS          8192
#define FT_OPT_NOHM             16384
#define FT_OPT_NOPOINTER        16384
#define FT_OPT_NOSECS           32768
#define FT_OPT_NOHANDS          49152
#define FT_OPT_RIGHTX           2048
#define FT_OPT_SIGNED           256

/* Coprocessor reset related macros */
#define FT_RESET_HOLD_COPROCESSOR		1
#define FT_RESET_RELEASE_COPROCESSOR	0

/* Macros for sound play and stop */
#define FT_SOUND_PLAY					1
#define FT_AUDIO_PLAY					1
/* Audio sample type macros */
#define FT_LINEAR_SAMPLES       0	//8bit signed samples
#define FT_ULAW_SAMPLES         1	//8bit ulaw samples
#define FT_ADPCM_SAMPLES        2	//4bit ima adpcm samples

/* Synthesized sound macros */
#define FT_SILENCE              0x00

#define FT_SQUAREWAVE           0x01
#define FT_SINEWAVE             0x02
#define FT_SAWTOOTH             0x03
#define FT_TRIANGLE             0x04

#define FT_BEEPING              0x05
#define FT_ALARM                0x06
#define FT_WARBLE               0x07
#define FT_CAROUSEL             0x08

#define FT_PIPS(n)              (0x0F + (n))

#define FT_HARP                 0x40
#define FT_XYLOPHONE            0x41
#define FT_TUBA                 0x42
#define FT_GLOCKENSPIEL         0x43
#define FT_ORGAN                0x44
#define FT_TRUMPET              0x45
#define FT_PIANO                0x46
#define FT_CHIMES               0x47
#define FT_MUSICBOX             0x48
#define FT_BELL                 0x49

#define FT_CLICK                0x50
#define FT_SWITCH               0x51
#define FT_COWBELL              0x52
#define FT_NOTCH                0x53
#define FT_HIHAT                0x54
#define FT_KICKDRUM             0x55
#define FT_POP                  0x56
#define FT_CLACK                0x57
#define FT_CHACK                0x58

#define FT_MUTE                 0x60
#define FT_UNMUTE               0x61

/* Synthesized sound frequencies, midi note macros */
#define FT_MIDI_A0            	21
#define FT_MIDI_A_0           	22
#define FT_MIDI_B0            	23
#define FT_MIDI_C1            	24
#define FT_MIDI_C_1           	25
#define FT_MIDI_D1            	26
#define FT_MIDI_D_1           	27
#define FT_MIDI_E1            	28
#define FT_MIDI_F1            	29
#define FT_MIDI_F_1           	30
#define FT_MIDI_G1            	31
#define FT_MIDI_G_1           	32
#define FT_MIDI_A1            	33
#define FT_MIDI_A_1           	34
#define FT_MIDI_B1            	35
#define FT_MIDI_C2            	36
#define FT_MIDI_C_2           	37
#define FT_MIDI_D2            	38
#define FT_MIDI_D_2           	39
#define FT_MIDI_E2            	40
#define FT_MIDI_F2            	41
#define FT_MIDI_F_2           	42
#define FT_MIDI_G2            	43
#define FT_MIDI_G_2           	44
#define FT_MIDI_A2            	45
#define FT_MIDI_A_2           	46
#define FT_MIDI_B2            	47
#define FT_MIDI_C3            	48
#define FT_MIDI_C_3           	49
#define FT_MIDI_D3            	50
#define FT_MIDI_D_3           	51
#define FT_MIDI_E3            	52
#define FT_MIDI_F3            	53
#define FT_MIDI_F_3           	54
#define FT_MIDI_G3            	55
#define FT_MIDI_G_3           	56
#define FT_MIDI_A3            	57
#define FT_MIDI_A_3           	58
#define FT_MIDI_B3            	59
#define FT_MIDI_C4            	60
#define FT_MIDI_C_4           	61
#define FT_MIDI_D4            	62
#define FT_MIDI_D_4           	63
#define FT_MIDI_E4            	64
#define FT_MIDI_F4            	65
#define FT_MIDI_F_4           	66
#define FT_MIDI_G4            	67
#define FT_MIDI_G_4           	68
#define FT_MIDI_A4            	69
#define FT_MIDI_A_4           	70
#define FT_MIDI_B4            	71
#define FT_MIDI_C5            	72
#define FT_MIDI_C_5           	73
#define FT_MIDI_D5            	74
#define FT_MIDI_D_5           	75
#define FT_MIDI_E5            	76
#define FT_MIDI_F5            	77
#define FT_MIDI_F_5           	78
#define FT_MIDI_G5            	79
#define FT_MIDI_G_5           	80
#define FT_MIDI_A5            	81
#define FT_MIDI_A_5           	82
#define FT_MIDI_B5            	83
#define FT_MIDI_C6            	84
#define FT_MIDI_C_6           	85
#define FT_MIDI_D6            	86
#define FT_MIDI_D_6           	87
#define FT_MIDI_E6            	88
#define FT_MIDI_F6            	89
#define FT_MIDI_F_6           	90
#define FT_MIDI_G6            	91
#define FT_MIDI_G_6           	92
#define FT_MIDI_A6            	93
#define FT_MIDI_A_6           	94
#define FT_MIDI_B6            	95
#define FT_MIDI_C7            	96
#define FT_MIDI_C_7           	97
#define FT_MIDI_D7            	98
#define FT_MIDI_D_7           	99
#define FT_MIDI_E7            	100
#define FT_MIDI_F7            	101
#define FT_MIDI_F_7           	102
#define FT_MIDI_G7            	103
#define FT_MIDI_G_7           	104
#define FT_MIDI_A7            	105
#define FT_MIDI_A_7           	106
#define FT_MIDI_B7            	107
#define FT_MIDI_C8            	108

/* Macros for audio playback parameters*/
#define FT_AUDIO_SAMPLINGFREQ_MIN		8*1000L
#define FT_AUDIO_SAMPLINGFREQ_MAX		48*1000L

//coprocessor error macros
#define FT_COPRO_ERROR					0xfffUL

// Useful Macros
#define RGB(r, g, b)         ((((r) << 16) | (g) << 8) | (b))
#define SQ(v)                ((v) * (v))
#define MIN(x,y)             ((x) > (y) ? (y) : (x))
#define MAX(x,y)             ((x) > (y) ? (x) : (y))
#define NOTE(n, sharp)       (((n) - 'C') + ((sharp) * 128))
#define F16(s)               (((s) * 65536))
#define INVALID_TOUCH_XY     0x8000
#define ABS(x)               ((x) > (0) ? (x) : (-x))

#endif  //FT800_h
/** EOF FT800.h ********************************************************/
