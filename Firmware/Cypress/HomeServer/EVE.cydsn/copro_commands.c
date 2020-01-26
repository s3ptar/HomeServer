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
#include "EVE_HAL.h"
#include "copro_commands.h"
#include "Utils.h"
#include "string.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
/***********************************************************************
* Local Funtions
***********************************************************************/

/***********************************************************************
* Global Variable
***********************************************************************/
extern unsigned int dli,cli;
/***********************************************************************
* Constant
***********************************************************************/
#define RAM_DL                          0x100000
#define RAM_CMD                         0x108000

///////////////commands//////////////////////////////
#define BEGIN(x)                        0x1F000000|x
#define CALL(x)                         0x1D000000|x
#define CLEAR(x,y,z)                    0x26000000|(x<<2)|(y<<1)|z
#define CLEAR_COLOR_RGB(x,y,z)          0x02000000|(x<<16)|(y<<8)|z
#define COLOR_RGB(x,y,z)                0x04000000|(x<<16)|(y<<8)|z
#define DISPLAY()                       0x00
#define LINE_WIDTH(x)                   0x06000000|x
#define POINT_SIZE(x)                   0x0D000000|x
#define RETURN()                        0x24000000
#define VERTEX2F(a,b)                   0x40000000|b|(a<<15)
#define VERTEX2II(a,b,c,d)              0x80000000|d|(a<<21)|(b<<12)|(c<<7)
#define END()                           0x21000000


/***********************************************************************
*! \fn			void transfer_string(char *s)
*  \brief		transfer an string
*  \param		char *s - to transfer
*  \exception	none
*  \return		none
***********************************************************************/
void transfer_string(char *s){
    int length,n;

    length = strlen(s) + 1;
    for(n=0;n<length;n++)
        wr8(RAM_CMD+cli+n,s[n]);
    wr8(RAM_CMD+cli+n,0);
    /* increment the length and align it by 4 bytes */
    cli += ((length + 3) & ~3);
    // cli+=length+1;
}

