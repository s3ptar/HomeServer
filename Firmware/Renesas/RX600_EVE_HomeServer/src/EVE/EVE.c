/* THIS SAMPLE CODE IS PROVIDED AS IS        */
/* AND IS SUBJECT TO ALTERATIONS. GLYN       */
/* ACCEPTS NO RESPONSIBILITY OR LIABILITY    */
/* FOR ANY ERRORS OR                         */
/* ELIGIBILITY FOR ANY PURPOSES.             */
/* (C) GLYN GmbH & Co. KG                    */
/*********************************************
Project : EVE.c
          Graphic library
          EVE with FT800
          Display: G-ET0350G0DH6
                   G-ET0430G0DH6
Version : 0.1 Status: Demo
Date    : 29.10.2013

Author  : P.Doerwald / GLYN GmbH & Co. KG

Releases: V0.1

Info    : O
*********************************************/
#include "iodefine.h"   
#include "spi.h"
#include "EVE.h" 
#include "copro_commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "FT_Gpu.h"
#include "board.h"
#include <math.h>

#define ft_random(x)		(rand() % (x))
#define ft_pgm_read_word(addr)   (*(int*)(addr))

////////////// Values for SPI ///////////////////////



#define Low_Byte(x)    					(x & 0xFF)         
#define High_Byte(x)   					((x >> 8) & 0xFF)
#define ERR   1
#define OK    0

////////////// Values for MEMORY_ACCESS_CONTROL register ///////////////////////
#define DEG0_NORMAL     0x08
#define DEG0_BMP        0x88
#define DEG90_NORMAL    0xA8
#define DEG90_BMP       0xE8
#define DEG180_NORMAL   0xC8      
#define DEG180_BMP      0x48
#define DEG270_NORMAL   0x68
#define DEG270_BMP      0x28

#define U8              unsigned char
#define U16             unsigned int
#define U32             unsigned long int
#define I32             long int

#define ft_pgm_read_byte(x)        (*(x))

#define Pi  3.14159
//////////////////// SPI Transfer description byte /////////////////////////////
//#define ID 1					// Use this if BS0 is connected to High
#define ID 0					// Use this if BS0 is connected to low

#define INDEX_WRITE       (0x70 | DisplayID)
#define REG_WRITE         (0x72 | DisplayID)
#define REG_READ          (0x73 | DisplayID)
#define INDEX_WRITE_WORD  ((INDEX_WRITE)<<8)
#define REG_WRITE_WORD    ((REG_WRITE)<<8)
#define REG_READ_WORD     ((REG_READ)<<8)

#define ChipOn  1      
#define ChipOff 0 

U16 __ft_cmd_fifo_wp = 0; //FT800 coprocessor fifo write pointer
unsigned int dli,cli;
extern U16 lum;

/************************************************************************
*! \fn			void refresh_display(void)
*  \brief		Refresh Commands
*  \param		none
*  \exception	none
*  \return		none
************************************************************************/
void refresh_display(void){

	cmd(DISPLAY());
	cmd_swap();
	wr32(REG_CMD_WRITE,cli);
	/* Wait till coprocessor completes the operation */
	wr32(REG_CMD_WRITE,0);
	wr32(REG_CMD_READ,0);
	delay_ms(10);
	recover_co_processor();
	waitcmdfifo_empty();

}

U8 EVE_BootUp(void){

    unsigned int x;
    init_IO();

    EVE_Power_on;
    EVE_Power_off;
    for (x=0;x<1000000;x++);
    EVE_Power_on;
    for (x=0;x<1000000;x++);

    //<11MHz SPI-Clock
    //read location 0 to wake up FT800
    //Fehler
    host_command(ACTIVE);

    //send command to change the PLL to external clock
    host_command(CLKEXT);
	return OK;
}  




