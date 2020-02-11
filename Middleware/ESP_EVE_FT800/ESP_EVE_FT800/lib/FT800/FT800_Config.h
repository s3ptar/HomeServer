/***********************************************************************
*! \file:                   FT800_Config.h
*  \projekt:                FT800_ESP
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/
 
#ifndef _FT800_Config_H_
#define _FT800_Config_H_
 
/***********************************************************************
 * Includes
 **********************************************************************/
 
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
 * Declarations
 **********************************************************************/
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

#ifdef LCD_WQVGA	
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

#ifdef LCD_WVVGA		
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

/***********************************************************************
 * Global Variable
 **********************************************************************/
 
/***********************************************************************
 * Constant
 **********************************************************************/
 
/***********************************************************************
 * Macros
 **********************************************************************/
 
/***********************************************************************
 * Funtions
 **********************************************************************/
 
 
 
 
#endif /* _FT800_Config_H_ */
