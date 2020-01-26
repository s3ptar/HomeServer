/*******************************************************************************
* File Name: PSoCEve.c 
* Version 0.1 Alpha
*
* Description:
*  PSoCEveLibrary project.
*  Library to easy the use of the FT EVE graphic chips from FTDI.
*  Actually in development and in a very early stage there is support only
*  for the FT800 chip with Cypress PSOC4 family microcontrollers.
*  It is planned to had support for FT801 and newer TF81x chips, also for
*  other PSOC families (PSOC3 and PSCO5).
*
* Note:
*
********************************************************************************
* Copyright (c) 2015 Jesús Rodríguez Cacabelos
* This library is dual licensed under the MIT and GPL licenses.
* http:
*******************************************************************************/

#include <project.h>
#include "PSoCEve.h"
#include "PSoCEve_Hal.h"

/*******************************************************************************
*   Function prototypes.
*******************************************************************************/

extern void FT_Register_Write(uint32 everegister, uint32 value);
extern uint32 FT_Register_Read(uint32 everegister);


/*******************************************************************************
* Function Name: FT_Register_Write
********************************************************************************
*
* Summary:
*  Write value to FT chip register.
*
* Parameters:
*  everegister:    register to be written.
*  value:          value.  
*
* Return:
*  none
*
*******************************************************************************/
void FT_Register_Write(uint32 everegister, uint32 value)
{
    FT_Write_UINT32(everegister | MEMORY_WRITE, value);
}

/*******************************************************************************
* Function Name: FT_Register_Read
********************************************************************************
*
* Summary:
*  Read value from FT chip register.
*
* Parameters:
*  everegister:    register to read.
*
* Return:
*  uint32 type value.
*
*******************************************************************************/
uint32 FT_Register_Read(uint32 everegister)
{
    return FT_Read_UINT32(everegister | MEMORY_READ);
}

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
* Function Name: FT_Init
********************************************************************************
*
* Summary:
*  Initializes the FT chip with values from PSoCEve_Config.h file.
*  After initialization, display is off, touch panel is disabled and audio
*  is disabled.
*  To enable display, touch panel and audio call proper functions.
*  As stated in FT datasheet, until the FT chip is initializad, maximun SPI bus
*  speed is 12Mhz. After initializacion, SPI bus speed can be incremented up to
*  30Mhz. This functions doesn´t change the SPI bus communication speed. 
*
* Parameters:
*  None
*
* Return:
*  0 if initialization fails. 1 if initialization is OK.
*
*******************************************************************************/

uint8 FT_Init()
{
    uint8 t = 0;
    
    // Force a hardware reset of EVE chip using PD_N pin.
    PD_N_Write(0); CyDelay(100); PD_N_Write(1); CyDelay(200);
    
    CyDelay(50);
    FTCommandWrite(FT800_ACTIVE);            // Start FT800
    CyDelay(50);	
    // Initialize EVE chip. Max SPI speed before the chip is initialized is 11Mhz.
    FTCommandWrite(FT800_CLKEXT);			// Set FT800 for external clock
    CyDelay(50);
    FTCommandWrite(FT800_ACTIVE);            // Start FT800
    CyDelay(50);	
	

    #if defined EVE_FT800
        FTCommandWrite(FT800_CLK48M);			// Set FT800 for 48MHz PLL
    #elif defined EVE_FT810
        FTCommandWrite(FT800_CLKSEL);
    #endif
    
    CyDelay(200);	
    FTCommandWrite(FT800_CORERST);			// Set FT800 for 48MHz PLL
    CyDelay(200);
    
    // Read ID register. If we don¨t get 0x7C something is bad.
    // if (EVE_Memory_Read_Byte(REG_ID) != 0x7C) return 0;
    if (FT_Register_Read(REG_ID) != 0x7C) return 0;
    
    // At startup, PCLK (pixel clock) and PWM_DUTY (used for backlight) are programmed to 0.
    //      Display is off until user turns it on.
    FT_Register_Write(REG_PCLK, 0);
    FT_Register_Write(REG_PWM_DUTY, 0);	
    
    // Continue initializing registers with values from configuration header file.
    FT_Register_Write(REG_HSIZE, LCDWIDTH);	
    FT_Register_Write(REG_VSIZE, LCDHEIGHT);
    FT_Register_Write(REG_HCYCLE, LCDHCYCLE);
    FT_Register_Write(REG_HOFFSET, LCDHOFFSET);
    FT_Register_Write(REG_HSYNC0, LCDHSYNC0);
    FT_Register_Write(REG_HSYNC1, LCDHSYNC1);
    FT_Register_Write(REG_VCYCLE, LCDVCYCLE);
    FT_Register_Write(REG_VOFFSET, LCDVOFFSET);
    FT_Register_Write(REG_VSYNC0, LCDVSYNC0);
    FT_Register_Write(REG_VSYNC1, LCDVSYNC1);
    FT_Register_Write(REG_SWIZZLE, LCDSWIZZLE);
    FT_Register_Write(REG_PCLK_POL, LCDPCLKPOL);
    
    FT_Register_Write(REG_VOL_PB, 0);               // turn recorded audio volume down
    FT_Register_Write(REG_VOL_SOUND, 0);            // turn synthesizer volume down
    FT_Register_Write(REG_SOUND, 0x0060);           // set synthesizer to mute
    
    /* Configure GPIO1 to control audio amplifier chip. */
    #ifdef USE_GPIO1_AUDIO
        t = FT_Register_Read(REG_GPIO_DIR);   
        t |= 0x02;                                  // GIPO1, as output.
        FT_Register_Write(REG_GPIO_DIR, t);
        
        t = FT_Register_Read(REG_GPIO);             
        t &= 0xFD;                                  // = 0, shutdown amplifier.
        FT_Register_Write(REG_GPIO, t);
    #endif
              
    return 1;
}

