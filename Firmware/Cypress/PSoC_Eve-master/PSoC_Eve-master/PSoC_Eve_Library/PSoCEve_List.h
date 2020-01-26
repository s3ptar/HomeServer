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

#if !defined(PSOC_EVE_LIST_H)
#define PSOC_EVE_LIST_H
    
#include <cytypes.h>    
#include "PSoCEve_Config.h"   
    
#if defined EVE_FT800
    extern uint8 vertexFormat;
#endif    

/*******************************************************************************
*   EVE Memory Map Addresses
*******************************************************************************/  

#if defined EVE_FT800
    #define RAM_G               0x000000    // Main graphics RAM.
    #define ROM_CHIPID          0x0C0000    // Chip ID and revision.
    #define ROM_FONT            0x0BB23C    // Font table and bitmaps.
    #define ROM_FONT_ADDR       0x0FFFFC    // Font table pointer address,
    #define RAM_DL              0x100000    // Display list RAM.   
    #define RAM_PAL             0x102000    // Palette RAM.  
    #define RAM_REG             0x102400    // Registers.
    #define RAM_CMD             0x108000    // Coprocessor command buffer.  

    #define RAM_G_END_ADDRESS   0x03FFFF    // RAM_G last position address.    
#elif defined EVE_FT810    
    #define RAM_G               0x000000    // Main graphics RAM.
    #define ROM_CHIPID          0x0C0000    // Chip ID and revision.    
    #define ROM_FONT            0x1E0000    
    #define ROM_FONT_ADDR       0x2FFFFC    // Font table pointer address,
    #define RAM_DL              0x300000    // Display list RAM. 
    #define RAM_REG             0x302000    // Registers.
    #define RAM_CMD             0x308000    // Coprocessor command buffer.
    
    #define RAM_G_END_ADDRESS   0x0FFFFF    // RAM_G last position address. 
#endif    
    
/*******************************************************************************
*   EVE Registers Addresses
*******************************************************************************/     

#if defined EVE_FT800    
    #define REG_ID                  0x102400 
    #define REG_FRAMES              0x102404    
    #define REG_CLOCK               0x102408    
    #define REG_FREQUENCY           0x10240c
    #define REG_RENDERMODE          0x102410
    #define REG_SNAPY               0x102414    
    #define REG_SNAPSHOT            0x102418    
    #define REG_CPURESET            0x10241c    
    #define REG_TAP_CRC             0x102420
    #define REG_TAP_MASK            0x102424    
    #define REG_HCYCLE              0x102428
    #define REG_HOFFSET             0x10242c
    #define REG_HSIZE               0x102430
    #define REG_HSYNC0              0x102434
    #define REG_HSYNC1              0x102438 
    #define REG_VCYCLE              0x10243c 
    #define REG_VOFFSET             0x102440    
    #define REG_VSIZE               0x102444
    #define REG_VSYNC0              0x102448
    #define REG_VSYNC1              0x10244c     
    #define REG_DLSWAP              0x102450    
    #define REG_ROTATE              0x102454  
    #define REG_OUTBITS             0x102458    
    #define REG_DITHER              0x10245c
    #define REG_SWIZZLE             0x102460    
    #define REG_CSPREAD             0x102464
    #define REG_PCLK_POL            0x102468
    #define REG_PCLK                0x10246c
    #define REG_TAG_X               0x102470    
    #define REG_TAG_Y               0x102474
    #define REG_TAG                 0x102478
    #define REG_VOL_PB              0x10247c    
    #define REG_VOL_SOUND           0x102480    
    #define REG_SOUND               0x102484    
    #define REG_PLAY                0x102488    
    #define REG_GPIO_DIR            0x10248c
    #define REG_GPIO                0x102490  
    #define REG_INT_FLAGS           0x102498
    #define REG_INT_EN              0x10249c
    #define REG_INT_MASK            0x1024a0
    #define REG_PLAYBACK_START      0x1024a4    
    #define REG_PLAYBACK_LENGTH     0x1024a8   
    #define REG_PLAYBACK_READPTR    0x1024ac 
    #define REG_PLAYBACK_FREQ       0x1024b0
    #define REG_PLAYBACK_FORMAT     0x1024b4
    #define REG_PLAYBACK_LOOP       0x1024b8
    #define REG_PLAYBACK_PLAY       0x1024bc 
    #define REG_PWM_HZ              0x1024c0
    #define REG_PWM_DUTY            0x1024c4
    #define REG_MACRO_0             0x1024c8    
    #define REG_MACRO_1             0x1024cc
    #define REG_CMD_READ            0x1024e4
    #define REG_CMD_WRITE           0x1024e8
    #define REG_CMD_DL              0x1024ec     
    #define REG_TOUCH_MODE          0x1024f0    
    #define REG_TOUCH_ADC_MODE      0x1024f4
    #define REG_TOUCH_CHARGE        0x1024f8
    #define REG_TOUCH_SETTLE        0x1024fc    
    #define REG_TOUCH_OVERSAMPLE    0x102500
    #define REG_TOUCH_RZTHRESH      0x102504    
    #define REG_TOUCH_RAW_XY        0x102508
    #define REG_TOUCH_RZ            0x10250c
    #define REG_TOUCH_SCREEN_XY     0x102510    
    #define REG_TOUCH_TAG_XY        0x102514    
    #define REG_TOUCH_TAG           0x102518 
    #define REG_TOUCH_TRANSFORM_A   0x10251c    
    #define REG_TOUCH_TRANSFORM_B   0x102520
    #define REG_TOUCH_TRANSFORM_C   0x102524
    #define REG_TOUCH_TRANSFORM_D   0x102528 
    #define REG_TOUCH_TRANSFORM_E   0x10252c    
    #define REG_TOUCH_TRANSFORM_F   0x102530
    #define REG_TOUCH_DIRECT_XY     0x102574
    #define REG_TOUCH_DIRECT_Z1Z2   0x102578
    #define REG_TRACKER             0x109000
