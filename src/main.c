/***********************************************************************************************************************
* @file     main.c
* @author   Boyan Bonev
* @version  1.0
* @date     11 Jan 2017
* @brief
*
* @copyright
***********************************************************************************************************************/

///=====================================================================================================================
/// \brief INCLUDE SECTION
///=====================================================================================================================
#include "stm32f10x_conf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "DrvRFRx.h"
#include "DrvRFTx.h"

///=====================================================================================================================
/// \brief LOCAL DATA
///=====================================================================================================================


///=====================================================================================================================
/// \brief
///=====================================================================================================================


///=====================================================================================================================
/// \brief
///=====================================================================================================================

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
int main(void)
{
    TaskHandle_t hTask;

    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

    /// < Creates the task to handle all USART functions
    hTask = xTaskCreateStatic( &DrvRx_Task,
                        ( char * ) "DrvRx_Task",
                        configRx_TASK_STACK_SIZE,
                        NULL,
                        configRx_TASK_PRIORITY,
                        xRxStack,
                        &xRxTaskTCBBuffer);
    assert_param( hTask != 0 );

    /// < Creates the task to handle all LCD functions
    hTask = xTaskCreateStatic( &DrvTx_Task,
                        ( char * ) "DrvTx_Task",
                        configTx_TASK_STACK_SIZE,
                        NULL,
                        configTx_TASK_PRIORITY,
                        xTxStack,
                        &xTxTaskTCBBuffer);
    assert_param( hTask != 0 );
/*
    /// <! Creates the task to handle all Game of Life
    hTask = xTaskCreateStatic( &GOL_Task,
                        ( char * ) "GOL_Task",
                        configGOL_TASK_STACK_SIZE,
                        NULL,
                        configGOL_TASK_PRIORITY,
                        xGOLStack,
                        &xGOLTaskTCBBuffer);
    assert_param( hTask != 0 );
*/
    //// <! Start of the RTOS scheduler, this function should never return >
    vTaskStartScheduler();

    while(1);

    return -1;
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
void vApplicationStackOverflowHook( TaskHandle_t *pxTask, signed portCHAR *pcTaskName )
{
    assert_param( pdFALSE );
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    static StackType_t xIdleStack[configMINIMAL_STACK_SIZE / 64];
    static StaticTask_t xIdleTaskTCBBuffer;

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = xIdleStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
void vApplicationIdleHook()
{
    while(1);
}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
void vApplicationTickHook( void )
{

}

///---------------------------------------------------------------------------------------------------------------------
/// \brief
/// \param  None
///---------------------------------------------------------------------------------------------------------------------
void assert_failed(char* file, unsigned long line)
{
    // User can add his own implementation to report the file name and line number,
    // ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
    asm("bkpt");
}
