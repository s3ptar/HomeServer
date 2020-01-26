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
#include "project.h"
#include "timing.h"
#include "EVE.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    init_Delay();
    EVE_BootUp();
	video_timing();
  
    play_sound(0x56,47,60);
    Touch();
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