#elif defined EVE_FT810
    #define REG_ID                  0x302000
    #define REG_FRAMES              0x302004
    #define REG_CLOCK               0x302008
    #define REG_FREQUENCY           0x30200C
    #define REG_RENDERMODE          0x302010
    #define REG_SNAPY               0x302014
    #define REG_SNAPSHOT            0x302018
    #define REG_SNAPFORMAT          0x30201C
    #define REG_CPURESET            0x302020 
    #define REG_TAP_CRC             0x302024
    #define REG_TAP_MASK            0x302028
    #define REG_HCYCLE              0x30202C
    #define REG_HOFFSET             0x302030
    #define REG_HSIZE               0x302034
    #define REG_HSYNC0              0x302038
    #define REG_HSYNC1              0x30203C
    #define REG_VCYCLE              0x302040 
    #define REG_VOFFSET             0x302044   
    #define REG_VSIZE               0x302048
    #define REG_VSYNC0              0x30204C
    #define REG_VSYNC1              0x302050    
    #define REG_DLSWAP              0x302054   
    #define REG_ROTATE              0x302058 
    #define REG_OUTBITS             0x30205C  
    #define REG_DITHER              0x302060
    #define REG_SWIZZLE             0x302064  
    #define REG_CSPREAD             0x302068
    #define REG_PCLK_POL            0x30206C
    #define REG_PCLK                0x302070
    #define REG_TAG_X               0x302074  
    #define REG_TAG_Y               0x302078
    #define REG_TAG                 0x30207C
    #define REG_VOL_PB              0x302080 
    #define REG_VOL_SOUND           0x302084  
    #define REG_SOUND               0x302088  
    #define REG_PLAY                0x30208C  
    #define REG_GPIO_DIR            0x302090
    #define REG_GPIO                0x302094
    #define REG_GPIOX_DIR           0x302098
    #define REG_GPIOX               0x30209C
    #define REG_INT_FLAGS           0x3020A8
    #define REG_INT_EN              0x3020AC
    #define REG_INT_MASK            0x3020B0
    #define REG_PLAYBACK_START      0x3020B4   
    #define REG_PLAYBACK_LENGTH     0x3020B8 
    #define REG_PLAYBACK_READPTR    0x3020BC
    #define REG_PLAYBACK_FREQ       0x3020C0
    #define REG_PLAYBACK_FORMAT     0x3020C4
    #define REG_PLAYBACK_LOOP       0x3020C8
    #define REG_PLAYBACK_PLAY       0x3020CC
    #define REG_PWM_HZ              0x3020D0
    #define REG_PWM_DUTY            0x3020D4
    #define REG_MACRO_0             0x3020D8   
    #define REG_MACRO_1             0x3020DC
    #define REG_CMD_READ            0x3020F8
    #define REG_CMD_WRITE           0x3020FC
    #define REG_CMD_DL              0x302100   
    #define REG_TOUCH_MODE          0x302104  
    #define REG_TOUCH_ADC_MODE      0x302108
    #define REG_TOUCH_CHARGE        0x30210C
    #define REG_TOUCH_SETTLE        0x302110 
    #define REG_TOUCH_OVERSAMPLE    0x302114
    #define REG_TOUCH_RZTHRESH      0x302118  
    #define REG_TOUCH_RAW_XY        0x30211C
    #define REG_TOUCH_RZ            0x302120
    #define REG_TOUCH_SCREEN_XY     0x302124  
    #define REG_TOUCH_TAG_XY        0x302128  
    #define REG_TOUCH_TAG           0x30212C
    #define REG_TOUCH_TAG1_XY       0x302130
    #define REG_TOUCH_TAG1          0x302134 
    #define REG_TOUCH_TAG2_XY       0x302138 
    #define REG_TOUCH_TAG2          0x30213C
    #define REG_TOUCH_TAG3_XY       0x302140
    #define REG_TOUCH_TAG3          0x302144
    #define REG_TOUCH_TAG4_XY       0x302148
    #define REG_TOUCH_TAG4          0x30214C    
    #define REG_TOUCH_TRANSFORM_A   0x302150
    #define REG_TOUCH_TRANSFORM_B   0x302154
    #define REG_TOUCH_TRANSFORM_C   0x302158
    #define REG_TOUCH_TRANSFORM_D   0x30215C
    #define REG_TOUCH_TRANSFORM_E   0x302160  
    #define REG_TOUCH_TRANSFORM_F   0x302164
    #define REG_TOUCH_CONFIG        0x302168
    #define REG_CTOUCH_TOUCH4_X     0x30216C
    #define REG_BIST_EN             0x302174
    #define REG_TRIM                0x302180
    #define REG_ANA_COMP            0x302184
    #define REG_SPI_WIDTH           0x302188  
    #define REG_TOUCH_DIRECT_XY     0x30218C
    #define REG_TOUCH_DIRECT_Z1Z2   0x302190
    #define REG_DATESTAMP           0x302564
    #define REG_CMDB_SPACE          0x302574
    #define REG_CMDB_WRITE          0x302578 
    #define REG_TRACKER             0x309000
    #define REG_TRACKER_1           0x309004
    #define REG_TRACKER_2           0x309008
    #define REG_TRACKER_3           0x30900C
    #define REG_TRACKER_4           0x309010
#endif    
    
/*******************************************************************************
*   EVE Chip Commands
*******************************************************************************/    
    
#define FT800_ACTIVE        0x00			// Initializes FT800
#define FT800_STANDBY	    0x41			// Place FT800 in Standby (clk running)
#define FT800_SLEEP		    0x42			// Place FT800 in Sleep (clk off)
#define FT800_PWRDOWN	    0x50			// Place FT800 in Power Down (core off)
#define FT800_CLKEXT	    0x44			// Select external clock source
#define FT800_CORERST	    0x68			// Reset core - all registers default  

#if defined EVE_FT800
    #define FT800_CLK36M	0x61			// Select 36MHz PLL
    #define FT800_CLK48M	0x62			// Select 48MHz PLL
#elif defined EVE_FT810    
    #define FT800_CLKINT	0x48			// Select internal clock source    
    #define FT800_PDROMS    0x49
    #define FT800_CLKSEL	0x62			// Select 48MHz PLL
    //#define FT800_CLKSEL	0x61			// Select 36MHz PLL
         
    #define FT800_PINDRIVE  0x70
    #define PIN_PD_STATE    0x71
#endif    
    
    // FT800 Memory Commands.
#define MEMORY_WRITE    0x800000	    // FT800 Host Memory Write 
#define MEMORY_READ		0x000000		// FT800 Host Memory Read    

/*******************************************************************************
*   Definition about swaping mode.
*******************************************************************************/
    
#define DLSWAP_LINE           1UL    
#define DLSWAP_FRAME          2UL    
    
/*******************************************************************************
*   Definition of contants for primitive type
*******************************************************************************/ 
    
