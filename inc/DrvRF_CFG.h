/***********************************************************************************************************************
* \file     DrvRF_CFG.h
* \author   Boyan Bonev
* \version  1.0
* \date     11 Feb 2017
* \brief
*
* \copyright
***********************************************************************************************************************/

#ifndef DRV_RF_CFG_H
#define DRV_RF_CFG_H

///=====================================================================================================================
/// EXPORTED CONSTANTS
///=====================================================================================================================
#define CFG_RECEIVER_ID                         { 0x17, 0x34, 0x68, 0x00}
#define CFG_TRANSMITER_ID                       { 0x87, 0x34, 0x68, 0x00}

#define CFG_START_LENGTH                        ((uint32_t)400)
#define CFG_BIT_START_LENGTH                    ((uint32_t)(CFG_START_LENGTH / 4))
#define CFG_DATA_HIGH_LENGTH                    (CFG_BIT_START_LENGTH + 2 * CFG_BIT_START_LENGTH)
#define CFG_DATA_LOW_LENGTH                     (CFG_BIT_START_LENGTH + 4 * CFG_BIT_START_LENGTH)

#define CFG_TRANSMITER_TIMER_PERIOD             (100)
#define CFG_RECEIVER_TIMER_PERIOD               (TRANSMITER_TIMER_PERIOD / 2)

#define CFG_TX_BUFFER_LENGTH                    ((uint8_t)64)   /// [bytes]
#define CFG_RX_BUFFER_LENGTH                    ((uint8_t)64)   /// [bytes]

///=====================================================================================================================
/// COMMANDS
///=====================================================================================================================

#endif /// DRV_RF_CFG_H