//uint8 FT_Init()
//{
//    uint8 t = 0;
//    
//    // Force a hardware reset of EVE chip using PD_N pin.
//    PD_N_Write(0); CyDelay(10); PD_N_Write(1); CyDelay(20);
//    
//    // Initialize EVE chip. Max SPI speed before the chip is initialized is 11Mhz.
//    FTCommandWrite(FT800_ACTIVE);            // Start FT800
//    CyDelay(5);	
//    FTCommandWrite(FT800_CLKEXT);			// Set FT800 for external clock
//    CyDelay(5);	
//    FTCommandWrite(FT800_CLK48M);			// Set FT800 for 48MHz PLL
//    CyDelay(5);	
//    FTCommandWrite(FT800_CORERST);			// Set FT800 for 48MHz PLL
//    CyDelay(5);
//    
//    // Read ID register. If we don¨t get 0x7C something is bad.
//    //if (EVE_Memory_Read_Byte(REG_ID) != 0x7C) return 0;
//    if (FT_Register_Read(REG_ID) != 0x7C) return 0;
//    
//    // At startup, PCLK (pixel clock) and PWM_DUTY (used for backlight) are programmed to 0.
//    //      Display is off until user turns it on.
//    FT_Register_Write(REG_PCLK, 0);
//    FT_Register_Write(REG_PWM_DUTY, 0);	
//    
//    // Continue initializing registers with values from configuration header file.
//    FT_Register_Write(REG_HSIZE, LCDWIDTH);	
//    FT_Register_Write(REG_VSIZE, LCDHEIGHT);
//    FT_Register_Write(REG_HCYCLE, LCDHCYCLE);
//    FT_Register_Write(REG_HOFFSET, LCDHOFFSET);
//    FT_Register_Write(REG_HSYNC0, LCDHSYNC0);
//    FT_Register_Write(REG_HSYNC1, LCDHSYNC1);
//    FT_Register_Write(REG_VCYCLE, LCDVCYCLE);
//    FT_Register_Write(REG_VOFFSET, LCDVOFFSET);
//    FT_Register_Write(REG_VSYNC0, LCDVSYNC0);
//    FT_Register_Write(REG_VSYNC1, LCDVSYNC1);
//    FT_Register_Write(REG_SWIZZLE, LCDSWIZZLE);
//    FT_Register_Write(REG_PCLK_POL, LCDPCLKPOL);
//    
//    FT_Register_Write(REG_VOL_PB, 0);               // turn recorded audio volume down
//    FT_Register_Write(REG_VOL_SOUND, 0);            // turn synthesizer volume down
//    FT_Register_Write(REG_SOUND, 0x0060);           // set synthesizer to mute
//    
//    /* Configure GPIO1 to control audio amplifier chip. */
//    #ifdef USE_GPIO1_AUDIO
//        t = FT_Register_Read(REG_GPIO_DIR);   
//        t |= 0x02;                                  // GIPO1, as output.
//        FT_Register_Write(REG_GPIO_DIR, t);
//        
//        t = FT_Register_Read(REG_GPIO);             
//        t &= 0xFD;                                  // = 0, shutdown amplifier.
//        FT_Register_Write(REG_GPIO, t);
//    #endif
//              
//    return 1;
//}


/*******************************************************************************
* Function Name: FT_Display_ON
********************************************************************************
*
* Summary:
*  Enable display. Put DISP signal high and start the FT clock. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

void FT_Display_ON()
{
    unsigned char gpio = FT_Register_Read(REG_GPIO);        // Read actual value of GPIO register.

    FT_Register_Write(REG_GPIO, (gpio | 0x80));			    // Set bit 7 of GPIO register (DISP signal).
    FT_Register_Write(REG_PCLK, LCDPCLK);			        // Start clock.
}

/*******************************************************************************
* Function Name: FT_Display_OFF
********************************************************************************
*
* Summary:
*  Disable display. Put DISP signal low and stop the FT clock. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

void FT_Display_OFF()
{
    unsigned char gpio = FT_Register_Read(REG_GPIO);        // Read actual value of GPIO register.
    
    FT_Register_Write(REG_GPIO, (gpio & 0x70));			    // Clear bit 7 of GPIO register (DISP signal).
    FT_Register_Write(REG_PCLK, LCDPCLK);			        // Stop clock.
}