#define PRIMITIVE_BITMAP        1
#define PRIMITIVE_POINT         2
#define PRIMITIVE_LINE          3
#define PRIMITIVE_LINE_STRIP    4
#define PRIMITIVE_EDGE_STRIP_R  5
#define PRIMITIVE_EDGE_STRIP_L  6
#define PRIMITIVE_EDGE_STRIP_A  7
#define PRIMITIVE_EDGE_STRIP_B  8
#define PRIMITIVE_RECTANGLE     9    
    
/*******************************************************************************
*   Definition of contants for the command BITMAP_LAYOUT
*******************************************************************************/     
    
#define BITMAP_LAYOUT_ARGB1555          0    
#define BITMAP_LAYOUT_L1                1
#define BITMAP_LAYOUT_L4                2
#define BITMAP_LAYOUT_L8                3
#define BITMAP_LAYOUT_RGB332            4
#define BITMAP_LAYOUT_ARGB2             5
#define BITMAP_LAYOUT_ARGB4             6
#define BITMAP_LAYOUT_RGB565            7 
#define BITMAP_LAYOUT_TEXT8X8           9
#define BITMAP_LAYOUT_TEXTVGA           10
#define BITMAP_LAYOUT_BARGRAPH          11    

#if defined EVE_FT800
    #define BITMAP_LAYOUT_PALETTED      8
#elif defined EVE_FT810
    #define BITMAP_LAYOUT_PALETTED565   14
    #define BITMAP_LAYOUT_PALETTED4444  15
    #define BITMAP_LAYOUT_PALETTED8     16
    #define BITMAP_LAYOUT_L2            17
#endif    
    
/*******************************************************************************
*   Definition of contants for the command BITMAP_SIZE
*******************************************************************************/ 
    
#define BITMAP_SIZE_FILTER_NEAREST      0 
#define BITMAP_SIZE_FILTER_BILINEAR     1 
#define BITMAP_SIZE_WRAP_BORDER         0
#define BITMAP_SIZE_WRAP_REPEAT         1    
    
/*******************************************************************************
*   Definition of contants for the command ALPHA_FUNC
*******************************************************************************/
    
#define ALPHA_FUNC_NEVER        0
#define ALPHA_FUNC_LESS         1
#define ALPHA_FUNC_LEQUAL       2
#define ALPHA_FUNC_GREATER      3
#define ALPHA_FUNC_GEQUAL       4
#define ALPHA_FUNC_EQUAL        5
#define ALPHA_FUNC_NOTEQUAL     6
#define ALPHA_FUNC_ALLWAYS      7     
    
/*******************************************************************************
*   Definition of contants for the command STENCIL_FUNC
*******************************************************************************/  
    
#define STENCIL_FUNC_NEVER      0
#define STENCIL_FUNC_LESS       1
#define STENCIL_FUNC_LEQUAL     2
#define STENCIL_FUNC_GREATER    3
#define STENCIL_FUNC_GEQUAL     4
#define STENCIL_FUNC_EQUAL      5
#define STENCIL_FUNC_NOTEQUAL   6
#define STENCIL_FUNC_ALLWAYS    7   

/*******************************************************************************
*   Definition of contants for the SCISSOR SIZE
*******************************************************************************/  
    
#if defined EVE_FT800
    #define SCISSOR_RESTORE_SIZE    512
#endif    
    
#if defined EVE_FT810
    #define SCISSOR_RESTORE_SIZE    2048
#endif    
 
    
/*******************************************************************************
*   Definition of contants for the command BLEND_FUNC
*******************************************************************************/  
    
#define BLEND_FUNC_ZERO                  0
#define BLEND_FUNC_ONE                   1
#define BLEND_FUNC_SRC_ALPHA             2
#define BLEND_FUNC_DST_ALPHA             3
#define BLEND_FUNC_ONE_MINUS_SRC_ALPHA   4  
#define BLEND_FUNC_ONE_MINUS_DST_ALPHA   5    
    
/*******************************************************************************
*   Definition of contants for the command STENCIL_OP
*******************************************************************************/  
  
#define STENCIL_OP_ZERO         0    
#define STENCIL_OP_KEEP         1
#define STENCIL_OP_REPLACE      2
#define STENCIL_OP_INCR         3
#define STENCIL_OP_DECR         4
#define STENCIL_OP_INVERT       5
    
/*******************************************************************************
*   Definition of clear constants
*******************************************************************************/

#define CLEAR_TAG           0x01
#define CLEAR_STENCIL       0x02
#define CLEAR_COLOR         0x04    
    
/*******************************************************************************
*   EVE Display List Commands & related macros.
*    
*   Every DL (display list) commmand is implemented as a function like macro. 
*   For a description of parameters for every command read related comments.
*   Every macro is a wrapper to a display list commands, so to know what every
*   display command does read the FT Programmers Guide from FTDI.    
*******************************************************************************/    
    
/* ************************************************************************** */   
#define DL_DISPLAY          	0x00000000  

/* ************************************************************************** */   
#define DL_BITMAP_SOURCE    	0x01000000 
#define _DLBitmapSource(address) (DL_BITMAP_SOURCE | address)   
    
void DLBitmapSource(uint32 address);

/* ************************************************************************** */  
#define DL_CLEAR_COLOR_RGB    	0x02000000
#define _DLClearColorRGB(red, green, blue) (DL_CLEAR_COLOR_RGB | (red << 16) | (green << 8) | blue)
    
void DLClearColorRGB(uint8 red, uint8 green, uint8 blue);    
/* ************************************************************************** */  
#define DL_TAG             		0x03000000
#define _DLTag(s) (DL_TAG | s)    

void DLTag(uint8 tag);
/* ************************************************************************** */ 
#define DL_COLOR_RGB        	0x04000000 
#define _DLColorRGB(red, green, blue) (DL_COLOR_RGB | (red << 16) | (green << 8) | blue)    

void DLColorRGB(uint8 red, uint8 green, uint8 blue);
/* ************************************************************************** */  
#define DL_BITMAP_HANDLE    	0x05000000
#define _DLBitmapHandle(handle) (DL_BITMAP_HANDLE | handle)  

void DLBitmapHandle(uint8 handle);
/* ************************************************************************** */      
#define DL_CELL             	0x06000000
#define _DLCell(cell) (DL_CELL | cell)    

void DLCell(uint8 cell);
/* ************************************************************************** */  
#define DL_BITMAP_LAYOUT    	0x07000000
#define _DLBitmapLayout(format, linestride, height) (DL_BITMAP_LAYOUT | ((format & 0x1F) << 19) | ((linestride & 0x03FF) << 9) | (height & 0x01FF))    
    
