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

////////////// Values for SPI ///////////////////////

//#define EVE_CS_on  		PORT2.PODR.BIT.B6=0
//#define EVE_CS_off 		PORT2.PODR.BIT.B6=1

#define Low_Byte(x)    					(x & 0xFF)         
#define High_Byte(x)   					((x >> 8) & 0xFF)
#define ERROR 1
#define OK    0


#define U8              unsigned char
#define U16             unsigned int
#define U32             unsigned long int

#define INCR                 3UL
#define ZERO                 0UL
#define GEQUAL               4UL
#define ALWAYS               7UL
#define KEEP                 1UL

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



//New
void wr8(U32 address, U8 data);
void wr16(U32 address, U16 data);
void wr32(U32 address, U32 data);
U8 rd8(U32 address);
U32 rd32(U32 address);
U8 EVE_BootUp(void);
void test_disp(void);
void test_disp2(U16 loops);
void test(void);
void updatecmdfifo(U16 count);
void waitcmdfifo_empty();
void play_sound(U8 sound, U8 note, U8 volume);
void play_song(U8 device,U8 vol,  U8 *song);
void video_timing(void);
void cmd_dlstart(void);
void 	Logo();
void test_number();
void Touch();
void Time();
void Time2(U16 sek, U16 min, U16 hr);
void CLEAR_FRAMES();
void SAMAPP_Sound();
void GPU_Ball_Stencil();

void demo_IO();
