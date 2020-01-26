/*
 * Menu.h
 *
 *  Created on: 26.01.2019
 *      Author: Besitzer
 */

#ifndef EVE_MENU_H_
#define EVE_MENU_H_


/*****************************************************************************
 * Includes
 ****************************************************************************/
#include "stdint.h"
#include "copro_commands.h"
#include "FT_Gpu.h"
#include "Eve.h"
/*****************************************************************************
 * Definitions
 ****************************************************************************/

//color code for Banner
#define Start_Banner_Okay		0x00AA00
#define End_Banner_Okay			0x00FF00
#define Start_Banner_Warning	0xFFFF00
#define	End_Banner_Warning		0xFF5500
#define Start_Banner_Error		0xBC0000
#define	End_Banner_Error		0xFF0000

//Tag Definition

#define Clock_Tag				1

/*****************************************************************************
 * Global Variable
 ****************************************************************************/

/*****************************************************************************
 * Functions
 ****************************************************************************/

void MainMenu(char* BannerString);

#endif /* EVE_MENU_H_ */