void DLBitmapLayout(uint8 format, uint16 linestride, uint16 height);
/* ************************************************************************** */        
#define DL_BITMAP_SIZE      	0x08000000
#define _DLBitmapSize(filter, wrapx, wrapy, width, height) (DL_BITMAP_SIZE | ((filter & 0x01) << 20) | ((wrapx & 0x01) << 19) | ((wrapy & 0x01) << 18) | ((width & 0x01FF) << 9) | (height & 0x01FF))    

void DLBitmapSize(uint8 filter, uint8 wrapx, uint8 wrapy, uint16 width, uint16 height);
/* ************************************************************************** */       
#define DL_ALPHA_FUNC       	0x09000000
#define _DLAlphaFunc(func, ref) (DL_ALPHA_FUNC | (func << 8) | ref)    

void DLAlphaFunc(uint8 func, uint8 ref);
/* ************************************************************************** */      
#define DL_STENCIL_FUNC     	0x0A000000
#define _DLStencilFunc(func, ref, mask) (DL_STENCIL_FUNC | (func << 16) | (ref << 8) | mask) 

void DLStencilFunc(uint8 func, uint8 ref, uint8 mask);
/* ************************************************************************** */ 
#define DL_BLEND_FUNC       	0x0B000000
#define _DLBlendFunc(src, dst) (DL_BLEND_FUNC | (src << 3) | dst)  

void DLBlendFunc(uint8 src, uint8 dst);
/* ************************************************************************** */     
#define DL_STENCIL_OP       	0x0C000000
#define _DLStencilOp(sfail, spass) (DL_STENCIL_OP | (sfail << 3) | spass)   

void DLStencilOp(uint8 sfail, uint8 spass);
/* ************************************************************************** */ 
#define DL_POINT_SIZE       	0x0D000000
#define _DLPointSize(size) (DL_POINT_SIZE | size)  

void DLPointSize(uint32 size);
/* ************************************************************************** */  
#define DL_LINE_WIDTH       	0x0E000000
#define _DLLineWidth(width) (DL_LINE_WIDTH | width)

void DLLineWidth(uint16 width);
/* ************************************************************************** */  
#define DL_COLOR_A          	0x10000000 
#define _DLColorA(alpha) (DL_COLOR_A | alpha)

void DLColorA(uint8 alpha);
/* ************************************************************************** */     
#define DL_CLEAR_STENCIL    	0x11000000
#define _DLClearStencil(s) (DL_CLEAR_STENCIL | s)  

void DLClearStencil(uint8 s);
/* ************************************************************************** */     
#define DL_CLEAR_TAG        	0x12000000
#define _DLClearTag(t) (DL_CLEAR_TAG | t)    

void DLClearTag(uint8 t);
/* ************************************************************************** */     
#define DL_STENCIL_MASK     	0x13000000
#define _DLStencilMask(mask) (DL_STENCIL_MASK | mask)   

void DLStencilMask(uint8 mask);
/* ************************************************************************** */     
#define DL_TAG_MASK         	0x14000000
#define _DLTagMask(mask) (DL_TAG_MASK | mask) 

void DLTagMask(uint8 mask);
/* ************************************************************************** */     
#define DL_BITMAP_TFORM_A   	0x15000000
#define _DLBitmapTransformA(a) (DL_BITMAP_TFORM_A | a)    

void DLBitmapTransformA(uint32 a);
/* ************************************************************************** */   
#define DL_BITMAP_TFORM_B   	0x16000000
#define _DLBitmapTransformB(b) (DL_BITMAP_TFORM_B | b)  

void DLBitmapTransformB(uint32 b);
/* ************************************************************************** */   
#define DL_BITMAP_TFORM_C   	0x17000000
#define _DLBitmapTransformC(c) (DL_BITMAP_TFORM_C | c)    

void DLBitmapTransformC(uint32 c);
/* ************************************************************************** */  
#define DL_BITMAP_TFORM_D   	0x18000000
#define _DLBitmapTransformD(d) (DL_BITMAP_TFORM_D | d)    

void DLBitmapTransformD(uint32 d);
/* ************************************************************************** */   
#define DL_BITMAP_TFORM_E   	0x19000000
#define _DLBitmapTransformE(e) (DL_BITMAP_TFORM_E | e) 

void DLBitmapTransformE(uint32 e);
/* ************************************************************************** */   
#define DL_BITMAP_TFORM_F   	0x1A000000
#define _DLBitmapTransformF(f) (DL_BITMAP_TFORM_F | f) 

void DLBitmapTransformF(uint32 f);
/* ************************************************************************** */   
#define DL_SCISSOR_XY       	0x1B000000

#if defined EVE_FT800
    #define _DLScissorXY(x, y) (DL_SCISSOR_XY | (x << 9) | y) 
#elif defined EVE_FT810
    #define _DLScissorXY(x, y) (DL_SCISSOR_XY | (x << 11) | y)
#endif

void DLScissorXY(uint16 x, uint16 y);
/* ************************************************************************** */  
#define DL_SCISSOR_SIZE     	0x1C000000

#if defined EVE_FT800
    #define _DLScissorSize(width, height) (DL_SCISSOR_SIZE | (width << 10) | height)
#elif defined EVE_FT810
    #define _DLScissorSize(width, height) (DL_SCISSOR_SIZE | (width << 12) | height)
#endif    

void DLScissorSize(uint16 width, uint16 height);
/* ************************************************************************** */  
#define DL_JUMP             	0x1E000000
#define _DLJump(address) (DL_JUMP | address)    

void DLJump(uint16 address);
/* ************************************************************************** */      
#define DL_BEGIN            	0x1F000000
#define _DLBegin(primitive) (DL_BEGIN | primitive)   

void DLBegin(uint8 primitive);
/* ************************************************************************** */     
#define DL_CALL             	0x1D000000
#define _DLCall(address) (DL_CALL | address)    

void DLCall(uint16 address);
/* ************************************************************************** */     
#define DL_COLOR_MASK       	0x20000000
#define _DLColorMask(r, g, b, a) (DL_COLOR_MASK | (r << 3) | (g << 2) | (b << 1) | a)    

void DLColorMask(uint8 r, uint8 g, uint8 b, uint8 a);
/* ************************************************************************** */ 
#define DL_END              	0x21000000
#define _DLEnd() (DL_END)    

void DLEnd();
/* ************************************************************************** */
#define DL_SAVE_CONTEXT     	0x22000000
#define _DLSaveContext() (DL_SAVE_CONTEXT)    

void DLSaveContext();
/* ************************************************************************** */  
#define DL_RESTORE_CONTEXT  	0x23000000
#define _DLRestoreContext() (DL_RESTORE_CONTEXT)   

