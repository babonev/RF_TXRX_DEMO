/***********************************************************************************************************************
* \file     DrvRFTx.h
* \author   Boyan Bonev
* \version  1.0
* \date     08 Feb 2017
* \brief    Driver for RF Transmitter 315/330/433 MHz
*
* \copyright
***********************************************************************************************************************/

#ifndef DRV_RFTX_H
#define DRV_RFTX_H

///=====================================================================================================================
/// EXPORTED TYPES
///=====================================================================================================================


///=====================================================================================================================
/// EXPORTED FUNCTIONS
///=====================================================================================================================
void DrvTx_Init( void );
void DrvTx_Send( const uint8_t data );


///=====================================================================================================================
/// \brief RTC OS interface
///=====================================================================================================================
#define configTx_TASK_STACK_SIZE       ( configMINIMAL_STACK_SIZE / 2 )
#define configTx_TASK_PRIORITY         ( tskIDLE_PRIORITY )

extern StackType_t xTxStack[configTx_TASK_STACK_SIZE];
extern StaticTask_t xTxTaskTCBBuffer;
extern TaskHandle_t xTxToNotify;

extern void DrvTx_Task( void *pvParameters );
///---------------------------------------------------------------------------------------------------------------------


#endif /// DRV_RFTX_H
