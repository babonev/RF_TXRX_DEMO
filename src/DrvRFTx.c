/***********************************************************************************************************************
* \file     DrvRFTx.h
* \author   Boyan Bonev
* \version  1.0
* \date     08 Feb 2017
* \brief    Driver for RF Transmitter 315/330/433 MHz
*
* \copyright
***********************************************************************************************************************/


///=====================================================================================================================
/// INCLUDE SECTION
///=====================================================================================================================
#include "stm32f10x_rtc.h"
#include "misc.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "event_groups.h"
#include "DrvRFTx.h"
#include "DrvRF_CFG.h"

///=====================================================================================================================
/// EXPORTED DATA
///=====================================================================================================================
StackType_t xTxStack[configTx_TASK_STACK_SIZE];
StaticTask_t xTxTaskTCBBuffer;
TaskHandle_t xTxToNotify = 0;


///=====================================================================================================================
/// LOCAL DATA
///=====================================================================================================================
static QueueHandle_t TxQueue;
static uint8_t txQueueStorage[CFG_TX_BUFFER_LENGTH];
static StaticQueue_t TxQueueBuffer;

///=====================================================================================================================
/// LOCAL FUNCTIONS
///=====================================================================================================================
static void Init_RCC(void);
static void Init_NVIC(void);
static void Init_Timer(void);
static void Init_GPIO(void);

static void SetOutput( const uint32_t state );


///=====================================================================================================================
/// EXPORTED FUNCTIONS
///=====================================================================================================================

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
void DrvTx_Init( void )
{
    /// Create a queue capable of containing inout and output data.
    TxQueue = xQueueCreateStatic(   configTX_BUFFER_LENGTH,
                                    sizeof( portCHAR ),
                                    txQueueStorage,
                                    &TxQueueBuffer );

    Init_RCC();
    Init_NVIC();
    Init_Timer();
    Init_GPIO();
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
void DrvTx_Task( void *pvParameters )
{
    char ch;

    DrvTx_Init();

    while(1)
    {

    }
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
void DrvTx_Send( const uint8_t data )
{
    if( xQueueSend( TxQueue, &data, 0 ) == pdPASS )
    {
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    }
}

///=====================================================================================================================
/// LOCAL FUNCTIONS
///=====================================================================================================================

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
static void Init_RCC(void)
{
    /* TIM2 and GPIOD clock enable */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief  Configures the nested vectored interrupt controller.
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
static void Init_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /// Enable the RTC Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief  Initializes and start internal RTC.
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
static void Init_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* TIM2 channel 2 pin (PD.00) configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief  Initializes and start internal RTC.
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
static void Init_Timer(void)
{
    TIM_TimeBaseInitTypeDef  TIM_InitStructure;

    TIM_InitStructure.TIM_Prescaler = 0;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = TRANSMITER_TIMER_PERIOD;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

    /* TIM enable counter */
    TIM_Cmd(TIM2, ENABLE);

    /* Enable the CC2 Interrupt Request */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief  
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
static void SetOutput( const uint32_t state )
{

}

///---------------------------------------------------------------------------------------------------------------------
/// \brief  This function handles RTC global interrupt request.
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
void TIM2_IRQHandler(void)
{
    uint32_t outState;

    /* TIM2_CH1 toggling with frequency = 183.1 Hz */
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update );

        outState = 0;
        SetOutput(outState);
    }
}
