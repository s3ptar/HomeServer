/* THIS SAMPLE CODE IS PROVIDED AS IS        */
/* AND IS SUBJECT TO ALTERATIONS. GLYN       */
/* ACCEPTS NO RESPONSIBILITY OR LIABILITY    */
/* FOR ANY ERRORS OR                         */
/* ELIGIBILITY FOR ANY PURPOSES.             */
/* (C) GLYN GmbH & Co. KG                    */
/*********************************************
Project : copro_commands.h
          Graphic library
          EVE coprocessor commands
Version : 0.2 Status: Demo
Date    : 29.11.2013

Author  : P.Doerwald / GLYN GmbH & Co. KG

Releases: V0.2

Info    : O
*********************************************/

#include "stdint.h"

void cmd_clock(int x, int y, int r, uint16_t options, uint16_t h, uint16_t m, uint16_t s, uint16_t ms);
void cmd_gauge(int x, int y, int r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range);
void cmd_text(int x, int y, int font, int options, char* s);
void cmd_number(int x, int y, int font, int options, uint32_t n);
void cmd_track(int x, int y, int w, int h, int tag);
void cmd_bgcolor(uint32_t x);
void cmd_fgcolor(uint32_t x);
void cmd_button(int x,int y, int w, int h, int font, int options, char* s);
void cmd_dial(int x, int y, int r, uint16_t options, uint16_t val);
void cmd_keys(int x,int y, int w, int h, int font, int options, char* s);
void cmd_gradient(int x0, int y0, uint32_t rgb0, int x1, int y1, uint32_t rgb1);
void cmd_progress(int x, int y, int w, int h, uint16_t options, uint16_t val, uint16_t range);
void cmd_scrollbar(int x, int y, int w, int h, int options, int val, int size, int range);
void cmd_slider(int x, int y, int w, int h, uint16_t options, uint16_t val, uint16_t range);
void cmd_spinner(int x, int y, uint16_t style, uint16_t scale);
void cmd_coldstart(void);
void cmd_dlstart(void);
void cmd_stop(void);
void cmd_setfont(uint32_t font, uint32_t ptr);
void cmd_swap();
void cmd_logo(void);
void cmd_memzero(uint32_t ptr,uint32_t num);
void cmd_calibrate(void);
void cmd();
