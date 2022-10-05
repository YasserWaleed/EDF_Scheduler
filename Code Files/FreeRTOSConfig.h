/*
 * FreeRTOS V202112.00
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
#include <GPIO.h>

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
extern int T1_EntryTime, T1_TotalTime;
extern int T2_EntryTime, T2_TotalTime;
extern int T3_EntryTime, T3_TotalTime;
extern int T4_EntryTime, T4_TotalTime;
extern int T5_EntryTime, T5_TotalTime;
extern int T6_EntryTime, T6_TotalTime;
extern int System_Time;
extern int CPU_Load;

#define configUSE_EDF_SCHEDULER 			 1
#define configUSE_APPLICATION_TASK_TAG 1

/* Trace Hooks */
#define traceTASK_SWITCHED_OUT() do\
	{		 if ((int)pxCurrentTCB->pxTaskTag == 1){GPIO_write(PORT_1, PIN2, PIN_IS_LOW); T1_TotalTime += T1TC - T1_EntryTime;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 2){GPIO_write(PORT_1, PIN3, PIN_IS_LOW); T2_TotalTime += T1TC - T2_EntryTime;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 3){GPIO_write(PORT_1, PIN4, PIN_IS_LOW); T3_TotalTime += T1TC - T3_EntryTime;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 4){GPIO_write(PORT_1, PIN5, PIN_IS_LOW); T4_TotalTime += T1TC - T4_EntryTime;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 5){GPIO_write(PORT_1, PIN6, PIN_IS_LOW); T5_TotalTime += T1TC - T5_EntryTime;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 6){GPIO_write(PORT_1, PIN7, PIN_IS_LOW); T6_TotalTime += T1TC - T6_EntryTime;}\
	System_Time = T1TC;\
	CPU_Load = ((T1_TotalTime + T2_TotalTime + T3_TotalTime + T4_TotalTime + T5_TotalTime + T6_TotalTime) / (float) System_Time) * 100;\
	}while(0)

													

	#define traceTASK_SWITCHED_IN() do\
	{		 if ((int)pxCurrentTCB->pxTaskTag == 1){GPIO_write(PORT_1, PIN2, PIN_IS_HIGH); T1_EntryTime = T1TC;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 2){GPIO_write(PORT_1, PIN3, PIN_IS_HIGH); T2_EntryTime = T1TC;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 3){GPIO_write(PORT_1, PIN4, PIN_IS_HIGH); T3_EntryTime = T1TC;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 4){GPIO_write(PORT_1, PIN5, PIN_IS_HIGH); T4_EntryTime = T1TC;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 5){GPIO_write(PORT_1, PIN6, PIN_IS_HIGH); T5_EntryTime = T1TC;}\
	else if ((int)pxCurrentTCB->pxTaskTag == 6){GPIO_write(PORT_1, PIN7, PIN_IS_HIGH); T6_EntryTime = T1TC;}\
	}while(0)

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
#define INCLUDE_vTaskDelay				1

#endif /* FREERTOS_CONFIG_H */
