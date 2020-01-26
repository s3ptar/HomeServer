## PSoC Eve: Open source library to control FTDI EVE chips using Cypress PSoC microcontrollers. ##

Work in progress and in a very early stage.
Library will support FT800 and FT810 chips. No support for other chips (at least for now) because i have no access to other FTDI chips for testing.

### Repository. ###
* PSoC_Eve_Library: the library.
* PSoC4Eve: draft. Code for testing during development. Changing every day.
* Examples:
	* PSoC4_Eve_Sketch: This is a port of the FT_App_Sketch demo from FTDI with some modifications. Tested with development kits mentioned in “Development environment” section.

Note.
 In file “PsoCEve_Config.h”:

	- Uncomment line “#define FT_800” and comment line “#define FT_810” if using chip FT800.
	- Comment line “#define FT_800” and uncomment line “#define FT_810” if using chip FT800.

	- Uncomment line “#define LCD_WQVGA” and comment other LCD definition lines if using 4.3” display.
	- Uncomment line “#define LCD_WVVGA” and comment other LCD definition lines if using 5” display.
	- LCD_QVGA (3.5” display) untested, i don´t have this display.

### Development environment. ###
* PSoC Creator 3.3
* Cypress development kit CY8CKIT-042.
* FTDI development kit VM800B43 (FT800 chip & 4.3” display).
* FTDI development kit VM810C50A-D (FT810 chip & 5” display).

MORE INFO ASAP.







