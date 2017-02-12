/***********************************************************************************************************************
* \file     DrvRFRx.h
* \author   Boyan Bonev
* \version  1.0
* \date     08 Feb 2017
* \brief    Driver for RF Receiver 315/330/433 MHz
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
#include "DrvRFRx.h"


///=====================================================================================================================
/// CONST DATA
///=====================================================================================================================
#define configRX_BUFFER_LENGTH              ((uint8_t)64)   /// [bytes]

///=====================================================================================================================
/// EXPORTED DATA
///=====================================================================================================================
StackType_t xRxStack[configRx_TASK_STACK_SIZE];
StaticTask_t xRxTaskTCBBuffer;
TaskHandle_t xRxToNotify = 0;


///=====================================================================================================================
/// LOCAL DATA
///=====================================================================================================================
static QueueHandle_t RxQueue;
static uint8_t rxQueueStorage[configRX_BUFFER_LENGTH];
static StaticQueue_t RxQueueBuffer;


///=====================================================================================================================
/// LOCAL FUNCTIONS
///=====================================================================================================================
static void Init_RCC(void);
static void Init_NVIC(void);
static void Init_Timer(void);
static void Init_GPIO(void);


///=====================================================================================================================
/// EXPORTED FUNCTIONS
///=====================================================================================================================

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
void DrvRx_Init( void )
{
    /// Create a queue capable of containing inout and output data.
    RxQueue = xQueueCreateStatic(   configRX_BUFFER_LENGTH,
                                    sizeof( portCHAR ),
                                    rxQueueStorage,
                                    &RxQueueBuffer );

    Init_RCC();
    Init_NVIC();
    Init_Timer();
    Init_GPIO();
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
void DrvRx_Task( void *pvParameters )
{
    char ch;

    DrvRx_Init();

    while(1)
    {
        if( xQueueReceive( RxQueue, &ch, 0 ) == pdPASS )
        {

        }
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
    /* TIM3 and GPIOA clock enable */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
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
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
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

    /* TIM3 channel 2 pin (PA.00) configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
    TIM_InitStructure.TIM_Period = RECEIVER_TIMER_PERIOD;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM3, &TIM_InitStructure);

    /* TIM enable counter */
    TIM_Cmd(TIM3, ENABLE);

    /* Enable the CC2 Interrupt Request */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief  This function handles RTC global interrupt request.
/// \param  None
/// \retval None
///---------------------------------------------------------------------------------------------------------------------
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
    {
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    }
}