/***********************************************************************
*! \fn			void cmd_bgcolor(U32 x)
*  \brief		set background color for next element
*  \param		U32 x
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_bgcolor(U32 x){
    wr32(RAM_CMD+cli,0xffffff09);
    cli+=4;
    wr32(RAM_CMD+cli,x);
    cli+=4;
}
/***********************************************************************
*! \fn			void cmd_fgcolor(U32 x)
*  \brief		set forecolor for next element
*  \param		U32 x
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_fgcolor(U32 x){
    wr32(RAM_CMD+cli,0xffffff0A);
    cli+=4;
    wr32(RAM_CMD+cli,x);
    cli+=4;
}
/***********************************************************************
*! \fn			void cmd_button(int x,int y, int w, int h, int font, int options, char* s)
*  \brief		drawn an button
*  \param		int x
*  \param		int y
*  \param		int w
*  \param		int h
*  \param		int font
*  \param		int options
*  \param		char* s
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_button(int x,int y, int w, int h, int font, int options, char* s){
    wr32(RAM_CMD+cli,0xffffff0D);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|font));
    cli+=4;
    transfer_string(s);
}
/***********************************************************************
*! \fn			void cmd_dial(int x, int y, int r, U16 options, U16 val)
*  \brief		draw an dial button
*  \param		int x
*  \param		int y
*  \param		int r
*  \param		U16 options
*  \param		U16 val
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_dial(int x, int y, int r, U16 options, U16 val){
    wr32(RAM_CMD+cli,0xffffff2d);
    cli+=4;
    wr32(RAM_CMD+cli,(y<<16)|(x&0xFFFF));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|r));
    cli+=4;
    wr32(RAM_CMD+cli,val);
    cli+=4;
}
/***********************************************************************
*! \fn			void cmd_keys(int x,int y, int w, int h, int font, int options, char* s)
*  \brief		draw an dkeys
*  \param		int x
*  \param		int y
*  \param		int w
*  \param		int h
*  \param		int font
*  \param		int options
*  \param		char* s
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_keys(int x,int y, int w, int h, int font, int options, char* s){
    wr32(RAM_CMD+cli,0xffffff0e);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|font));
    cli+=4;
    transfer_string(s);
}

/***********************************************************************
*! \fn			void cmd_gradient(int x0, int y0, U32 rgb0, int x1, int y1, U32 rgb1)
*  \brief		draw an gradient
*  \param		int x0
*  \param		int y0
*  \param		U32 rgb0
*  \param		int x1
*  \param		int y1
*  \param		U32 rgb1
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_gradient(int x0, int y0, U32 rgb0, int x1, int y1, U32 rgb1){
    wr32(RAM_CMD+cli,0xffffff0b);
    cli+=4;
    wr32(RAM_CMD+cli,((y0<<16)|(x0&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,rgb0);
    cli+=4;
    wr32(RAM_CMD+cli,((y1<<16)|x1));
    cli+=4;
    wr32(RAM_CMD+cli,rgb1);
    cli+=4;
}

/***********************************************************************
*! \fn			void cmd_progress(int x, int y, int w, int h, U16 options, U16 val, U16 range)
*  \brief		draw an progressbar
*  \param		int x
*  \param		int y
*  \param		int w
*  \param		int h
*  \param		U16 options
*  \param		U16 val
*  \param		U16 val
*  \param	    U16 range
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_progress(int x, int y, int w, int h, U16 options, U16 val, U16 range){
    wr32(RAM_CMD+cli,0xffffff0f);
    cli+=4;
    wr32(RAM_CMD+cli, (((U32)y<<16)|(x&0xFFFF)));
    cli+=4; 
    wr32(RAM_CMD+cli, (((U32)h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli, (((U32)val<<16)|options));
    cli+=4;
    wr32(RAM_CMD+cli, range);
    cli+=4;
     
}
/***********************************************************************
*! \fn			void cmd_progress(int x, int y, int w, int h, U16 options, U16 val, U16 range)
*  \brief		draw an progressbar
*  \param		int x
*  \param		int y
*  \param		int w
*  \param		int h
*  \param		U16 options
*  \param		U16 val
*  \param	    U16 range
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_scrollbar(int x, int y, int w, int h, int options, int val, int size, int range){
    wr32(RAM_CMD+cli,0xffffff11);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;     
    wr32(RAM_CMD+cli,((h<<16)|w));
    cli+=4;     
    wr32(RAM_CMD+cli,((val<<16)|options));
    cli+=4;    
    wr32(RAM_CMD+cli,((range<<16)|size));
    cli+=4;     
}
/***********************************************************************
*! \fn			void cmd_slider(int x, int y, int w, int h, U16 options, U16 val, U16 range)
*  \brief		draw an slider
*  \param		int x
*  \param		int y
*  \param		int w
*  \param		int h
*  \param		U16 options
*  \param		U16 val
*  \param   	U16 range
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_slider(int x, int y, int w, int h, U16 options, U16 val, U16 range){
    wr32(RAM_CMD+cli,0xffffff10);
    cli+=4;
    wr32(RAM_CMD+cli, (((U32)y<<16)|(x&0xFFFF)));
    cli+=4; 
    wr32(RAM_CMD+cli, (((U32)h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli, (((U32)val<<16)|options));
    cli+=4;
    wr32(RAM_CMD+cli, range);
    cli+=4;
     
}
/***********************************************************************
*! \fn			void cmd_spinner(int x, int y, U16 style, U16 scale)
*  \brief		draw an spinner
*  \param		int x
*  \param		int y
*  \param		U16 style
*  \param		U16 scale
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_spinner(int x, int y, U16 style, U16 scale){
    wr32(RAM_CMD+cli,0xffffff16);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((scale<<16)|style));
    cli+=4;
}

/***********************************************************************
*! \fn			void cmd_clock(int x, int y, int r, U16 options, U16 h, U16 m, U16 s, U16 ms)
*  \brief		draw an clock
*  \param		int x
*  \param		int y
*  \param		int r
*  \param		U16 options
*  \param		U16 h
*  \param		U16 m
*  \param   	U16 range
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_clock(int x, int y, int r, U16 options, U16 h, U16 m, U16 s, U16 ms){
    wr32(RAM_CMD+cli,0xffffff14);
    cli+=4;
    wr32(RAM_CMD+cli,(y<<16)|(x&0xFFFF));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|r));
    cli+=4;
    wr32(RAM_CMD+cli,((m<<16)|h));
    cli+=4;
    wr32(RAM_CMD+cli,((ms<<16)|s));
    cli+=4;
}

/***********************************************************************
*! \fn			void cmd_gauge(int x, int y, int r, U16 options, U16 major, U16 minor, U16 val, U16 range)
*  \brief		draw an gauge
*  \param		int x
*  \param		int y
*  \param		int r
*  \param		U16 options
*  \param		U16 major
*  \param		U16 minor
*  \param   	U16 val
*  \param   	U16 range
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_gauge(int x, int y, int r, U16 options, U16 major, U16 minor, U16 val, U16 range){
    wr32(RAM_CMD+cli,0xffffff13);
    cli+=4;
    wr32(RAM_CMD+cli,(y<<16)|(x&0xFFFF));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|r));
    cli+=4;
    wr32(RAM_CMD+cli,((minor<<16)|major));
    cli+=4;
    wr32(RAM_CMD+cli,((range<<16)|val));
    cli+=4;
}

/***********************************************************************
*! \fn			cmd_text(int x, int y, int font, int options, char* s)
*  \brief		draw an text
*  \param		int x
*  \param		int y
*  \param		int font
*  \param		int options
*  \param		char* s
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_text(int x, int y, int font, int options, char* s){
    wr32(RAM_CMD+cli,0xffffff0C);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|font));
    cli+=4;
    transfer_string(s);
}
/***********************************************************************
*! \fn			void cmd_number(int x, int y, int font, int options, U32 n)
*  \brief		draw an number
*  \param		int x
*  \param		int y
*  \param		int font
*  \param		int options
*  \param		U32 n
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_number(int x, int y, int font, int options, U32 n){
    wr32(RAM_CMD+cli,0xffffff2e);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|font));
    cli+=4;
    wr32(RAM_CMD+cli,n);
    cli+=4;
}

/***********************************************************************
*! \fn			void cmd_track(int x, int y, int w, int h, int tag)
*  \brief		draw an track
*  \param		int x
*  \param		int y
*  \param		int w
*  \param		int h
*  \param		Uint tag
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_track(int x, int y, int w, int h, int tag){
    wr32(RAM_CMD+cli,0xffffff2C);
    cli+=4;
    wr32(RAM_CMD+cli, (((U32)y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli, (((U32)h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli, tag);
    cli+=4;       
}

/***********************************************************************
*! \fn			cmd_coldstart(void)
*  \brief		send coldstart command
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_coldstart(void){
    wr32(RAM_CMD+cli,0xffffff32);
    cli+=4;
}
/***********************************************************************
*! \fn			cmd_dlstar
*  \brief		send dlstart command
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_dlstart(void){
    wr32(RAM_CMD+cli,0xffffff00);
    cli+=4;
}

/***********************************************************************
*! \fn			void cmd_stop(void)
*  \brief		send stop command
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_stop(void){
    wr32(RAM_CMD+cli,0xffffff17);
    cli+=4;

}

/***********************************************************************
*! \fn			void cmd_setfont(U32 font, U32 ptr)
*  \brief		set font
*  \param		U32 font
*  \param		U32 ptr
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_setfont(U32 font, U32 ptr){
    wr32(RAM_CMD+cli,0xffffff2b);
    cli+=4;
    wr32(RAM_CMD+cli,font);
    cli+=4;
    wr32(RAM_CMD+cli,ptr);
    cli+=4;  
}  
/***********************************************************************
*! \fn			void cmd_swap(void)
*  \brief		send swap command
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_swap(void){
    wr32(RAM_CMD+cli,0xffffff01);
    cli+=4;

}
/***********************************************************************
*! \fn			void cmd_logo(void)
*  \brief		draw logo
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_logo(void){
    wr32(RAM_CMD+cli,0xffffff31);
    cli+=4;
}
/***********************************************************************
*! \fn			void cmd_calibrate(void)
*  \brief		send calibration command
*  \param		none
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_calibrate(void){
    wr32(RAM_CMD+cli,0xffffff15);
    cli+=4;
}
/***********************************************************************
*! \fn			void cmd_memzero(U32 ptr,U32 num)
*  \brief		set memory to zero
*  \param		U32 ptr
*  \param		U32 num
*  \exception	none
*  \return		none
***********************************************************************/
void cmd_memzero(U32 ptr,U32 num){
    wr32(RAM_CMD+cli,0xffffff1c);
    cli+=4;
    wr32(RAM_CMD+cli,ptr);
    cli+=4;
    wr32(RAM_CMD+cli,num);
    cli+=4;
}
/***********************************************************************
*! \fn			void cmd(U32 befehl)
*  \brief		set command
*  \param		U32 befehl
*  \exception	none
*  \return		none
***********************************************************************/
void cmd(U32 befehl){
    wr32(RAM_CMD+cli,befehl);
    cli+=4;
}
/* [] END OF FILE */


