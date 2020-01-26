/* THIS SAMPLE CODE IS PROVIDED AS IS        */
/* AND IS SUBJECT TO ALTERATIONS. GLYN       */
/* ACCEPTS NO RESPONSIBILITY OR LIABILITY    */
/* FOR ANY ERRORS OR                         */
/* ELIGIBILITY FOR ANY PURPOSES.             */
/* (C) GLYN GmbH & Co. KG                    */
/*********************************************
Project : EVE_SPI.h
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

#include "stdint.h"

#define U8              unsigned char
#define U16             unsigned int
#define U32             unsigned long int

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

////////////////////////////// Display Variables ///////////////////////////////
#define ET0430
//#define ET0350
//#define ETV570

#ifdef ET0350
#define DispWidth        320
#define DispHeight       240
#endif

#ifdef ET0430
#define DispWidth        480
#define DispHeight       272
#endif

#ifdef ETV570
#define DispWidth        512
#define DispHeight       480
#endif


//New
U8 EVE_BootUp(void);

U32 read_CHIPID();
void updatecmdfifo(U16 count);
void waitcmdfifo_empty();
void recover_co_processor();
void play_sound(U8 sound, U8 note, U8 volume);
void TFT_off();
void TFT_on();
void TFT_dim(U8 percent);
void TFT_fadeout(U16 zeit);
void TFT_fadein(U16 zeit);
void dl(unsigned long cmd);
void video_timing(void);
void cmd_dlstart(void);
void Logo();
void Display_test(uint8_t second);
void Display_test2(uint8_t hour, uint8_t minute, uint8_t second);


extern unsigned int dli,cli;
