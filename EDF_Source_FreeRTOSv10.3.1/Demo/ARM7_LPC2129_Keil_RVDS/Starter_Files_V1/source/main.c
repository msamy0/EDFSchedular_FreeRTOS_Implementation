/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 
 * Edits was implemented in this project in favour of learning course submission
 * Edited By: @msamy0
 * All Edits made to the original freeRTOS demo is gaurded #if (configUSE_EDF_SCHEDULER == 1)
 * All Edits made to the original freeRTOS demo is encapsulated by the following s:-
   ########################################Samy_EDF_Edits_Start########################################
	 ########################################Samy_EDF_Edits_End########################################
 * Egypt_FWD_Adv 2023
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

/*########################################Samy_EDF_Edits_Start########################################*/
typedef unsigned char 			   uint8;
typedef unsigned short int         uint16;
typedef unsigned int               uint32;
typedef unsigned long long         uint64;
typedef signed char                sint8;
typedef signed short int           sint16;
typedef signed int                 sint32;

typedef enum
{
	LOW_STATUS,
	HIGH_STATUS,
	RISING_EDGE,
	FALLING_EDGE
}Button_Status_ENM;

#define T1_Period	200
#define T2_Period	200
#define T3_Period	200
#define T4_Period	200
#define T5_Period	200
#define T6_Period	200

#define BUTTON_1_PIN	PIN1
#define BUTTON_1_PORT	PORT_0
#define BUTTON_2_PIN	PIN2
#define BUTTON_2_PORT	PORT_0
#define IDLE_HOOK_PIN	PIN0
#define IDLE_HOOK_PORT	PORT_0
#define TICK_HOOK_PIN	PIN4
#define TICK_HOOK_PORT	PORT_0				

TaskHandle_t T1_Handle	= NULL;
TaskHandle_t T2_Handle	= NULL;
TaskHandle_t T3_Handle 	= NULL;
TaskHandle_t T4_Handle	= NULL;
TaskHandle_t T5_Handle	= NULL;
TaskHandle_t T6_Handle	= NULL;

QueueHandle_t Q_T1_Button1 	=NULL;
QueueHandle_t Q_T2_Button2 	=NULL;
QueueHandle_t Q_T3_String 	=NULL;

uint32 T1_IN=0 , T1_OUT=0 , T1_Total=0;
uint32 T2_IN=0 , T2_OUT=0 , T2_Total=0;
uint32 T3_IN=0 , T3_OUT=0 , T3_Total=0;
uint32 T4_IN=0 , T4_OUT=0 , T4_Total=0;
uint32 T5_IN=0 , T5_OUT=0 , T5_Total=0;
uint32 T6_IN=0 , T6_OUT=0 , T6_Total=0;
uint32 SysTime = 0;
uint32 CPU_Load = 0;

/*########################################Samy_EDF_Edits_End########################################*/	

/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

/*########################################Samy_EDF_Edits_Start########################################*/

/* Hooks Implementaion*/
/*---------------------------------------------------------------*/
void vApplicationTickHook( void )
{
	GPIO_write(TICK_HOOK_PORT , TICK_HOOK_PIN , PIN_IS_HIGH);
	GPIO_write(TICK_HOOK_PORT , TICK_HOOK_PIN , PIN_IS_LOW);
}

void vApplicationIdleHook( void )
{
	GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_HIGH);
	/*it is gonna be LOW at each entrance of of any task*/
}

