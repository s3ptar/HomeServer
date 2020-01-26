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
#ifndef _EVE_H_
#define _EVE_H_
/***********************************************************************
 * Includes
 **********************************************************************/
#include "EVE_Config.h"
#include "FT_GPU.h"
#include "timing.h"
/***********************************************************************
 * Informations
 **********************************************************************/


/***********************************************************************
 * Global Variable
 **********************************************************************/

/***********************************************************************
 * Constant
 **********************************************************************/

#define INCR                 3UL
#define ZERO                 0UL
#define GEQUAL               4UL
#define ALWAYS               7UL
#define KEEP                 1UL

//Host Commands
#define ACTIVE               0x00
#define STANDBY              0x41
#define SLEEP                0x42
#define PWRDOWN              0x50
#define CLKEXT               0x44
#define CLK48M               0x62
#define CLK36M               0x61
#define CORERST              0x68


#define OPT_CENTER           1536UL
#define OPT_CENTERX          512UL
#define OPT_CENTERY          1024UL
#define OPT_FLAT             256UL
#define OPT_MONO             1UL
#define OPT_NOBACK           4096UL
#define OPT_NODL             2UL
#define OPT_NOHANDS          49152UL
#define OPT_NOHM             16384UL
#define OPT_NOPOINTER        16384UL
#define OPT_NOSECS           32768UL
#define OPT_NOTICKS          8192UL
#define OPT_RIGHTX           2048UL
#define OPT_SIGNED           256UL
#define PALETTED             8UL
#define FTPOINTS             2UL
#define DEG0_NORMAL         0x08
#define DEG0_BMP            0x88
#define DEG90_NORMAL        0xA8
#define DEG90_BMP           0xE8
#define DEG180_NORMAL       0xC8      
#define DEG180_BMP          0x48
#define DEG270_NORMAL       0x68
#define DEG270_BMP          0x28


////////////////////////////// Display Variables ///////////////////////////////

#define ft_random(x)		            (rand() % (x))
#define ft_pgm_read_word(addr)          (*(int*)(addr))
#define ft_pgm_read_byte(x)             (*(x))

/***********************************************************************
 * Functions
 **********************************************************************/
//New
//void delay_ms(U32 zeit);
U8 EVE_BootUp(void);

U32 read_CHIPID(void);
void updatecmdfifo(U16 count);
void waitcmdfifo_empty(void);
void recover_co_processor();
void play_sound(U8 sound, U8 note, U8 volume);
void TFT_off(void);
void TFT_on(void);
void TFT_dim(U8 percent);
void TFT_fadeout(U16 zeit);
void TFT_fadein(U16 zeit);
void dl(unsigned long cmd);
void video_timing(void);
void cmd_dlstart(void);
void Logo(void);
void Touch(void);
    
    
#endif //_EVE_H_
/* [] END OF FILE */
