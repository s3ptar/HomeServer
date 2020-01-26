/*******************************************************************************
* File Name: nPD_EVE.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_nPD_EVE_H) /* Pins nPD_EVE_H */
#define CY_PINS_nPD_EVE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "nPD_EVE_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 nPD_EVE__PORT == 15 && ((nPD_EVE__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    nPD_EVE_Write(uint8 value);
void    nPD_EVE_SetDriveMode(uint8 mode);
uint8   nPD_EVE_ReadDataReg(void);
uint8   nPD_EVE_Read(void);
void    nPD_EVE_SetInterruptMode(uint16 position, uint16 mode);
uint8   nPD_EVE_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the nPD_EVE_SetDriveMode() function.
     *  @{
     */
        #define nPD_EVE_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define nPD_EVE_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define nPD_EVE_DM_RES_UP          PIN_DM_RES_UP
        #define nPD_EVE_DM_RES_DWN         PIN_DM_RES_DWN
        #define nPD_EVE_DM_OD_LO           PIN_DM_OD_LO
        #define nPD_EVE_DM_OD_HI           PIN_DM_OD_HI
        #define nPD_EVE_DM_STRONG          PIN_DM_STRONG
        #define nPD_EVE_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define nPD_EVE_MASK               nPD_EVE__MASK
#define nPD_EVE_SHIFT              nPD_EVE__SHIFT
#define nPD_EVE_WIDTH              1u

/* Interrupt constants */
#if defined(nPD_EVE__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in nPD_EVE_SetInterruptMode() function.
     *  @{
     */
        #define nPD_EVE_INTR_NONE      (uint16)(0x0000u)
        #define nPD_EVE_INTR_RISING    (uint16)(0x0001u)
        #define nPD_EVE_INTR_FALLING   (uint16)(0x0002u)
        #define nPD_EVE_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define nPD_EVE_INTR_MASK      (0x01u) 
#endif /* (nPD_EVE__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define nPD_EVE_PS                     (* (reg8 *) nPD_EVE__PS)
/* Data Register */
#define nPD_EVE_DR                     (* (reg8 *) nPD_EVE__DR)
/* Port Number */
#define nPD_EVE_PRT_NUM                (* (reg8 *) nPD_EVE__PRT) 
/* Connect to Analog Globals */                                                  
#define nPD_EVE_AG                     (* (reg8 *) nPD_EVE__AG)                       
/* Analog MUX bux enable */
#define nPD_EVE_AMUX                   (* (reg8 *) nPD_EVE__AMUX) 
/* Bidirectional Enable */                                                        
#define nPD_EVE_BIE                    (* (reg8 *) nPD_EVE__BIE)
/* Bit-mask for Aliased Register Access */
#define nPD_EVE_BIT_MASK               (* (reg8 *) nPD_EVE__BIT_MASK)
/* Bypass Enable */
#define nPD_EVE_BYP                    (* (reg8 *) nPD_EVE__BYP)
/* Port wide control signals */                                                   
#define nPD_EVE_CTL                    (* (reg8 *) nPD_EVE__CTL)
/* Drive Modes */
#define nPD_EVE_DM0                    (* (reg8 *) nPD_EVE__DM0) 
#define nPD_EVE_DM1                    (* (reg8 *) nPD_EVE__DM1)
#define nPD_EVE_DM2                    (* (reg8 *) nPD_EVE__DM2) 
/* Input Buffer Disable Override */
#define nPD_EVE_INP_DIS                (* (reg8 *) nPD_EVE__INP_DIS)
/* LCD Common or Segment Drive */
#define nPD_EVE_LCD_COM_SEG            (* (reg8 *) nPD_EVE__LCD_COM_SEG)
/* Enable Segment LCD */
#define nPD_EVE_LCD_EN                 (* (reg8 *) nPD_EVE__LCD_EN)
/* Slew Rate Control */
#define nPD_EVE_SLW                    (* (reg8 *) nPD_EVE__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define nPD_EVE_PRTDSI__CAPS_SEL       (* (reg8 *) nPD_EVE__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define nPD_EVE_PRTDSI__DBL_SYNC_IN    (* (reg8 *) nPD_EVE__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define nPD_EVE_PRTDSI__OE_SEL0        (* (reg8 *) nPD_EVE__PRTDSI__OE_SEL0) 
#define nPD_EVE_PRTDSI__OE_SEL1        (* (reg8 *) nPD_EVE__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define nPD_EVE_PRTDSI__OUT_SEL0       (* (reg8 *) nPD_EVE__PRTDSI__OUT_SEL0) 
#define nPD_EVE_PRTDSI__OUT_SEL1       (* (reg8 *) nPD_EVE__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define nPD_EVE_PRTDSI__SYNC_OUT       (* (reg8 *) nPD_EVE__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(nPD_EVE__SIO_CFG)
    #define nPD_EVE_SIO_HYST_EN        (* (reg8 *) nPD_EVE__SIO_HYST_EN)
    #define nPD_EVE_SIO_REG_HIFREQ     (* (reg8 *) nPD_EVE__SIO_REG_HIFREQ)
    #define nPD_EVE_SIO_CFG            (* (reg8 *) nPD_EVE__SIO_CFG)
    #define nPD_EVE_SIO_DIFF           (* (reg8 *) nPD_EVE__SIO_DIFF)
#endif /* (nPD_EVE__SIO_CFG) */

/* Interrupt Registers */
#if defined(nPD_EVE__INTSTAT)
    #define nPD_EVE_INTSTAT            (* (reg8 *) nPD_EVE__INTSTAT)
    #define nPD_EVE_SNAP               (* (reg8 *) nPD_EVE__SNAP)
    
	#define nPD_EVE_0_INTTYPE_REG 		(* (reg8 *) nPD_EVE__0__INTTYPE)
#endif /* (nPD_EVE__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_nPD_EVE_H */


/* [] END OF FILE */