/* Functions Implementation*/
/*---------------------------------------------------------------*/
void T1_Button1(void * pvParameter)
{
	// Record the first time Task is called to calculate next absolute call (Deadline)(Period)
	TickType_t previousWakeTime = xTaskGetTickCount();
	
	//Store the 4 button status (HIGH LOW RISE FALL)
	volatile Button_Status_ENM buttonState;
	
	//Stores the current and previous pin reading to decide the button status
	volatile pinState_t currentState = PIN_IS_LOW;
	volatile pinState_t previousState = PIN_IS_LOW;
	
	while(1)
	{
		currentState = GPIO_read(BUTTON_1_PORT , BUTTON_1_PIN);
		if (previousState == PIN_IS_LOW)
		{
			if (currentState == PIN_IS_LOW)
			{
				buttonState = LOW_STATUS; //button was Low last call and still Low
			}
			else
			{
				buttonState = RISING_EDGE;//button was low Last call and now it is High(Rising)
			}
		}
		else /*(previousState == PIN_IS_HIGH)*/
		{
			if (currentState == PIN_IS_HIGH)
			{
				buttonState = HIGH_STATUS;//button was High last call and still High
			}
			else
			{
				buttonState = FALLING_EDGE;//button was High Last call and now it is Low (Falling)
			}
		}
		previousState = currentState; //save "current" pin state to be "previous" for next call
		
		//Push button state to the queue to be prepared for showing up in UART
		if (Q_T1_Button1 != NULL)
		{
			xQueueSend(Q_T1_Button1, (void *) &buttonState,0);
		}
		vTaskDelayUntil( &previousWakeTime, T1_Period);//wait till next call 
		GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_LOW);//first thing to do on re-call
	}
}

void T2_Button2(void * pvParameter)
{
	// Record the first time Task is called to calculate next absolute call (Deadline)(Period)
	TickType_t previousWakeTime = xTaskGetTickCount();
	
	//Store the 4 button status (HIGH LOW RISE FALL)
	volatile Button_Status_ENM buttonState;
	
	//Stores the current and previous pin reading to decide the button status
	volatile pinState_t currentState = PIN_IS_LOW;
	volatile pinState_t previousState = PIN_IS_LOW;
	
	while(1)
	{
		currentState = GPIO_read(BUTTON_2_PORT , BUTTON_2_PIN);
		if (previousState == PIN_IS_LOW)
		{
			if (currentState == PIN_IS_LOW)
			{
				buttonState = LOW_STATUS; //button was Low last call and still Low
			}
			else
			{
				buttonState = RISING_EDGE;//button was low Last call and now it is High(Rising)
			}
		}
		else /*(previousState == PIN_IS_HIGH)*/
		{
			if (currentState == PIN_IS_HIGH)
			{
				buttonState = HIGH_STATUS;//button was High last call and still High
			}
			else
			{
				buttonState = FALLING_EDGE;//button was High Last call and now it is Low (Falling)
			}
		}
		previousState = currentState; //save "current" pin state to be "previous" for next call
		
		//Push button state to the queue to be prepared for showing up in UART
		if (Q_T2_Button2 != NULL)
		{
			xQueueSend(Q_T2_Button2, (void *) &buttonState,0);
		}
		vTaskDelayUntil( &previousWakeTime, T1_Period);//wait till next call 
		GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_LOW);//first thing to do on re-call
	}
}
/*
void T3_PeriodicString( void * pvParameters )
{
	TickType_t previousWakeTime = xTaskGetTickCount();
	uint8 T3_isRunning = 1;
	
	while(1)
	{
		if(Q_T3_String != NULL)
		{
			xQueueSend(Q_T3_String, (void *) &T3_isRunning,0);
		}
		vTaskDelayUntil( &previousWakeTime, T3_Period );
		GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_LOW);//first thing to do on re-call
	}
}
*/
void T3_PeriodicString( void * pvParameters )
{
	TickType_t previousWakeTime = xTaskGetTickCount();
	char T3_isRunning_String[] = "Task 3 is Running\n";
	
	while(1)
	{
		if(Q_T3_String != NULL)
		{
			uint8 localCounter = 0;
			while (T3_isRunning_String[localCounter] != '\n')
			{
				xQueueSend(Q_T3_String, (void *) &T3_isRunning_String[localCounter],0);
				localCounter++;
			}
			xQueueSend(Q_T3_String, (void *) &T3_isRunning_String[localCounter],0);
				
		}
		vTaskDelayUntil( &previousWakeTime, T3_Period );
		GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_LOW);//first thing to do on re-call
	}
}
void T4_UART_Print( void * pvParameters )
{
	TickType_t previousWakeTime = xTaskGetTickCount();
	signed char buffer_String[100];
	Button_Status_ENM buffer_ButtonStatus;
		
	while(1)
	{
		if(Q_T3_String != NULL)
		{
			if (Q_T1_Button1 != NULL)
			{
				if((xQueueReceive(Q_T1_Button1 , (void *)&buffer_ButtonStatus ,(TickType_t)0)) == pdPASS)
				{
					if (buffer_ButtonStatus == RISING_EDGE)
					{
						vSerialPutString((const signed char *)"Button 1 Rising\n" , 17);
					}
					else if (buffer_ButtonStatus == FALLING_EDGE)
					{
						vSerialPutString((const signed char *)"Button 1 Falling\n" , 18);
					}
					else/* Fixed status from last read*/
					{
						/* Print here that button is unchanged if needed*/
					}
				}	
			}
			if (Q_T2_Button2 != NULL)
			{
				if((xQueueReceive(Q_T2_Button2 , (void *)&buffer_ButtonStatus ,(TickType_t)0)) == pdPASS)
				{
					if (buffer_ButtonStatus == RISING_EDGE)
					{
						vSerialPutString((const signed char *)"Button 2 Rising\n" , 17);
					}
					else if (buffer_ButtonStatus == FALLING_EDGE)
					{
						vSerialPutString((const signed char *)"Button 2 Falling\n" , 18);
					}
					else/* Fixed status from last read*/
					{
						/* Print here that button is unchanged if needed*/
					}
				}	
			}
			if (Q_T3_String != NULL)
			{
				uint8 localCounter=0;
				while ((xQueueReceive(Q_T3_String , (void *)&buffer_String[localCounter] ,(TickType_t)0)) == pdPASS)
				{
					xSerialPutChar((signed char) buffer_String[localCounter]);
					if (buffer_String[localCounter] == '\n')
					{
						break;
					}
					localCounter++;
				}	
			}	
		}
		vTaskDelayUntil( &previousWakeTime, T3_Period );
		GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_LOW);//first thing to do on re-call
	}
}
void T5_Load1( void * pvParameters )
{
		uint32 localCounter = 0;
		TickType_t previousWakeTime = xTaskGetTickCount();
		while(1)
		{
			for(localCounter = 0 ; localCounter<33300 ;localCounter++)
			{
				localCounter=localCounter;
			}
			vTaskDelayUntil( &previousWakeTime, T5_Period );
			GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_LOW);//first thing to do on re-call
		}
		
}
void T6_Load2( void * pvParameters )
{
		uint32 localCounter = 0;
		TickType_t previousWakeTime = xTaskGetTickCount();
	
		while(1)
		{
			for(localCounter = 0 ; localCounter<33300 ;localCounter++)
			{
				localCounter=localCounter;
			}
			vTaskDelayUntil( &previousWakeTime, T6_Period );
			GPIO_write(IDLE_HOOK_PORT , IDLE_HOOK_PIN , PIN_IS_LOW);//first thing to do on re-call
		}
		
}