void updatecmdfifo(U16 count){
  
    __ft_cmd_fifo_wp  = (__ft_cmd_fifo_wp + count) & 4095;

    //4 byte alignment
    __ft_cmd_fifo_wp = (__ft_cmd_fifo_wp + 3) & 0xffc;
    wr16(REG_CMD_WRITE,__ft_cmd_fifo_wp);

}

/* Wait for the complete consumption of Coprocessor commands */
void waitcmdfifo_empty(){

	int cmdrdptr,cmdwrptr;

	do{
	    cmdrdptr = rd16(REG_CMD_READ);
		cmdwrptr = rd16(REG_CMD_WRITE);
	}while (cmdwrptr != cmdrdptr);
    __ft_cmd_fifo_wp =rd16(REG_CMD_WRITE);
}

void recover_co_processor(){

    wr8(REG_CPURESET,1);
    wr32(REG_CMD_WRITE,0);
    wr32(REG_CMD_READ,0);
    wr8(REG_CPURESET,0);
} 

void play_sound(U8 sound, U8 note, U8 volume){

    wr8(REG_VOL_SOUND,volume);
    wr16(REG_SOUND,sound|(note<<8));
    wr8(REG_PLAY,1);
  
}     
/******** controlling TFT *********************/
void TFT_off(){

    wr8(REG_GPIO,0x00);      //Bit7 low
}  
void TFT_on(){

    wr8(REG_GPIO,0x80);      //Bit7 high
}
void TFT_dim(U8 percent){

    wr16(REG_PWM_DUTY,(100-percent)*0x80/100);
}  

void TFT_fadeout(U16 zeit){

    U8 x;
    x=lum;
    while(x--){
        delay_ms(zeit);
        TFT_dim(x);
    }
}

void TFT_fadein(U16 zeit){

    U8 x;
    for(x=0;x<=lum;x++){
        delay_ms(zeit);
    TFT_dim(x);
    }
}

void dl(unsigned long cmd){

    wr32(RAM_DL+dli,cmd);
    dli+=4;
}  

void video_timing(void){

    #ifdef ET0350
        wr16(REG_HCYCLE,408);
        wr16(REG_HOFFSET,33);
        wr16(REG_HSIZE,320);
        wr16(REG_HSYNC0,0);
        wr16(REG_HSYNC1,30);
        wr16(REG_VCYCLE,263);
        wr16(REG_VOFFSET,4);
        wr16(REG_VSIZE,240);
        wr16(REG_VSYNC0,0);
        wr16(REG_VSYNC1,3);
   #endif
  
   #ifdef ET0430
       wr16(REG_HCYCLE,548);
       wr16(REG_HOFFSET,43);
       wr16(REG_HSIZE,480);
       wr16(REG_HSYNC0,0);
       wr16(REG_HSYNC1,41);
       wr16(REG_VCYCLE,292);
       wr16(REG_VOFFSET,12);
       wr16(REG_VSIZE,272);
       wr16(REG_VSYNC0,0);
       wr16(REG_VSYNC1,10);
   #endif
  
   #ifdef ETV570
       wr16(REG_HCYCLE,800);
       wr16(REG_HOFFSET,30);
       wr16(REG_HSIZE,512);
       wr16(REG_HSYNC0,0);
       wr16(REG_HSYNC1,41);
       wr16(REG_VCYCLE,525);
       wr16(REG_VOFFSET,12);
       wr16(REG_VSIZE,480);
       wr16(REG_VSYNC0,0);
       wr16(REG_VSYNC1,10);
   #endif
  
  
    wr16(REG_PWM_HZ,1000);
    TFT_dim(lum);
  
    wr8(REG_GPIO_DIR,0x80);  //Bit7 output
    wr8(REG_GPIO,0x80);      //Bit7 high
  
    wr32(RAM_DL,CLEAR_COLOR_RGB(0,0,0));
    wr32(RAM_DL+4,CLEAR(1,1,1));
    wr32(RAM_DL+8,DISPLAY());
    wr32(REG_DLSWAP,DLSWAP_FRAME);
  
    #ifdef ET0350
        wr16(REG_PCLK,7);
    #endif
  
    #ifdef ET0430
        wr16(REG_PCLK,5);
    #endif
  
    #ifdef ETV570
        wr16(REG_PCLK,4);
    #endif
  
    //pause is necessary
    delay_ms(1000);
}  




