/*
 * board.h
 *
 *  Created on: 31.10.2018
 *      Author: Besitzer
 */

#ifndef BOARD_H_
#define BOARD_H_



/*****************************************************************************
 * Includes
 ****************************************************************************/
#include "iodefine.h"
#include "stdint.h"


/*****************************************************************************
 * Definitions
 ****************************************************************************/
#define LED1_DDR	    PORTC.PDR.BIT.B5
#define LED1_Port	    PORTC.PODR.BIT.B5
#define LED1_Port	    PORTC.PODR.BIT.B5
#define EVE_CS_on  		PORT2.PODR.BIT.B2=0
#define EVE_CS_off 		PORT2.PODR.BIT.B2=1
#define EVE_Power_off  	PORT3.PODR.BIT.B3=0
#define EVE_Power_on   	PORT3.PODR.BIT.B3=1

//http://www.farnell.com/datasheets/1509864.pdf
//i2C Page 92
//P15 = RXD1/SCK3/SMISO1/SSCL1/CRX1-DS - IRQ5
//P16 = TXD1/RXD3/SMOSI1/SMISO3/SSDA1/SSCL3/MOSIA/SCL2-DS/IERXD/USB0_VBUS/USB0_VBUSEN/USB0_OVRCURB - IRQ6
//RIIC1
/* Defines the address the EEPROM device responds to on the IIC bus */
#define SIM_EEPROM_DEVICE_ADDRESS        0xA0
/* Defines the location the simulated memory will be stored on the MCU */
#define SIM_EEPROM_LOCATION              0x00002000

/************************************************************************
 * Global Variable
 ***********************************************************************/
extern unsigned int ms,sek,hr,min;
extern unsigned long int ms_counter,ms_counter2;
extern volatile char day,month,year,day_old,month_old,year_old;

/*****************************************************************************
 * Functions
 ****************************************************************************/
void init_board( void );
void SysTick_Handler (void);
void delay_ms(uint32_t delay_time);
void init_IO(void);
void Init_EEPROM_Slave(void);


#endif /* BOARD_H_ */
