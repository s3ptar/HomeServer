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

/***********************************************************************
* Includes
***********************************************************************/
#include "EVE.h"
#include "Utils.h"
#include "EVE_HAL.h"
#include "copro_commands.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array


/***********************************************************************
* Global Variable
***********************************************************************/
extern unsigned int dli,cli;
unsigned int lum=100;
extern U16 ms,sek,hr,min;
/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Local Variable
***********************************************************************/

U16 __ft_cmd_fifo_wp = 0; //FT800 coprocessor fifo write pointer

/***********************************************************************
* Local Funtions
***********************************************************************/

/***********************************************************************
*! \fn			U8 EVE_BootUp(void)
*  \brief		Power Eve Device Up
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
U8 EVE_BootUp(void){
    unsigned int x;
    init_EVE_IO(); 
  
    EVE_Power(1); 
    EVE_Power(0); 
    for (x=0;x<1000000;x++);
    EVE_Power(1); 
    for (x=0;x<1000000;x++);
    //<11MHz SPI-Clock
    //read location 0 to wake up FT800
    host_command(ACTIVE);
    //send command to change the PLL to external clock
    host_command(CLKEXT);
    return OK;
} 

/***********************************************************************
*! \fn			void updatecmdfifo(U16 count)
*  \brief		update the commands
*  \param		U16 count
*  \exception	none
*  \return		none
***********************************************************************/
void updatecmdfifo(U16 count){
  
   __ft_cmd_fifo_wp  = (__ft_cmd_fifo_wp + count) & 4095;

   //4 byte alignment
   __ft_cmd_fifo_wp = (__ft_cmd_fifo_wp + 3) & 0xffc;
   wr16(REG_CMD_WRITE,__ft_cmd_fifo_wp);

}

/***********************************************************************
*! \fn			void waitcmdfifo_empty(void)
*  \brief		Wait for the complete consumption of Coprocessor commands
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void waitcmdfifo_empty(void){
	int cmdrdptr,cmdwrptr;

	do{
		cmdrdptr = rd16(REG_CMD_READ);
		cmdwrptr = rd16(REG_CMD_WRITE);
	}while (cmdwrptr != cmdrdptr);
    __ft_cmd_fifo_wp =rd16(REG_CMD_WRITE);
}
/***********************************************************************
*! \fn			void recover_co_processor(void)
*  \brief		co processor in defin state
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void recover_co_processor(void){
    wr8(REG_CPURESET,1);
    wr32(REG_CMD_WRITE,0);
    wr32(REG_CMD_READ,0);
    wr8(REG_CPURESET,0);
}

/***********************************************************************
*! \fn			void play_sound(U8 sound, U8 note, U8 volume)
*  \brief		play an note on defind volume
*  \param		U8 sound, U8 note, U8 volume
*  \exception	none
*  \return		none
***********************************************************************/
void play_sound(U8 sound, U8 note, U8 volume){
    
    wr8(REG_VOL_SOUND,volume);
    wr16(REG_SOUND,sound|(note<<8));
    wr8(REG_PLAY,1);
  
}  

/***********************************************************************
*! \fn			void TFT_off(void)
*  \brief		TFT Off
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void TFT_off(void){
    wr8(REG_GPIO,0x00);      //Bit7 low
}  

/***********************************************************************
*! \fn			void TFT_on(void)
*  \brief		TFT On
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void TFT_on(void){
    wr8(REG_GPIO,0x80);      //Bit7 high
}
/***********************************************************************
*! \fn			void TFT_dim(U8 percent)
*  \brief		TFT dimm
*  \param		U8 percent
*  \exception	none
*  \return		none
***********************************************************************/
void TFT_dim(U8 percent){
    wr16(REG_PWM_DUTY,(100-percent)*0x80/100);
}  

/***********************************************************************
*! \fn			void TFT_fadeout(U16 zeit)
*  \brief		TFT fade out
*  \param		U16 zeit
*  \exception	none
*  \return		none
***********************************************************************/
void TFT_fadeout(U16 zeit){
    U8 x;
    x=lum;
    while(x--){
        delay_ms(zeit);
        TFT_dim(x);
    } 
}

/***********************************************************************
*! \fn			void TFT_fadein(U16 zeit)
*  \brief		TFT fade in
*  \param		U16 zeit
*  \exception	none
*  \return		none
***********************************************************************/
void TFT_fadein(U16 zeit){
    U8 x;
    for(x=0;x<=lum;x++){
        delay_ms(zeit);
        TFT_dim(x);
    } 
}

/***********************************************************************
*! \fn			void dl(unsigned long cmd)
*  \brief		load data
*  \param		unsigned long cmd
*  \exception	none
*  \return		none
***********************************************************************/
void dl(unsigned long cmd){
    wr32(RAM_DL+dli,cmd);
    dli+=4;
}  

/***********************************************************************
*! \fn			void video_timing(void)
*  \brief		setup video timing
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
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
  
    //pause is necessary
    delay_ms(1000);
}  

/***********************************************************************
*! \fn			U32 read_CHIPID(void)
*  \brief		read Chip ID
*  \param		none
*  \exception	none
*  \return		U32 Chip ID
***********************************************************************/
U32 read_CHIPID(void){
    return (rd32(ROM_CHIPID));
}     

/***********************************************************************
*! \fn			void Touch(void)
*  \brief		callibrate the touchscreen
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void Touch(void){
        
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

/***********************************************************************
*! \fn			void Logo(void)
*  \brief		draw FTDI Logi
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void Logo(void){
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
/* [] END OF FILE */
