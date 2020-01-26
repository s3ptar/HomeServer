/*******************************************************************************
* File Name: EVE_SPI.h
* Version 2.50
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_EVE_SPI_H)
#define CY_SPIM_EVE_SPI_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define EVE_SPI_INTERNAL_CLOCK             (0u)

#if(0u != EVE_SPI_INTERNAL_CLOCK)
    #include "EVE_SPI_IntClock.h"
#endif /* (0u != EVE_SPI_INTERNAL_CLOCK) */

#define EVE_SPI_MODE                       (1u)
#define EVE_SPI_DATA_WIDTH                 (8u)
#define EVE_SPI_MODE_USE_ZERO              (1u)
#define EVE_SPI_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define EVE_SPI_TX_BUFFER_SIZE             (4u)
#define EVE_SPI_RX_BUFFER_SIZE             (4u)
#define EVE_SPI_INTERNAL_TX_INT_ENABLED    (0u)
#define EVE_SPI_INTERNAL_RX_INT_ENABLED    (0u)

#define EVE_SPI_SINGLE_REG_SIZE            (8u)
#define EVE_SPI_USE_SECOND_DATAPATH        (EVE_SPI_DATA_WIDTH > EVE_SPI_SINGLE_REG_SIZE)

#define EVE_SPI_FIFO_SIZE                  (4u)
#define EVE_SPI_TX_SOFTWARE_BUF_ENABLED    ((0u != EVE_SPI_INTERNAL_TX_INT_ENABLED) && \
                                                     (EVE_SPI_TX_BUFFER_SIZE > EVE_SPI_FIFO_SIZE))

#define EVE_SPI_RX_SOFTWARE_BUF_ENABLED    ((0u != EVE_SPI_INTERNAL_RX_INT_ENABLED) && \
                                                     (EVE_SPI_RX_BUFFER_SIZE > EVE_SPI_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
} EVE_SPI_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  EVE_SPI_Init(void)                           ;
void  EVE_SPI_Enable(void)                         ;
void  EVE_SPI_Start(void)                          ;
void  EVE_SPI_Stop(void)                           ;

void  EVE_SPI_EnableTxInt(void)                    ;
void  EVE_SPI_EnableRxInt(void)                    ;
void  EVE_SPI_DisableTxInt(void)                   ;
void  EVE_SPI_DisableRxInt(void)                   ;

void  EVE_SPI_Sleep(void)                          ;
void  EVE_SPI_Wakeup(void)                         ;
void  EVE_SPI_SaveConfig(void)                     ;
void  EVE_SPI_RestoreConfig(void)                  ;

void  EVE_SPI_SetTxInterruptMode(uint8 intSrc)     ;
void  EVE_SPI_SetRxInterruptMode(uint8 intSrc)     ;
uint8 EVE_SPI_ReadTxStatus(void)                   ;
uint8 EVE_SPI_ReadRxStatus(void)                   ;
void  EVE_SPI_WriteTxData(uint8 txData)  \
                                                            ;
uint8 EVE_SPI_ReadRxData(void) \
                                                            ;
uint8 EVE_SPI_GetRxBufferSize(void)                ;
uint8 EVE_SPI_GetTxBufferSize(void)                ;
void  EVE_SPI_ClearRxBuffer(void)                  ;
void  EVE_SPI_ClearTxBuffer(void)                  ;
void  EVE_SPI_ClearFIFO(void)                              ;
void  EVE_SPI_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != EVE_SPI_BIDIRECTIONAL_MODE)
    void  EVE_SPI_TxEnable(void)                   ;
    void  EVE_SPI_TxDisable(void)                  ;
#endif /* (0u != EVE_SPI_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(EVE_SPI_TX_ISR);
CY_ISR_PROTO(EVE_SPI_RX_ISR);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 EVE_SPI_initVar;


/***************************************
*           API Constants
***************************************/

#define EVE_SPI_TX_ISR_NUMBER     ((uint8) (EVE_SPI_TxInternalInterrupt__INTC_NUMBER))
#define EVE_SPI_RX_ISR_NUMBER     ((uint8) (EVE_SPI_RxInternalInterrupt__INTC_NUMBER))

