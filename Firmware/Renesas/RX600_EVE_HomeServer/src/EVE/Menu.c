/*
 * Menu.c
 *
 *  Created on: 26.01.2019
 *      Author: Besitzer
 */

/************************************************************************
 * Includes
 ***********************************************************************/
#include "Menu.h"
/************************************************************************
 * Informations
 ***********************************************************************/

/************************************************************************
 * Local Funtions
 ***********************************************************************/

/************************************************************************
 * Global Variable
 ***********************************************************************/

/************************************************************************
 * Constant
 ***********************************************************************/

#define Menu_Banner_End			23

/************************************************************************
*! \fn			void MainMenu(char* BannerString)
*  \brief		Print the Banner and an Information rom String
*  \param		char *BannerString
*  \exception	none
*  \return		none
************************************************************************/
void MainMenu(char* BannerString){

	//Draw Menu band
	cli = 0;
	cmd_dlstart();
	cmd(CLEAR(1, 1, 1));
	cmd(BEGIN(RECTS));
	    cmd(VERTEX2II(0, 23, 0, 0));
	    cmd(VERTEX2II(480, 0, 0, 0));
	    cmd_gradient(480, 14, 0xFFFF00, 480, 0, 0xFF0000);
	cmd(END());
	//Set Color Black
	cmd(COLOR_RGB(0, 0, 0));
	cmd_text(302, 1, 22, 0, BannerString);
	//Draw Line
	cmd(BEGIN(LINES));
		cmd(COLOR_RGB(0, 0, 0));
	    cmd(VERTEX2II(0, 26, 0, 0));
	    cmd(VERTEX2II(480, 26, 0, 0));
	cmd(END());

	//Draw Menu band
	cmd(BEGIN(RECTS));
	    cmd(COLOR_RGB(170, 255, 255));
	    cmd(VERTEX2II(0, 25, 0, 0));
	    cmd(VERTEX2II(480, 272, 0, 0));
	cmd(END());

	cmd(COLOR_RGB(0, 0, 0));
	cmd(TAG(Clock_Tag));
	cmd_button(17, 47, 90, 31, 27, 0, "Uhrzeit");

	refresh_display();

}