// read Chip ID
U32 read_CHIPID(){

    return (rd32(ROM_CHIPID));
}     

// callibrate the touchscreen
void Touch(){
        
    cli = 0;
    cmd_dlstart();
    cmd(CLEAR(1,1,1));
    cmd(COLOR_RGB(255,255,255));
    cmd_text(80,30,27,OPT_CENTERY,"Please tap on the dot");
    cmd_calibrate();
    cmd_swap();
    wr32(REG_CMD_WRITE,cli);
    /* Wait till coprocessor completes the operation */
    waitcmdfifo_empty();
    wr32(REG_CMD_WRITE,0);
    wr32(REG_CMD_READ,0);
    delay_ms(100);
}        

void Logo(){

	cli = 0;
    cmd_dlstart();
    cmd_logo();
    //wr32(REG_DLSWAP,1);
    cmd_swap();
    wr32(REG_CMD_WRITE,cli);
    /* Wait till coprocessor completes the operation */
	waitcmdfifo_empty();
    wr32(REG_CMD_WRITE,0);
    wr32(REG_CMD_READ,0);
    delay_ms(5000);
}


void Display_test(uint8_t second){

	cli = 0;
	cmd_dlstart();
	cmd(CLEAR(1, 1, 1));
	cmd(CLEAR_COLOR_RGB(31, 63, 127));
	cmd_clock(224, 134, 113, 0, 13, 51, second, 0);
	refresh_display();

}



void Display_test2(uint8_t hour, uint8_t minute, uint8_t second){

	char DateTime[19];
	cli = 0;
	cmd_dlstart();
	cmd(CLEAR(1, 1, 1));
	//Paint Background
	cmd(CLEAR_COLOR_RGB(85, 255, 127));
	cmd_gradient(0, 0, 0x007FFF, 477, 269, 0x7FFF00);
	cmd(TAG(1));
	cmd_button(165, 194, 120, 36, 27, 0, "Bark");
    cmd(BEGIN(RECTS));
	cmd(VERTEX2II(270, 13, 0, 0));
	cmd(VERTEX2II(470, 33, 0, 0));
	cmd(END());
	cmd(COLOR_RGB(0, 0, 0));
	sprintf(DateTime,"%02d:%02d:%02d %02d.%02d.%04d",hour, minute, second, 1,11,2018);
	cmd_text(273, 11, 28, 0, DateTime);

	if(rd16(REG_TOUCH_TAG)==1){
		cmd_button(165, 194, 120, 36, 27, 0, "Barked");
	}

	refresh_display();

}

void Display_test3(uint8_t hour, uint8_t minute, uint8_t second){

	char DateTime[19];
	cli = 0;
	uint8_t tag;
	tag=rd16(REG_TOUCH_TAG);

	cmd_dlstart();
	cmd(CLEAR(1, 1, 1));
	//Paint Background
	cmd(CLEAR_COLOR_RGB(85, 255, 127));
	cmd_gradient(0, 0, 0x007FFF, 477, 269, 0x7FFF00);
	cmd(TAG(1));
	if(!tag)
		cmd_button(165, 194, 120, 36, 27, 0, "Bark");
	else
		cmd_button(165, 194, 120, 36, 27, 0, "Wuff");
    cmd(BEGIN(RECTS));
	cmd(VERTEX2II(270, 13, 0, 0));
	cmd(VERTEX2II(470, 33, 0, 0));
	cmd(END());
	cmd(COLOR_RGB(0, 0, 0));
	sprintf(DateTime,"%02d:%02d:%02d %02d.%02d.%04d",hour, minute, second, 1,11,2018);
	cmd_text(273, 11, 28, 0, DateTime);

	refresh_display();

}