#define EVE_SPI_TX_ISR_PRIORITY   ((uint8) (EVE_SPI_TxInternalInterrupt__INTC_PRIOR_NUM))
#define EVE_SPI_RX_ISR_PRIORITY   ((uint8) (EVE_SPI_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define EVE_SPI_INT_ON_SPI_DONE    ((uint8) (0u   << EVE_SPI_STS_SPI_DONE_SHIFT))
#define EVE_SPI_INT_ON_TX_EMPTY    ((uint8) (0u   << EVE_SPI_STS_TX_FIFO_EMPTY_SHIFT))
#define EVE_SPI_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           EVE_SPI_STS_TX_FIFO_NOT_FULL_SHIFT))
#define EVE_SPI_INT_ON_BYTE_COMP   ((uint8) (0u  << EVE_SPI_STS_BYTE_COMPLETE_SHIFT))
#define EVE_SPI_INT_ON_SPI_IDLE    ((uint8) (0u   << EVE_SPI_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define EVE_SPI_INT_ON_TX_NOT_FULL_DEF ((EVE_SPI_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (EVE_SPI_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define EVE_SPI_TX_INIT_INTERRUPTS_MASK    (EVE_SPI_INT_ON_SPI_DONE  | \
                                                     EVE_SPI_INT_ON_TX_EMPTY  | \
                                                     EVE_SPI_INT_ON_TX_NOT_FULL_DEF | \
                                                     EVE_SPI_INT_ON_BYTE_COMP | \
                                                     EVE_SPI_INT_ON_SPI_IDLE)