/*########################################Samy_EDF_Edits_End########################################*/	




/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */


int main( void )
{
	xSerialPortInitMinimal(ser9600);
	Q_T1_Button1 = xQueueCreate(1, sizeof(uint8));
	Q_T2_Button2 = xQueueCreate(1, sizeof(uint8));
	Q_T3_String = xQueueCreate(1, 100*sizeof(char));
	
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
    /* Create Tasks here */

xTaskPeriodicCreate(T1_Button1	,/* Function that implements the task. */
					"Task1"		,/* Text name for the task. */		
					100			,/* Stack size in words, not bytes. */
					NULL		,/* Parameter passed into the task. */
					1			,/* Priority at which the task is created. */
					&T1_Handle	,
					T1_Period	);
	
xTaskPeriodicCreate(T2_Button2,
					"Task2"		,		
					100			,
					NULL		,
					1			,
					&T2_Handle	,
					T2_Period	);
					
xTaskPeriodicCreate(T3_PeriodicString,
					"Task3"		,		
					100			,
					NULL		,
					1			,
					&T3_Handle	,
					T3_Period	);
					
xTaskPeriodicCreate(T4_UART_Print,
					"Task4"		,		
					100			,
					NULL		,
					1			,
					&T4_Handle	,
					T4_Period	);
					
xTaskPeriodicCreate(T5_Load1,
					"Task5"		,		
					100			,
					NULL		,
					1			,
					&T5_Handle	,
					T5_Period	);
					
xTaskPeriodicCreate(T6_Load2,
					"Task6"		,		
					100			,
					NULL		,
					1			,
					&T6_Handle	,
					T6_Period	);
					


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


