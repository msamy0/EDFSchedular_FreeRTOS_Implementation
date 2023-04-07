/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			1
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1

#define configQUEUE_REGISTRY_SIZE 	0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay					1

/*######################################## Samy_EDF_Edits_Start ########################################*/
#include "GPIO.h"

#define configUSE_EDF_SCHEDULER   				1
#define INCLUDE_xTaskGetIdleTaskHandle			1
#define configSUPPORT_DYNAMIC_ALLOCATION		1
#define configUSE_QUEUE_SETS					1
#define configUSE_APPLICATION_TASK_TAG			1

typedef unsigned char 			   uint8;
typedef unsigned short int         uint16;
typedef unsigned int               uint32;
typedef unsigned long long         uint64;
typedef signed char                sint8;
typedef signed short int           sint16;
typedef signed int                 sint32;
	
typedef enum
{
	RESET_STATUS = 0,
	RISING_EDGE,
	FALLING_EDGE
}Button_Status_ENM;

#define T1_Period	50
#define T2_Period	50
#define T3_Period	100
#define T4_Period	20
#define T5_Period	10
#define T6_Period	100

#define BUTTON_1_PIN	PIN1
#define BUTTON_1_PORT	PORT_0
#define BUTTON_2_PIN	PIN2
#define BUTTON_2_PORT	PORT_0
#define IDLE_HOOK_PIN	PIN0
#define IDLE_HOOK_PORT	PORT_0
#define TICK_HOOK_PIN	PIN3
#define TICK_HOOK_PORT	PORT_0

#define Tx_HOOK_PORT	PORT_1
#define T1_HOOK_PIN		PIN1
#define T2_HOOK_PIN		PIN2
#define T3_HOOK_PIN		PIN3
#define T4_HOOK_PIN		PIN4
#define T5_HOOK_PIN		PIN5
#define T6_HOOK_PIN		PIN6

extern uint32 T1_IN , T1_Total;
extern uint32 T2_IN , T2_Total;
extern uint32 T3_IN , T3_Total;
extern uint32 T4_IN , T4_Total;
extern uint32 T5_IN , T5_Total;
extern uint32 T6_IN , T6_Total;
extern uint32 SysTime;
extern uint32 CPU_Load;

/* Record the tick in which you entered the task and raise the corresppnding pin HIGH */

#define traceTASK_SWITCHED_IN() do{\
									if((uint32)pxCurrentTCB->pxTaskTag == 1)\
									{\
										GPIO_write(Tx_HOOK_PORT , T1_HOOK_PIN , PIN_IS_HIGH);\
										T1_IN = T1TC;\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 2)\
									{\
										GPIO_write(Tx_HOOK_PORT , T2_HOOK_PIN , PIN_IS_HIGH);\
										T2_IN = T1TC;\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 3)\
									{\
										GPIO_write(Tx_HOOK_PORT , T3_HOOK_PIN , PIN_IS_HIGH);\
										T3_IN = T1TC;\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 4)\
									{\
										GPIO_write(Tx_HOOK_PORT , T4_HOOK_PIN , PIN_IS_HIGH);\
										T4_IN = T1TC;\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 5)\
									{\
										GPIO_write(Tx_HOOK_PORT , T5_HOOK_PIN , PIN_IS_HIGH);\
										T5_IN = T1TC;\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 6)\
									{\
										GPIO_write(Tx_HOOK_PORT , T6_HOOK_PIN , PIN_IS_HIGH);\
										T6_IN = T1TC;\
									}\
									}while(0);

/* On leaving each task, calculate the total execution time of each task. Use all tasks execution time with
   the CPU all execution time to calculate the CPU load >> CPU Load = sum of tasks execution time / All CPU run time*/

#define traceTASK_SWITCHED_OUT() do{\
									if((uint32)pxCurrentTCB->pxTaskTag == 1 )\
									{\
										GPIO_write(Tx_HOOK_PORT , T1_HOOK_PIN , PIN_IS_LOW );\
										T1_Total += (T1TC - T1_IN );\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 2 )\
									{\
										GPIO_write(Tx_HOOK_PORT , T2_HOOK_PIN , PIN_IS_LOW );\
										T2_Total += (T1TC - T2_IN );\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 3 )\
									{\
										GPIO_write(Tx_HOOK_PORT , T3_HOOK_PIN , PIN_IS_LOW );\
										T3_Total += (T1TC - T3_IN );\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 4 )\
									{\
										GPIO_write(Tx_HOOK_PORT , T4_HOOK_PIN , PIN_IS_LOW );\
										T4_Total += (T1TC - T4_IN );\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 5 )\
									{\
										GPIO_write(Tx_HOOK_PORT , T5_HOOK_PIN , PIN_IS_LOW );\
										T5_Total += (T1TC - T5_IN );\
									}\
									else if((uint32)pxCurrentTCB->pxTaskTag == 6 )\
									{\
										GPIO_write(Tx_HOOK_PORT , T6_HOOK_PIN , PIN_IS_LOW );\
										T6_Total += (T1TC - T6_IN );\
									}\
									SysTime = T1TC;\
									CPU_Load = ( (((T1_Total + T2_Total + T3_Total + T4_Total + T5_Total + T6_Total) / (float)SysTime )) * 100 );\
									}while(0);

/*######################################## Samy_EDF_Edits_End ########################################*/	

#endif /* FREERTOS_CONFIG_H */