void DLRestoreContext();
/* ************************************************************************** */   
#define DL_RETURN           	0x24000000
#define _DLReturn() (DL_RETURN)    

void DLReturn();
/* ************************************************************************** */      
#define DL_MACRO            	0x25000000
#define _DLMacro(macro) (DL_MACRO | macro)   

void DLMacro(uint8 macro);
/* ************************************************************************** */    
#define DL_CLEAR            	0x26000000
#define _DLClear(color, stencil, tag) (DL_CLEAR | (color << 2) | (stencil << 1) | tag) 

void DLClear(uint8 color, uint8 stencil, uint8 tag);
/* ************************************************************************** */ 
#define DL_VERTEX2F         	0x40000000

#if defined EVE_FT800
    #define _DLVertex2F(x, y) (DL_VERTEX2F | ((x << (4 - vertexFormat)) << 15) | (y << (4 - vertexFormat)))  
#elif defined EVE_FT810
    #define _DLVertex2F(x, y) (DL_VERTEX2F | (x << 15) | y)    
#endif    

void DLVertex2F(int16 x, int16 y);
/* ************************************************************************** */     
#define DL_VERTEX2II        	0x80000000
#define _DLVertex2II(x, y, handle, cell) (DL_VERTEX2II | (x << 21) | (y << 12) | (handle << 7) | cell) 

void DLVertex2II(uint16 x, uint16 y, uint8 handle, uint8 cell);

/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */
    #define VERTEX_FORMAT_1         0
    #define VERTEX_FORMAT_1_2       1
    #define VERTEX_FORMAT_1_4       2
    #define VERTEX_FORMAT_1_8       3
    #define VERTEX_FORMAT_1_16      4
/* ************************************************************************** */    

void DLVertexFormat(uint8 format);
/* ************************************************************************** */

#if defined EVE_FT810

    #define DL_VERTEX_FORMAT        0x27000000
    #define _DLVertexFormat(format) (DL_VERTEX_FORMAT | (format & 0x00000003))
    /* ************************************************************************** */ 
    #define DL_BITMAP_LAYOUT_H    	0x28000000
    #define _DLBitmapLayout_H(linestride, height) (DL_BITMAP_LAYOUT_H | ((linestride & 0x0C00) >> 8) | ((height & 0x0600) >> 9)) 
    /* ************************************************************************** */
    #define DL_BITMAP_SIZE_H      	0x29000000
    #define _DLBitmapSize_H(width, height) (DL_BITMAP_SIZE_H | ((width & 0x0600) >> 7) | ((height & 0x0600) >> 9)) 
    /* ************************************************************************** */
    #define DL_PALETTE_SOURCE       0x2A000000
    #define _DLPaletteSource(address) (DL_PALETTE_SOURCE | (address & 0x003FFFFF))
    
    void DLPaletteSource(uint32 address);
    /* ************************************************************************** */
    #define DL_VERTEX_TRANSLATE_X   0x2B000000
    #define _DLVertexTranslateX(translation)  (DL_VERTEX_TRANSLATE_X | (translation & 0x0001FFFF))
    
    void DLVertexTranslateX(int32 translation);
    /* ************************************************************************** */
    #define DL_VERTEX_TRANSLATE_Y   0x2C000000
    #define _DLVertexTranslateY(translation)  (DL_VERTEX_TRANSLATE_Y | (translation & 0x0001FFFF))
    
    void DLVertexTranslateY(int32 translation);    
    /* ************************************************************************** */
    #define DL_NOP                  0x2D000000
    #define _DLNop() (DL_NOP)
    
    void DLNop();
    
#endif    

/*******************************************************************************
*   EVE Coproccesor List Commands & related macros.
*    
*   Every CMD (coproccesor list) commmand is implemented as a function like
*   macro. 
*   For a description of parameters for every command read related comments.
*   Every macro is a wrapper to a coproccesor list command, so to know what
*   every display command does read the FT Programmers Guide from FTDI.    
*******************************************************************************/   

/*******************************************************************************
*   Data types for data formating.
*******************************************************************************/

typedef struct { int32 Command; } CINT32;
typedef struct { int32 Command; int8 v1; } CINT32_INT8;
typedef struct { int32 Command; int8 v0; int8 v1; int8 v2; int8 v3; } CINT32_4INT8;
typedef struct { int32 Command; int32 v0; } CINT32_CINT32;
typedef struct { int32 Command; int32 v0; int32 v1; } CINT32_2CINT32;
typedef struct { int32 Command; int32 v0; int32 v1; int32 v2; } CINT32_3CINT32;
typedef struct { int32 Command; int16 v0; int16 v1; int8 v2; int8 v3; int8 v4; int8 v5; int16 v6; int16 v7; int8 v8; int8 v9; int8 v10; int8 v11; } CINT32_2INT16_4INT8_X2;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; } CINT32_4INT16;
//typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int8 v4; int8 v5; } CINT32_4INT16_2INT8;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int32 v4; } CINT32_4INT16_CINT32;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int32 v4; int16 v5; } CINT32_4INT16_CINT32_INT16;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int16 v4; } CINT32_5INT16;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int16 v4; int16 v5; } CINT32_6INT16;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int16 v4; int16 v5; int16 v6; } CINT32_7INT16;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int16 v4; int16 v5; int16 v6; int16 v7; } CINT32_8INT16;
typedef struct { int32 Command; int16 v0; int16 v1; int16 v2; int16 v3; int16 v4; int16 v5; int16 v6; int16 v7; int16 v8; int16 v9; } CINT32_10INT16;




/*******************************************************************************
*   Option values for commands that use option parameter.
*******************************************************************************/

#define OPT_3D          0
#define OPT_RGB565      0
#define OPT_MONO        1
#define OPT_NODL        2
#define OPT_FLAT        256
#define OPT_SIGNED      256
#define OPT_CENTERX     512
#define OPT_CENTERY     1024
#define OPT_CENTER      1536
#define OPT_RIGHTX      2048
#define OPT_NOBACK      4096
#define OPT_NOTICKS     8192
#define OPT_NOHM        16384
#define OPT_NOPOINTER   16384
#define OPT_NOSECS      32768
#define OPT_NOHANDS     49152

/*******************************************************************************
*   EVE Coprocesor Commands & related macros.
*******************************************************************************/

/* ************************************************************************** */
#define CMD_DLSTART             0xffffff00
#define _CMDDLStart() ((uint8*)&(CINT32){CMD_DLSTART}), sizeof(CINT32), 0

