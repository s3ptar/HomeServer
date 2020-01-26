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
#ifndef __EVE_HAL_H_
#define __EVE_HAL_H_
/***********************************************************************
 * Includes
 **********************************************************************/
#include "EVE_Config.h"
#include "nPD_EVE.h"
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array

/***********************************************************************
 * Global Variable
 **********************************************************************/
extern unsigned int dli,cli;
/***********************************************************************
 * Constant
 **********************************************************************/

#define INCR                            3UL
#define ZERO                            0UL
#define GEQUAL                          4UL
#define ALWAYS                          7UL
#define KEEP                            1UL

#define OPT_CENTER                      1536UL
#define OPT_CENTERX                     512UL
#define OPT_CENTERY                     1024UL
#define OPT_FLAT                        256UL
#define OPT_MONO                        1UL
#define OPT_NOBACK                      4096UL
#define OPT_NODL                        2UL
#define OPT_NOHANDS                     49152UL
#define OPT_NOHM                        16384UL
#define OPT_NOPOINTER                   16384UL
#define OPT_NOSECS                      32768UL
#define OPT_NOTICKS                     8192UL
#define OPT_RIGHTX                      2048UL
#define OPT_SIGNED                      256UL
#define PALETTED                        8UL
#define FTPOINTS                        2UL

#define INDEX_WRITE                     (0x70 | DisplayID)
#define REG_WRITE                       (0x72 | DisplayID)
#define REG_READ                        (0x73 | DisplayID)
#define INDEX_WRITE_WORD                ((INDEX_WRITE)<<8)
#define REG_WRITE_WORD                  ((REG_WRITE)<<8)
#define REG_READ_WORD                   ((REG_READ)<<8)

#define ChipOn                          1      
#define ChipOff                         0 

/***********************************************************************
 * Functions
 **********************************************************************/
void wr8(U32 address, U8 data);
void wr16(U32 address, U16 data);
void wr32(U32 address, U32 data);
U8 rd8(U32 address);
U16 rd16(U32 address);
U32 rd32(U32 address);
void init_EVE_IO(void);
void EVE_Power(U8 on_off);
void host_command(U8 cmd);

#endif /* _EVE_HAL_H_ */

/* [] END OF FILE */
