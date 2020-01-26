/*******************************************************************************
* File Name: PSoCEve_Config.h  
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

#if !defined(PSOC_EVE_CONFIG_H) /* Pins SPI_EVE_miso_s_H */
#define PSOC_EVE_CONFIG_H
    
#define SPI_NAME    SPI
#define SPI_SS_NAME SPI_SS
    
#define EVE_FT800
//#define EVE_FT810    
    
    
//#define LCD_QVGA
#define LCD_WQVGA 
//#define LCD_WVVGA    
    
// LCD display parameters
#ifdef LCD_QVGA			            // QVGA display parameters
    #define LCDWIDTH    320				    // Active width of LCD display
    #define LCDHEIGHT   240					// Active height of LCD display
    #define LCDHCYCLE   408					// Total number of clocks per line
    #define LCDHOFFSET  70				    // Start of active line
    #define LCDHSYNC0   0					// Start of horizontal sync pulse
    #define LCDHSYNC1   10				    // End of horizontal sync pulse
    #define LCDVCYCLE   263					// Total number of lines per screen
    #define LCDVOFFSET  13				    // Start of active screen
    #define LCDVSYNC0   0					// Start of vertical sync pulse
    #define LCDVSYNC1   2					// End of vertical sync pulse
    #define LCDPCLK     8					// Pixel Clock
    #define LCDSWIZZLE  2					// Define RGB output pins
    #define LCDPCLKPOL  0					// Define active edge of PCLK
	#define LCDCSPREAD  1
	#define LCDDITHER   1        
#endif

#ifdef LCD_WQVGA		            // WQVGA display parameters
    #define LCDWIDTH    480				    // Active width of LCD display
    #define LCDHEIGHT   272					// Active height of LCD display
    #define LCDHCYCLE   548					// Total number of clocks per line
    #define LCDHOFFSET  43				    // Start of active line
    #define LCDHSYNC0   0					// Start of horizontal sync pulse
    #define LCDHSYNC1   41				    // End of horizontal sync pulse
    #define LCDVCYCLE   292					// Total number of lines per screen
    #define LCDVOFFSET  12				    // Start of active screen
    #define LCDVSYNC0   0					// Start of vertical sync pulse
    #define LCDVSYNC1   10					// End of vertical sync pulse
    #define LCDPCLK     5					// Pixel Clock
    #define LCDSWIZZLE  0					// Define RGB output pins
    #define LCDPCLKPOL  1					// Define active edge of PCLK   
	#define LCDCSPREAD  1
	#define LCDDITHER   1    
#endif

#ifdef LCD_WVVGA		            // WQVGA display parameters
    #define LCDWIDTH        800				    // Active width of LCD display
    #define LCDHEIGHT       480					// Active height of LCD display
    #define LCDHCYCLE       928					// Total number of clocks per line
    #define LCDHOFFSET      88				    // Start of active line
    #define LCDHSYNC0       0					// Start of horizontal sync pulse
    #define LCDHSYNC1       48				    // End of horizontal sync pulse
    #define LCDVCYCLE       525					// Total number of lines per screen
    #define LCDVOFFSET      32				    // Start of active screen
    #define LCDVSYNC0       0					// Start of vertical sync pulse
    #define LCDVSYNC1       3					// End of vertical sync pulse
    #define LCDPCLK         2					// Pixel Clock
    #define LCDSWIZZLE      0					// Define RGB output pins
    #define LCDPCLKPOL      1					// Define active edge of PCLK  
	#define LCDCSPREAD      0
	#define LCDDITHER       1    
#endif

/* Uncomment USE_TOUCHPANEL line to include touch functions.
   If not using a touch panel, commenting this line you can save some program memory space.
*/
#define USE_TOUCHPANEL

/* Uncomment USE_AUDIO line to include audio functions.
   If not using audio, commenting this line you can save some program memory space.
*/
#define USE_AUDIO

/* Development modules from FTDI use GPIO1 as output to control the audio 
   amplifier chip. If your design doesn´t use this, comment next line. 
*/
#define USE_GPIO1_AUDIO

    
#endif /* End PSOC_EVE_CONFIG_H */ 

/* [] END OF FILE */