#define EVE_SPI_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          EVE_SPI_STS_RX_FIFO_FULL_SHIFT))
#define EVE_SPI_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          EVE_SPI_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define EVE_SPI_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          EVE_SPI_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define EVE_SPI_RX_INIT_INTERRUPTS_MASK    (EVE_SPI_INT_ON_RX_FULL      | \
                                                     EVE_SPI_INT_ON_RX_NOT_EMPTY | \
                                                     EVE_SPI_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define EVE_SPI_BITCTR_INIT            (((uint8) (EVE_SPI_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/
#if(CY_PSOC3 || CY_PSOC5)
    #define EVE_SPI_TXDATA_REG (* (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F0_REG)
    #define EVE_SPI_TXDATA_PTR (  (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F0_REG)
    #define EVE_SPI_RXDATA_REG (* (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F1_REG)
    #define EVE_SPI_RXDATA_PTR (  (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(EVE_SPI_USE_SECOND_DATAPATH)
        #define EVE_SPI_TXDATA_REG (* (reg16 *) \
                                          EVE_SPI_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define EVE_SPI_TXDATA_PTR (  (reg16 *) \
                                          EVE_SPI_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define EVE_SPI_RXDATA_REG (* (reg16 *) \
                                          EVE_SPI_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define EVE_SPI_RXDATA_PTR (  (reg16 *) \
                                          EVE_SPI_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define EVE_SPI_TXDATA_REG (* (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F0_REG)
        #define EVE_SPI_TXDATA_PTR (  (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F0_REG)
        #define EVE_SPI_RXDATA_REG (* (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F1_REG)
        #define EVE_SPI_RXDATA_PTR (  (reg8 *) \
                                                EVE_SPI_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (EVE_SPI_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define EVE_SPI_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        EVE_SPI_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define EVE_SPI_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        EVE_SPI_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(EVE_SPI_USE_SECOND_DATAPATH)
    #define EVE_SPI_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        EVE_SPI_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define EVE_SPI_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        EVE_SPI_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (EVE_SPI_USE_SECOND_DATAPATH) */

#define EVE_SPI_COUNTER_PERIOD_REG     (* (reg8 *) EVE_SPI_BSPIM_BitCounter__PERIOD_REG)
#define EVE_SPI_COUNTER_PERIOD_PTR     (  (reg8 *) EVE_SPI_BSPIM_BitCounter__PERIOD_REG)
#define EVE_SPI_COUNTER_CONTROL_REG    (* (reg8 *) EVE_SPI_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define EVE_SPI_COUNTER_CONTROL_PTR    (  (reg8 *) EVE_SPI_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define EVE_SPI_TX_STATUS_REG          (* (reg8 *) EVE_SPI_BSPIM_TxStsReg__STATUS_REG)
#define EVE_SPI_TX_STATUS_PTR          (  (reg8 *) EVE_SPI_BSPIM_TxStsReg__STATUS_REG)
#define EVE_SPI_RX_STATUS_REG          (* (reg8 *) EVE_SPI_BSPIM_RxStsReg__STATUS_REG)
#define EVE_SPI_RX_STATUS_PTR          (  (reg8 *) EVE_SPI_BSPIM_RxStsReg__STATUS_REG)

#define EVE_SPI_CONTROL_REG            (* (reg8 *) \
                                      EVE_SPI_BSPIM_BidirMode_CtrlReg__CONTROL_REG)
#define EVE_SPI_CONTROL_PTR            (  (reg8 *) \
                                      EVE_SPI_BSPIM_BidirMode_CtrlReg__CONTROL_REG)

#define EVE_SPI_TX_STATUS_MASK_REG     (* (reg8 *) EVE_SPI_BSPIM_TxStsReg__MASK_REG)
#define EVE_SPI_TX_STATUS_MASK_PTR     (  (reg8 *) EVE_SPI_BSPIM_TxStsReg__MASK_REG)
#define EVE_SPI_RX_STATUS_MASK_REG     (* (reg8 *) EVE_SPI_BSPIM_RxStsReg__MASK_REG)
#define EVE_SPI_RX_STATUS_MASK_PTR     (  (reg8 *) EVE_SPI_BSPIM_RxStsReg__MASK_REG)

#define EVE_SPI_TX_STATUS_ACTL_REG     (* (reg8 *) EVE_SPI_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define EVE_SPI_TX_STATUS_ACTL_PTR     (  (reg8 *) EVE_SPI_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define EVE_SPI_RX_STATUS_ACTL_REG     (* (reg8 *) EVE_SPI_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define EVE_SPI_RX_STATUS_ACTL_PTR     (  (reg8 *) EVE_SPI_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(EVE_SPI_USE_SECOND_DATAPATH)
    #define EVE_SPI_AUX_CONTROLDP1     (EVE_SPI_AUX_CONTROL_DP1_REG)
#endif /* (EVE_SPI_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define EVE_SPI_STS_SPI_DONE_SHIFT             (0x00u)
#define EVE_SPI_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define EVE_SPI_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define EVE_SPI_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define EVE_SPI_STS_SPI_IDLE_SHIFT             (0x04u)
#define EVE_SPI_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define EVE_SPI_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define EVE_SPI_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define EVE_SPI_STS_SPI_DONE           ((uint8) (0x01u << EVE_SPI_STS_SPI_DONE_SHIFT))
#define EVE_SPI_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << EVE_SPI_STS_TX_FIFO_EMPTY_SHIFT))
#define EVE_SPI_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << EVE_SPI_STS_TX_FIFO_NOT_FULL_SHIFT))
#define EVE_SPI_STS_BYTE_COMPLETE      ((uint8) (0x01u << EVE_SPI_STS_BYTE_COMPLETE_SHIFT))
#define EVE_SPI_STS_SPI_IDLE           ((uint8) (0x01u << EVE_SPI_STS_SPI_IDLE_SHIFT))
#define EVE_SPI_STS_RX_FIFO_FULL       ((uint8) (0x01u << EVE_SPI_STS_RX_FIFO_FULL_SHIFT))
#define EVE_SPI_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << EVE_SPI_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define EVE_SPI_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << EVE_SPI_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define EVE_SPI_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define EVE_SPI_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define EVE_SPI_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define EVE_SPI_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define EVE_SPI_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define EVE_SPI_FIFO_CLR       (EVE_SPI_TX_FIFO_CLR | EVE_SPI_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define EVE_SPI_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define EVE_SPI_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define EVE_SPI_AUX_CTRL_FIFO0_CLR         (0x01u)
#define EVE_SPI_AUX_CTRL_FIFO1_CLR         (0x02u)
#define EVE_SPI_AUX_CTRL_FIFO0_LVL         (0x04u)
#define EVE_SPI_AUX_CTRL_FIFO1_LVL         (0x08u)
#define EVE_SPI_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define EVE_SPI_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define EVE_SPI_IS_ENABLED (0u != (EVE_SPI_TX_STATUS_ACTL_REG & EVE_SPI_INT_ENABLE))

/* Retuns TX status register */
#define EVE_SPI_GET_STATUS_TX(swTxSts) ( (uint8)(EVE_SPI_TX_STATUS_REG | \
                                                          ((swTxSts) & EVE_SPI_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define EVE_SPI_GET_STATUS_RX(swRxSts) ( (uint8)(EVE_SPI_RX_STATUS_REG | \
                                                          ((swRxSts) & EVE_SPI_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define EVE_SPI_WriteByte   EVE_SPI_WriteTxData
#define EVE_SPI_ReadByte    EVE_SPI_ReadRxData
void  EVE_SPI_SetInterruptMode(uint8 intSrc)       ;
uint8 EVE_SPI_ReadStatus(void)                     ;
void  EVE_SPI_EnableInt(void)                      ;
void  EVE_SPI_DisableInt(void)                     ;

#define EVE_SPI_TXDATA                 (EVE_SPI_TXDATA_REG)
#define EVE_SPI_RXDATA                 (EVE_SPI_RXDATA_REG)
#define EVE_SPI_AUX_CONTROLDP0         (EVE_SPI_AUX_CONTROL_DP0_REG)
#define EVE_SPI_TXBUFFERREAD           (EVE_SPI_txBufferRead)
#define EVE_SPI_TXBUFFERWRITE          (EVE_SPI_txBufferWrite)
#define EVE_SPI_RXBUFFERREAD           (EVE_SPI_rxBufferRead)
#define EVE_SPI_RXBUFFERWRITE          (EVE_SPI_rxBufferWrite)

#define EVE_SPI_COUNTER_PERIOD         (EVE_SPI_COUNTER_PERIOD_REG)
#define EVE_SPI_COUNTER_CONTROL        (EVE_SPI_COUNTER_CONTROL_REG)
#define EVE_SPI_STATUS                 (EVE_SPI_TX_STATUS_REG)
#define EVE_SPI_CONTROL                (EVE_SPI_CONTROL_REG)
#define EVE_SPI_STATUS_MASK            (EVE_SPI_TX_STATUS_MASK_REG)
#define EVE_SPI_STATUS_ACTL            (EVE_SPI_TX_STATUS_ACTL_REG)

#define EVE_SPI_INIT_INTERRUPTS_MASK  (EVE_SPI_INT_ON_SPI_DONE     | \
                                                EVE_SPI_INT_ON_TX_EMPTY     | \
                                                EVE_SPI_INT_ON_TX_NOT_FULL_DEF  | \
                                                EVE_SPI_INT_ON_RX_FULL      | \
                                                EVE_SPI_INT_ON_RX_NOT_EMPTY | \
                                                EVE_SPI_INT_ON_RX_OVER      | \
                                                EVE_SPI_INT_ON_BYTE_COMP)
                                                
#define EVE_SPI_DataWidth                  (EVE_SPI_DATA_WIDTH)
#define EVE_SPI_InternalClockUsed          (EVE_SPI_INTERNAL_CLOCK)
#define EVE_SPI_InternalTxInterruptEnabled (EVE_SPI_INTERNAL_TX_INT_ENABLED)
#define EVE_SPI_InternalRxInterruptEnabled (EVE_SPI_INTERNAL_RX_INT_ENABLED)
#define EVE_SPI_ModeUseZero                (EVE_SPI_MODE_USE_ZERO)
#define EVE_SPI_BidirectionalMode          (EVE_SPI_BIDIRECTIONAL_MODE)
#define EVE_SPI_Mode                       (EVE_SPI_MODE)
#define EVE_SPI_DATAWIDHT                  (EVE_SPI_DATA_WIDTH)
#define EVE_SPI_InternalInterruptEnabled   (0u)

#define EVE_SPI_TXBUFFERSIZE   (EVE_SPI_TX_BUFFER_SIZE)
#define EVE_SPI_RXBUFFERSIZE   (EVE_SPI_RX_BUFFER_SIZE)

#define EVE_SPI_TXBUFFER       EVE_SPI_txBuffer
#define EVE_SPI_RXBUFFER       EVE_SPI_rxBuffer

#endif /* (CY_SPIM_EVE_SPI_H) */


/* [] END OF FILE */
