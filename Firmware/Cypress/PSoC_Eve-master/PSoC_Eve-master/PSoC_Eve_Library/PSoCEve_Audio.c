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

#include "PSoCEve.h" 
#include "PSoCEve_Audio.h"

    /* Conditional compilation. For more info look at PSoCEve_config.h */
#ifdef USE_GPIO1_AUDIO
    
    
    /*******************************************************************************
    * Function Name: FT_Audio_Mute
    ********************************************************************************
    *
    * Summary:
    *  Put GPIO1 pin of FT chip to low state. This puts the audio amplifier in FTDI
    *  development kits in shutdown condition.
    *
    * Parameters:
    *  none
    *
    * Return:
    *  none
    *
    *******************************************************************************/    
    void FT_Audio_Mute()
    {
        uint8 t = 0;
        
        t = FT_Register_Read(REG_GPIO);             
        t &= 0xFD;                                  
        FT_Register_Write(REG_GPIO, t); 
    }
    
    /*******************************************************************************
    * Function Name: FT_Audio_Unmute
    ********************************************************************************
    *
    * Summary:
    *  Put GPIO1 pin of FT chip to high state. This puts the audio amplifier in FTDI
    *  development kits in poer on  condition.
    *
    * Parameters:
    *  none
    *
    * Return:
    *  none
    *
    *******************************************************************************/      
    void FT_Audio_Unmute()
    {
        uint8 t = 0;
        
        t = FT_Register_Read(REG_GPIO);             
        t |= 0x02;                                  
        FT_Register_Write(REG_GPIO, t);    
    }
    
#endif

/*******************************************************************************
* Function Name: FT_Sound_Volume
********************************************************************************
*
* Summary:
*  Set volume level for sounds.
*
* Parameters:
*  volume:  volume level. Maximun 255.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Sound_Volume(uint8 volume)
{
    FT_Register_Write(REG_VOL_SOUND, volume);
}

/*******************************************************************************
* Function Name: FT_Sound_Play
********************************************************************************
*
* Summary:
*  Play sound.
*
* Parameters:
*  sound:   sound to be played.
*  pitch:   pitch. 
*  Look at FT datsheet for more info.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Sound_Play(uint8 sound, uint8 pitch)
{
    FT_Register_Write(REG_SOUND, sound | (pitch << 16));
    FT_Register_Write(REG_PLAY, 1);
}

/*******************************************************************************
* Function Name: FT_Sound_Stop
********************************************************************************
*
* Summary:
*  Stops sound.
*
* Parameters:
*  none
*
* Return:
*  none
*
*******************************************************************************/
void FT_Sound_Stop()
{
    FT_Register_Write(REG_SOUND, 0x60);
    FT_Register_Write(REG_PLAY, 1);    
}


/* [] END OF FILE */
