/*
 * Mutex_main.c
 * @author  Firas Hamdi
 * @version V1.0
 * @date    01-September-2016
 * @brief   Use the mutex to ensure mutual exclusion between
 * tasks using the shared ressource stdout
 */
#include "stm32f4xx.h"
#include "freeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//function prototypes
void Task1Handler(void* params);
void Task2Handler(void* params);

//Tasks handles
TaskHandle_t Task1Handle=NULL;
TaskHandle_t Task2Handle=NULL;

//Mutex handle
SemaphoreHandle_t MutexHandle=NULL;

int main(void)
{
	printf("Welcome\n");
	DWT->CTRL |=1;	//Recorder timestamp
	//Activate the HSI in the default state
	RCC_DeInit();
	//Update the system core clock
	SystemCoreClockUpdate();

	//Start recording events
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	//Create the mutex
	MutexHandle=xSemaphoreCreateMutex();
	if((MutexHandle!=NULL))
	{
		//Create the tasks
		xTaskCreate(Task1Handler,"Manager-Task",400,NULL,1,&Task1Handle);
		xTaskCreate(Task2Handler,"Employee-Task",400,NULL,2,&Task2Handle);

		//Start the scheduler
		vTaskStartScheduler();
	}

	for(;;);
}


void Task1Handler(void* params)
{
	while(1)
	{
		xSemaphoreTake(MutexHandle,0);
		printf("Task 1 running\n");
		xSemaphoreGive(MutexHandle);
	}
}

void Task2Handler(void* params)
{

	while(1)
	{
		printf("Task 2 is running\n");
		vTaskDelay(rand()&0xF);
	}
}

void vApplicationIdleHook()
{
	//Send the processor into sleep mode to save power
	__WFI();
}