void CMDDLStart();
/* ************************************************************************** */
#define CMD_SWAP                0xffffff01
#define _CMDSwap() ((uint8*)&(CINT32){CMD_SWAP}), sizeof(CINT32), 0   

void CMDSwap();
/* ************************************************************************** */
#define CMD_INTERRUPT           0xffffff02
#define _CMDInterrupt(milliseconds) \
    ((uint8*)&(CINT32_CINT32){CMD_INTERRUPT, milliseconds}), sizeof(CINT32_CINT32), 0

void CMDInterrupt(uint32 milliseconds);
/* ************************************************************************** */
#define CMD_BGCOLOR             0xffffff09
#define _CMDBgcolor(red, green, blue) \
    ((uint8*)&(CINT32_4INT8){CMD_BGCOLOR, blue, green, red, 0}), sizeof(CINT32_4INT8), 0

void CMDBgcolor(int8 red, int8 green, int8 blue);
/* ************************************************************************** */
#define CMD_FGCOLOR             0xffffff0a
#define _CMDFgcolor(red, green, blue) \
    ((uint8*)&(CINT32_4INT8){CMD_FGCOLOR, blue, green, red, 0}), sizeof(CINT32_4INT8), 0

void CMDFgcolor(int8 red, int8 green, int8 blue);
/* ************************************************************************** */
#define CMD_GRADIENT            0xffffff0b
#define _CMDGradient(x0, y0, red0, green0, blue0, x1, y1, red1, green1, blue1) \
    ((uint8*)&(CINT32_2INT16_4INT8_X2){CMD_GRADIENT, x0, y0, blue0, green0, red0, 0, x1, y1, blue1, green1, red1, 0}), sizeof(CINT32_2INT16_4INT8_X2), 0

void CMDGradient(int16 x0, int16 y0, int8 red0, int8 green0, int8 blue0, int16 x1, int16 y1, int8 red1, int8 green1, int8 blue1);
/* ************************************************************************** */
#define CMD_TEXT                0xffffff0c
#define _CMDTextNew(x, y, font, options, text) \
    ((uint8*)&(CINT32_4INT16){CMD_TEXT, x, y, font, options}), sizeof(CINT32_4INT16), (unsigned char*)text  

void CMDText(int16 x, int16 y, int16 font, int16 options, char* text);
/* ************************************************************************** */
#define CMD_BUTTON              0xffffff0d
#define _CMDButton(x, y, width, height, font, options, text) \
    ((uint8*)&(CINT32_6INT16){CMD_BUTTON, x, y, width, height, font, options}), sizeof(CINT32_6INT16), (unsigned char*)text  

void CMDButton(int16 x, int16 y, int16 width, int16 height, int16 font, int16 options, char* text);
/* ************************************************************************** */
#define CMD_KEYS                0xffffff0e
#define _CMDKeys(x, y, width, height, font, options, text) \
    ((uint8*)&(CINT32_6INT16){CMD_KEYS, x, y, width, height, font, options}), sizeof(CINT32_6INT16), (unsigned char*)text

void CMDKeys(int16 x, int16 y, int16 width, int16 height, int16 font, int16 options, char* text);
/* ************************************************************************** */
#define CMD_PROGRESS            0xffffff0f
#define _CMDProgressBar(x, y, width, height, options, value, range) \
    ((uint8*)&(CINT32_7INT16){CMD_PROGRESS, x, y, width, height, options, value, range}), sizeof(CINT32_7INT16), 0

void CMDProgressBar(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 range);
/* ************************************************************************** */
#define CMD_SLIDER              0xffffff10
#define _CMDSlider(x, y, width, height, options, value, range) \
    ((uint8*)&(CINT32_7INT16){CMD_SLIDER, x, y, width, height, options, value, range}), sizeof(CINT32_7INT16), 0

void CMDSlider(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 range);
/* ************************************************************************** */
#define CMD_SCROLLBAR           0xffffff11
#define _CMDScrollBar(x, y, width, height, options, value, size, range) \
    ((uint8*)&(CINT32_8INT16){CMD_SCROLLBAR, x, y, width, height, options, value, size, range}), sizeof(CINT32_8INT16), 0

void CMDScrollBar(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 size, int16 range);
/* ************************************************************************** */
#define CMD_TOGGLE              0xffffff12

#define TOGGLE_STATE_OFF    0
#define TOGGLE_STATE_ON     65535

#define _CMDToggle(x, y, width, font, options, state, text) \
    ((uint8*)&(CINT32_6INT16){CMD_TOGGLE, x, y, width, font, options, state}), sizeof(CINT32_6INT16), (unsigned char*)text

void CMDToggle(int16 x, int16 y, int16 width, int16 font, int16 options, int16 state, char* text);
/* ************************************************************************** */
#define CMD_GAUGE               0xffffff13
#define _CMDGauge(x, y, radius, options, major, minor, value, range) \
    ((uint8*)&(CINT32_8INT16){CMD_GAUGE, x, y, radius, options, major, minor, value, range}), sizeof(CINT32_8INT16), 0

void CMDGauge(int16 x, int16 y, int16 radius, int16 options, int16 major, int16 minor, int16 value, int16 range);
/* ************************************************************************** */
#define CMD_CLOCK               0xffffff14
#define _CMDClock(x, y, radius, options, hours, minutes, seconds, milliseconds) \
    ((uint8*)&(CINT32_8INT16){CMD_CLOCK, x, y, radius, options, hours, minutes, seconds, milliseconds}), sizeof(CINT32_8INT16), 0

void CMDClock(int16 x, int16 y, int16 radius, int16 options, int16 hours, int16 minutes, int16 seconds, int16 milliseconds);
/* ************************************************************************** */
#define CMD_CALIBRATE           0xffffff15
#define _CMDCalibrate() ((uint8*)&(CINT32_INT8){CMD_CALIBRATE, 0}), sizeof(CINT32_INT8), 0

void CMDCalibrate();
/* ************************************************************************** */
#define CMD_SPINNER             0xffffff16

#define SPINNER_ROUND   0
#define SPINNER_LINEAR  1
#define SPINNER_CLOCK   2
#define SPINNER_ORBIT   3

#define _CMDSpinner(x, y, style, scale) \
    ((uint8*)&(CINT32_4INT16){CMD_SPINNER, x, y, style, scale}), sizeof(CINT32_4INT16), 0

void CMDSpinner(int16 x, int16 y, int16 style, int16 scale);
/* ************************************************************************** */
#define CMD_STOP                0xffffff17
#define _CMDStop() ((uint8*)&(CINT32){CMD_STOP}), sizeof(CINT32), 0

