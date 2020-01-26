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
*! \file:                   copro_commands.h
*  \projekt:                PSOC_EVE_Sensorboard
*  \created on:             2020-02-12
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief                   Graphic library
                            EVE coprocessor commands
***********************************************************************/

#ifndef _copro_commands_h_
#define _copro_commands_h_

/***********************************************************************
 * Includes
 **********************************************************************/
#include "EVE_Config.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array

/***********************************************************************
 * Global Variable
 **********************************************************************/

/***********************************************************************
 * Constant
 **********************************************************************/

    
/***********************************************************************
 * Functions
 **********************************************************************/
void cmd_clock(int x, int y, int r, U16 options, U16 h, U16 m, U16 s, U16 ms);
void cmd_gauge(int x, int y, int r, U16 options, U16 major, U16 minor, U16 val, U16 range);
void cmd_text(int x, int y, int font, int options, char* s);
void cmd_number(int x, int y, int font, int options, U32 n);
void cmd_track(int x, int y, int w, int h, int tag);
void cmd_bgcolor(U32 x);
void cmd_fgcolor(U32 x);
void cmd_button(int x,int y, int w, int h, int font, int options, char* s);
void cmd_dial(int x, int y, int r, U16 options, U16 val);
void cmd_keys(int x,int y, int w, int h, int font, int options, char* s);
void cmd_gradient(int x0, int y0, U32 rgb0, int x1, int y1, U32 rgb1);
void cmd_progress(int x, int y, int w, int h, U16 options, U16 val, U16 range);
void cmd_scrollbar(int x, int y, int w, int h, int options, int val, int size, int range);
void cmd_slider(int x, int y, int w, int h, U16 options, U16 val, U16 range);
void cmd_spinner(int x, int y, U16 style, U16 scale);
void cmd_coldstart(void);
void cmd_dlstart(void);
void cmd_stop(void);
void cmd_setfont(U32 font, U32 ptr);
void cmd_swap(void);
void cmd_logo(void);
void cmd_memzero(U32 ptr,U32 num);
void cmd_calibrate(void);
void cmd(U32 befehl);

#endif /* _copro_commands.h_ */

/* [] END OF FILE */
