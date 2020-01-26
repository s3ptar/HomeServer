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

#if !defined(PSOC_EVE_AUDIO_H)
#define PSOC_EVE_AUDIO_H
    
#include <cytypes.h>  
 
/*******************************************************************************
*   Function prototypes.
*******************************************************************************/

#ifdef USE_GPIO1_AUDIO          // Conditional compilation. More info in PSocEVE_Config.h file.
    void FT_Audio_Mute();
    void FT_Audio_Unmute();
#endif

void FT_Sound_Volume(uint8 volume);
void FT_Sound_Play(uint8 sound, uint8 pitch);
void FT_Sound_Stop();    
    
#endif /* End PSOC_EVE_AUDIO_H */   

/* [] END OF FILE */
