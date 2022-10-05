/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
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


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

extern BaseType_t xTaskPeriodicCreate( TaskFunction_t pxTaskCode,
                            const char * const pcName, 
                            const configSTACK_DEPTH_TYPE usStackDepth,
                            void * const pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * const pxCreatedTask,
														TickType_t period);

/* Reset timer 1 */
/*static void timer1Reset(void) { T1TCR |= 0x2; T1TCR &= ~0x2; } */

/* Initialize Timer 1 */
static void configTimer1(void) { T1PR = 1000; T1TCR |= 0x1; }

/* Hardware Setup */
static void prvSetupHardware( void ) { xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE); GPIO_init(); configTimer1(); VPBDIV = mainBUS_CLK_FULL; }

void vApplicationTickHook ( void ) { GPIO_write(PORT_1, PIN1, PIN_IS_HIGH);	GPIO_write(PORT_1, PIN1, PIN_IS_LOW); }

void vApplicationIdleHook ( void ) { GPIO_write(PORT_1, PIN0, PIN_IS_HIGH); }

/*-----------------------------------------------------------*/
														
uint8_t B1_RisingMessage [4] = "B1+\n";
uint8_t B2_RisingMessage [4] = "B2+\n";
uint8_t B1_FallingMessage[4] = "B1-\n";
uint8_t B2_FallingMessage[4] = "B2-\n";
uint8_t PeriodicMessage  [3] = "PM\n";
														
uint8_t T1_message = '.';
uint8_t T2_message = '.';
uint8_t T3_message = '.';

pinState_t B1_InitialReading;
pinState_t B2_InitialReading;
pinState_t B1_CurrentReading;
pinState_t B2_CurrentReading;

int T1_EntryTime = 0, T1_TotalTime;
int T2_EntryTime = 0, T2_TotalTime;
int T3_EntryTime = 0, T3_TotalTime;
int T4_EntryTime = 0, T4_TotalTime;
int T5_EntryTime = 0, T5_TotalTime;
int T6_EntryTime = 0, T6_TotalTime;
int System_Time = 0;
int CPU_Load;
		
/* Criteria 3 : Tasks Implementations */

/* Monitor Button1, Send a message upon edge detection */
void T1_Button1 ( void * pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	B1_InitialReading = GPIO_read(PORT_1 , PIN8);
	vTaskSetApplicationTaskTag(NULL,  (TaskHookFunction_t) 1);
	for(;;)
	{
		//xSerialPutChar( '1' );
		GPIO_write(PORT_1, PIN0, PIN_IS_LOW);
		B1_CurrentReading = GPIO_read(PORT_1 , PIN8);
		if( B1_CurrentReading == PIN_IS_HIGH &&  B1_InitialReading == PIN_IS_LOW)
		{
			T1_message = 'R';
		}
		else if (B1_CurrentReading == PIN_IS_LOW &&  B1_InitialReading == PIN_IS_HIGH)
		{
			T1_message = 'F';			
		}
		vTaskDelayUntil(&xLastWakeTime, 50);
		B1_InitialReading = B1_CurrentReading;
	}
}

/* Monitor Button2, Send a message upon edge detection */
void T2_Button2 ( void * pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	B2_InitialReading = GPIO_read(PORT_1 , PIN9);
	vTaskSetApplicationTaskTag(NULL,  (TaskHookFunction_t) 2);
	for(;;)
	{
		//xSerialPutChar( '2' );
		GPIO_write(PORT_1, PIN0, PIN_IS_LOW);
		B2_CurrentReading = GPIO_read(PORT_1 , PIN9);
		if( B2_CurrentReading == PIN_IS_HIGH &&  B2_InitialReading == PIN_IS_LOW)
		{
			T2_message = 'R';
		}
		else if (B2_CurrentReading == PIN_IS_LOW &&  B2_InitialReading == PIN_IS_HIGH)
		{
			T2_message = 'F';	
		}
		vTaskDelayUntil(&xLastWakeTime, 50);
		B2_InitialReading = B2_CurrentReading;
	}
}

/* Send periodic message every 100ms */
void T3_Periodic ( void * pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,  (TaskHookFunction_t) 3); 
	for(;;) 
	{
		//xSerialPutChar( '3' );
		GPIO_write(PORT_1, PIN0, PIN_IS_LOW);
		T3_message = 'P';
		vTaskDelayUntil(&xLastWakeTime, 100);
	}
}

/* Write on UART messages of other tasks */
void T4_UART ( void * pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	uint16_t i;
	vTaskSetApplicationTaskTag(NULL,  (TaskHookFunction_t) 4);
	for(;;) 
	{
		//xSerialPutChar( '4' );
		GPIO_write(PORT_1, PIN0, PIN_IS_LOW);
		if (T1_message == 'R') { for(i = 0; i < 4; i++){xSerialPutChar( B1_RisingMessage [i]);} T1_message = '.';}
		if (T1_message == 'F') { for(i = 0; i < 4; i++){xSerialPutChar( B1_FallingMessage[i]);} T1_message = '.';}
		if (T2_message == 'R') { for(i = 0; i < 4; i++){xSerialPutChar( B2_RisingMessage [i]);} T2_message = '.';}
		if (T2_message == 'F') { for(i = 0; i < 4; i++){xSerialPutChar( B2_FallingMessage[i]);} T2_message = '.';}
		if (T3_message == 'P') { for(i = 0; i < 3; i++){xSerialPutChar( PeriodicMessage  [i]);} T3_message = '.';}
		vTaskDelayUntil( &xLastWakeTime, 20 );
	}
}

void T5_Load1 ( void * pvParameters )
{
	uint16_t i = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,  (TaskHookFunction_t) 5); 
	for(;;)
	{
		//xSerialPutChar( '5' );
		GPIO_write(PORT_1, PIN0, PIN_IS_LOW);
		for(i = 0; i < 29800; i++){}
		vTaskDelayUntil(&xLastWakeTime, 10);
	}
}

void T6_Load2 ( void * pvParameters )
{
	uint32_t i = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,  (TaskHookFunction_t) 6); 
	for(;;)
	{
		//xSerialPutChar( '6' );
		GPIO_write(PORT_1, PIN0, PIN_IS_LOW);
		for(i = 0; i < 87800; i++){}
		vTaskDelayUntil(&xLastWakeTime, 100);
	}
}

/* Tasks Handles */
TaskHandle_t T1;
TaskHandle_t T2;
TaskHandle_t T3;
TaskHandle_t T4;
TaskHandle_t T5;
TaskHandle_t T6;

/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */

int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	
	/* Criteria3 : Task Creation */
	xTaskPeriodicCreate( T1_Button1		, "B1" , 100, (void *) NULL, 1, &T1 , 50  );
	xTaskPeriodicCreate( T2_Button2		, "B2" , 100, (void *) NULL, 1, &T2 , 50  );
	xTaskPeriodicCreate( T3_Periodic	, "PT" , 100, (void *) NULL, 1, &T3 , 100  );
	xTaskPeriodicCreate( T4_UART			, "UR" , 100, (void *) NULL, 1, &T4 , 20  );
	xTaskPeriodicCreate( T5_Load1			, "L1" , 100, (void *) NULL, 1, &T5 , 10  );
	xTaskPeriodicCreate( T6_Load2			, "L2" , 100, (void *) NULL, 1, &T6 , 100  );
	
	/* Now all the tasks have been started - start the scheduler. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/
