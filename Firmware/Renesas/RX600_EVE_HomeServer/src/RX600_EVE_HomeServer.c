/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  RX600_EVE_HomeServer                   */
/*      FILE         :  RX600_EVE_HomeServer.c                 */
/*      DESCRIPTION  :  Main Program                           */
/*                                                             */
/*      This file was generated by e2 studio.                  */
/*                                                             */
/***************************************************************/

#include <iodefine.h>
#include "board.h"
#include "stdint.h"
#include "Menu.h"

#ifdef CPPAPP
//Initialize global constructors
extern void __main()
{
  static int initialized;
  if (! initialized)
    {
      typedef void (*pfunc) ();
      extern pfunc __ctors[];
      extern pfunc __ctors_end[];
      pfunc *p;

      initialized = 1;
      for (p = __ctors_end; p > __ctors; )
    (*--p) ();

    }
}
#endif 

unsigned int lum=100;


int main(void) {

	char StringForBanner[19];
	uint8_t time,tag;
	init_board();
	EVE_BootUp();
	video_timing();
    play_sound(0x56,47,60);
	//Touch();

	LED1_DDR = 1;
	LED1_Port = 1;
	time = sek;
	//Logo();

    while(1) {

    	delay_ms(500);
    	LED1_Port = 1;
    	delay_ms(500);
    	LED1_Port = 0;

    	//Display_test2(hr, min, sek, tag);
    	sprintf(StringForBanner,"%02d:%02d:%02d %02d.%02d.%04d",hr, min, sek, 1,11,2018);
    	MainMenu(&StringForBanner);

	// TODO: add application code here
    }
}