void CMDStop();
/* ************************************************************************** */
#define CMD_MEMCRC              0xffffff18
/* ************************************************************************** */
#define CMD_REGREAD             0xffffff19
/* ************************************************************************** */
#define CMD_MEMWRITE            0xffffff1a
#define _CMDMemWrite(ptr, size) \
    ((uint8*)&(CINT32_2CINT32){CMD_MEMWRITE, ptr, size}), sizeof(CINT32_2CINT32), 0

void CMDMemWrite(int32 ptr, int32 size);
/* ************************************************************************** */
#define CMD_MEMSET              0xffffff1b
#define _CMDMemSet(ptr, value, size) \
    ((uint8*)&(CINT32_3CINT32){CMD_MEMSET, ptr, value, size}), sizeof(CINT32_3CINT32), 0

void CMDMemSet(int32 ptr, int32 value, int32 size);
/* ************************************************************************** */
#define CMD_MEMZERO             0xffffff1c
#define _CMDMemZero(ptr, size) \
    ((uint8*)&(CINT32_2CINT32){CMD_MEMZERO, ptr, size}), sizeof(CINT32_2CINT32), 0

void CMDMemZero(int32 ptr, int32 size);
/* ************************************************************************** */
#define CMD_MEMCPY              0xffffff1d
#define _CMDMemCopy(dest, src, size) \
    ((uint8*)&(CINT32_3CINT32){CMD_MEMCPY, dest, src, size}), sizeof(CINT32_3CINT32), 0

void CMDMemCopy(int32 dest, int32 src, int32 size);
/* ************************************************************************** */
#define CMD_APPEND              0xffffff1e
#define _CMDAppend(ptr, size) \
    ((uint8*)&(CINT32_2CINT32){CMD_APPEND, ptr, size}), sizeof(CINT32_2CINT32), 0

void CMDAppend(int32 ptr, int32 size);
/* ************************************************************************** */
#define CMD_SNAPSHOT            0xffffff1f
#define _CMDSnapshot(ptr) \
    ((uint8*)&(CINT32_CINT32){CMD_SNAPSHOT, ptr}), sizeof(CMD_SNAPSHOT), 0

void CMDSnapshot(int32 ptr);
/* ************************************************************************** */
#define CMD_INFLATE             0xffffff22
#define _CMDInflate(ptr) \
    ((uint8*)&(CINT32_CINT32){CMD_INFLATE, ptr}), sizeof(CINT32_CINT32), 0

void CMDInflate(int32 ptr);
/* ************************************************************************** */
#define CMD_GETPTR              0xffffff23
#define _CMDGETPTR(dummy) \
    ((uint8*)&(CINT32_CINT32){CMD_GETPTR, dummy}), sizeof(CINT32_CINT32), 0

inline int32 CMDGetPtr();
/* ************************************************************************** */
#define CMD_LOADIMAGE           0xffffff24
#define _CMDLoadImage(ptr, options) \
    ((uint8*)&(CINT32_2CINT32){CMD_LOADIMAGE, ptr, options}), sizeof(CINT32_2CINT32), 0

void CMDLoadImage(int32 ptr, int32 options);
void CMDLoadImage_Data(uint8* data, uint32 datalength, uint8 isend);
/* ************************************************************************** */
#define CMD_GETPROPS            0xffffff25
#define _CMDGETPROPS(ptr, dummy_width, dummy_height) \
    ((uint8*)&(CINT32_3CINT32){CMD_GETPROPS, ptr, dummy_width, dummy_height}), sizeof(CINT32_3CINT32), 0

void CMDGetProps(int32 ptr, int32* width, int32* height);
/* ************************************************************************** */
#define CMD_LOADIDENTITY        0xffffff26
#define _CMDLoadIdentity() ((uint8*)&(CINT32){CMD_LOADIDENTITY}), sizeof(CINT32), 0

void CMDLoadIdentity();
/* ************************************************************************** */
#define CMD_TRANSLATE           0xffffff27
#define _CMDTranslate(tx, ty) \
    ((uint8*)&(CINT32_2CINT32){CMD_TRANSLATE, tx, ty}), sizeof(CINT32_2CINT32), 0

void CMDTranslate(int32 tx, int32 ty);
/* ************************************************************************** */
#define CMD_SCALE               0xffffff28
#define _CMDScale(x, y) \
    ((uint8*)&(CINT32_2CINT32){CMD_SCALE, (x), (y)}), sizeof(CINT32_2CINT32), 0

void CMDScale(int32 x, int32 y);
/* ************************************************************************** */
#define CMD_ROTATE              0xffffff29
#define _CMDRotate(angle) \
    ((uint8*)&(CINT32_CINT32){CMD_ROTATE, ((angle))}), sizeof(CINT32_CINT32), 0

void CMDRotate(int32 angle);
/* ************************************************************************** */
#define CMD_SETMATRIX           0xffffff2a
#define _CMDSetmatrix() ((uint8*)&(CINT32){CMD_SETMATRIX}), sizeof(CINT32), 0

void CMDSetmatrix();
/* ************************************************************************** */
#define CMD_SETFONT             0xffffff2b
#define _CMDSetfont(font, ptr) \
    ((uint8*)&(CINT32_2CINT32){CMD_SETFONT, font, ptr}), sizeof(CINT32_2CINT32), 0

void CMDSetfont(int32 font, int32 ptr);
/* ************************************************************************** */
#define CMD_TRACK               0xffffff2c
#define _CMDTrack(x, y, width, height, tag) \
    ((uint8*)&(CINT32_5INT16){CMD_TRACK, x, y, width, height, tag}), sizeof(CINT32_5INT16), 0

void CMDTrack(int16 x, int16 y, int16 width, int16 height, int16 tag);
/* ************************************************************************** */
#define CMD_DIAL                0xffffff2d
#define _CMDDial(x, y, radius, options, value) \
    ((uint8*)&(CINT32_5INT16){CMD_DIAL, x, y, radius, options, value}), sizeof(CINT32_5INT16), 0

void CMDDial(int16 x, int16 y, int16 radius, int16 options, int16 value);
/* ************************************************************************** */
#define CMD_NUMBER              0xffffff2e
#define _CMDNumber(x, y, font, options, number) \
    ((uint8*)&(CINT32_4INT16_CINT32){CMD_NUMBER, x, y, font, options, number}), sizeof(CINT32_4INT16_CINT32), 0

