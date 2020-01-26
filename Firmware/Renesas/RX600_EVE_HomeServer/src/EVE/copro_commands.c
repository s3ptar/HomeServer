/* THIS SAMPLE CODE IS PROVIDED AS IS        */
/* AND IS SUBJECT TO ALTERATIONS. GLYN       */
/* ACCEPTS NO RESPONSIBILITY OR LIABILITY    */
/* FOR ANY ERRORS OR                         */
/* ELIGIBILITY FOR ANY PURPOSES.             */
/* (C) GLYN GmbH & Co. KG                    */
/*********************************************
Project : copro_commands.c
          Graphic library
          EVE coprocessor commands
Version : 0.2 Status: Demo
Date    : 29.11.2013

Author  : P.Doerwald / GLYN GmbH & Co. KG

Releases: V0.2

Info    : O
*********************************************/

#include "EVE_SPI.h" 
#include "copro_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdint.h"


//#define U8              unsigned char
//#define uint16_t        unsigned int
//#define uint32_t        unsigned long int

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

unsigned int dli,cli;


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

void cmd_bgcolor(uint32_t x){

    wr32(RAM_CMD+cli,0xffffff09);
    cli+=4;
    wr32(RAM_CMD+cli,x);
    cli+=4;
}
void cmd_fgcolor(uint32_t x){

    wr32(RAM_CMD+cli,0xffffff0A);
    cli+=4;
    wr32(RAM_CMD+cli,x);
    cli+=4;
}

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
void cmd_dial(int x, int y, int r, uint16_t options, uint16_t val){

    wr32(RAM_CMD+cli,0xffffff2d);
    cli+=4;
    wr32(RAM_CMD+cli,(y<<16)|(x&0xFFFF));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|r));
    cli+=4;
    wr32(RAM_CMD+cli,val);
    cli+=4;
}
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


void cmd_gradient(int x0, int y0, uint32_t rgb0, int x1, int y1, uint32_t rgb1){

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
void cmd_progress(int x, int y, int w, int h, uint16_t options, uint16_t val, uint16_t range){

    wr32(RAM_CMD+cli,0xffffff0f);
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)val<<16)|options));
    cli+=4;
    wr32(RAM_CMD+cli, range);
    cli+=4;
     
}
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
void cmd_slider(int x, int y, int w, int h, uint16_t options, uint16_t val, uint16_t range){

    wr32(RAM_CMD+cli,0xffffff10);
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)val<<16)|options));
    cli+=4;
    wr32(RAM_CMD+cli, range);
    cli+=4;
     
}
void cmd_spinner(int x, int y, uint16_t style, uint16_t scale){

    wr32(RAM_CMD+cli,0xffffff16);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((scale<<16)|style));
    cli+=4;
}


void cmd_clock(int x, int y, int r, uint16_t options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms){

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

void cmd_gauge(int x, int y, int r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range){

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

void cmd_text(int x, int y, int font, int options, char* s){

    wr32(RAM_CMD+cli,0xffffff0C);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|font));
    cli+=4;
    transfer_string(s);
}
void cmd_number(int x, int y, int font, int options, uint32_t n){

    wr32(RAM_CMD+cli,0xffffff2e);
    cli+=4;
    wr32(RAM_CMD+cli,((y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli,((options<<16)|font));
    cli+=4;
    wr32(RAM_CMD+cli,n);
    cli+=4;
}

void cmd_track(int x, int y, int w, int h, int tag){

    wr32(RAM_CMD+cli,0xffffff2C);
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)y<<16)|(x&0xFFFF)));
    cli+=4;
    wr32(RAM_CMD+cli, (((uint32_t)h<<16)|w));
    cli+=4;
    wr32(RAM_CMD+cli, tag);
    cli+=4;
}

void cmd_coldstart(void){

    wr32(RAM_CMD+cli,0xffffff32);
    cli+=4;
}

void cmd_dlstart(void){

    wr32(RAM_CMD+cli,0xffffff00);
    cli+=4;
}
void cmd_stop(){

    wr32(RAM_CMD+cli,0xffffff17);
    cli+=4;

}
void cmd_setfont(uint32_t font, uint32_t ptr){

    wr32(RAM_CMD+cli,0xffffff2b);
    cli+=4;
    wr32(RAM_CMD+cli,font);
    cli+=4;
    wr32(RAM_CMD+cli,ptr);
    cli+=4;
}  

void cmd_swap(){

    wr32(RAM_CMD+cli,0xffffff01);
    cli+=4;

}

void cmd_logo(void){

    wr32(RAM_CMD+cli,0xffffff31);
    cli+=4;
}
void cmd_calibrate(void){

    wr32(RAM_CMD+cli,0xffffff15);
    cli+=4;
}
void cmd_memzero(uint32_t ptr,uint32_t num){

     wr32(RAM_CMD+cli,0xffffff1c);
     cli+=4;
     wr32(RAM_CMD+cli,ptr);
     cli+=4;
     wr32(RAM_CMD+cli,num);
     cli+=4;
}

void cmd(uint32_t befehl){

   wr32(RAM_CMD+cli,befehl);
   cli+=4;
}