void CMDNumber(int16 x, int16 y, int16 font, int16 options, int32 number);
/* ************************************************************************** */
#define CMD_SCREENSAVER         0xffffff2f
#define _CMDScreenSaver() ((uint8*)&(CINT32){CMD_SCREENSAVER}), sizeof(CINT32), 0

void CMDScreenSaver();
/* ************************************************************************** */
#define CMD_SKETCH              0xffffff30
#define _CMDSketch(x, y, width, height, ptr, format) \
    ((uint8*)&(CINT32_4INT16_CINT32_INT16){CMD_SKETCH, x, y, width, height, ptr, format}), sizeof(CINT32_4INT16_CINT32_INT16), 0

void CMDSketch(int16 x, int16 y, int16 width, int16 height, int32 ptr, int16 format);
/* ************************************************************************** */
#define CMD_LOGO                0xffffff31
#define _CMDLogo() ((uint8*)&(CINT32){CMD_LOGO}), sizeof(CINT32), 0

void CMDLogo();
/* ************************************************************************** */
#define CMD_COLDSTART           0xffffff32
#define _CMDColdstart() ((uint8*)&(CINT32){CMD_COLDSTART}), sizeof(CINT32), 0 

void CMDColdstart();
/* ************************************************************************** */
#define CMD_GETMATRIX           0xffffff33
/* ************************************************************************** */
#define CMD_GRADCOLOR           0xffffff34
#define _CMDGradcolor(red, green, blue) \
    ((uint8*)&(CINT32_4INT8){CMD_GRADCOLOR, blue, green, red, 0}), sizeof(CINT32_4INT8), 0

void CMDGradcolor(int8 red, int8 green, int8 blue);


/* ************************************************************************** */
/* ************************************************************************** */

void CMDSetBitmap(int32 address, int16 format, int16 width, int16 height);
/* ************************************************************************** */

#if defined EVE_FT810

    #define NUMBER_BASE_BINARY      2
    #define NUMBER_BASE_OCTAL       8
    #define NUMBER_BASE_DECIMAL     10
    #define NUMBER_BASE_HEXADECIMAL 16
    
    /* ************************************************************************** */
    #define CMD_CSKETCH         0xFFFFFF35
    /* ************************************************************************** */
    #define CMD_SETROTATE       0xFFFFFF36
    #define _CMDSETROTATE(rotation) \
        ((uint8*)&(CINT32_CINT32){CMD_SETROTATE, rotation}), sizeof(CINT32_CINT32), 0
    
    void CMDSetRotate(int32 rotation);
    /* ************************************************************************** */
    #define CMD_SNAPSHOT2       0xFFFFFF37
    /* ************************************************************************** */
    #define CMD_SETBASE         0xFFFFFF38
    #define _CMDSETBASE(base) \
        ((uint8*)&(CINT32_CINT32){CMD_SETBASE, base}), sizeof(CINT32_CINT32), 0

    void CMDSetBase(int32 base);
    /* ************************************************************************** */
    #define CMD_MEDIAFIFO       0xFFFFFF39
    /* ************************************************************************** */
    #define CMD_PLAYVIDEO       0xFFFFFF3A
    /* ************************************************************************** */
    #define CMD_SETFONT2        0xFFFFFF3B
    #define _CMDSETFONT2(font, ptr, firstchar) \
        ((uint8*)&(CINT32_3CINT32){CMD_SETFONT2, font, ptr, firstchar}), sizeof(CINT32_3CINT32), 0 
    
    void CMDSetfont2(int32 font, int32 ptr, int32 firstchar);
    /* ************************************************************************** */
    #define CMD_SETSCRATCH      0xFFFFFF3C
    /* ************************************************************************** */
    #define CMD_ROMFONT         0xFFFFFF3F
    #define _CMDROMFONT(handle, font) \
        ((uint8*)&(CINT32_2CINT32){CMD_ROMFONT, handle, font}), sizeof(CINT32_2CINT32), 0

    void CMDRomfont(int32 font, int32 handle);
    /* ************************************************************************** */
    #define CMD_VIDEOSTART      0xFFFFFF40
    /* ************************************************************************** */
    #define CMD_VIDEOFRAME      0xFFFFFF41
    /* ************************************************************************** */
    #define CMD_SETBITMAP       0xFFFFFF43   
    #define _CMDSetBitmap(address, format, width, height) \
    ((uint8*)&(CINT32_5INT16){CMD_SETBITMAP, address, format, width, height, 0}), sizeof(CINT32_5INT16), 0
        //((uint8*)&(CINT32_4INT16_2INT8){CMD_SETBITMAP, address, format, width, height, 0, 0}), sizeof(CINT32_4INT16_2INT8), 0
    
    
    //void CMDSetBitmap(int32 address, int16 format, int16 width, int16 height);
    /* ************************************************************************** */
    
#endif    
    
/*******************************************************************************
*   Typedefs.
*******************************************************************************/

/* Typedef name : LISTTYPE.
   Description: To be used to know if there is a list in progress and wich type of
                list.
*/
typedef enum { NONE, DLIST, DATA } TRANSFERTYPE;

typedef enum { OK, UNKNOWN_LIST_TYPE, LIST_IN_PROGRESS, LIST_NOT_IN_PROGRESS } FTERROR;

/*******************************************************************************
*   Definition of constants for EVE_CoPro_EndDisplayList
*******************************************************************************/

typedef enum { END_DL_NOSWAP, END_DL_SWAP } SWAPACTION;
 

/*******************************************************************************
*   Variables.
*******************************************************************************/
    
extern uint16 ramCMDOffset;
 
/*******************************************************************************
*   Function prototypes.
*******************************************************************************/

uint8 FTIsCoproccesorReady();
uint16 FTGetCMDFifoFreeSpace();
void FT_Write_ByteArray_4(const uint8 *data, uint32 length);
FTERROR FT_ListStart(TRANSFERTYPE transfertype);
//FTERROR FT_ListStart(LISTTYPE listtype);
FTERROR FT_ListEnd(SWAPACTION swap);
//void DLListNewItem(uint32 item);
FTERROR FT_InflateFromFlash(const uint8 *flashptr, uint32 ramgptr, uint32 size);
FTERROR FT_InflateFromExternalFlash(uint32 flashptr, uint32 ramgptr, uint32 size);
FTERROR FT_LoadImageFromExternalFlash(uint32 flashptr, uint32 ramgptr, uint32 size, uint16 options);
FTERROR FT_TransferToRAMG(uint32 flashptr, uint32 size);    

#endif /* End PSOC_EVE_LIST_H */   

/* [] END OF FILE */
