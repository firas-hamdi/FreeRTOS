/**
  ******************************************************************************
  * @file    main.c

  * @author  Firas Hamdi
  * @version V1.0
  * @date    01-December-2013
  * @brief   Use binary semaphore to synchronize between two tasks
  ******************************************************************************
*/
#include "stm32f4xx.h"
#include "freeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <stdint.h>

//Function prototypes
void Task1Handler(void* params);
void Task2Handler(void* params);
void GPIO_LED_init();

//Tasks handles
TaskHandle_t Task1Handle=NULL;
TaskHandle_t Task2Handle=NULL;

//Queue handle
QueueHandle_t QueueHandle=NULL;

//Semaphore handle
SemaphoreHandle_t SemHandle=NULL;

int main(void)
{
	DWT->CTRL |=1;	//Recorder timestamp
	//Activate the HSI in the default state
	RCC_DeInit();
	//Update the system core clock
	SystemCoreClockUpdate();

	//Start recording events
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	//Peripheral initialization
	GPIO_LED_init();

	//Create the queue
	QueueHandle=xQueueCreate(1,sizeof(uint8_t));
	//Create the binary semaphore
	vSemaphoreCreateBinary(SemHandle);

	if((QueueHandle !=NULL)&&(SemHandle!=NULL))
	{
		//Create the tasks
		xTaskCreate(Task1Handler,"Manager-Task",400,NULL,2,&Task1Handle);
		xTaskCreate(Task2Handler,"Employee-Task",400,NULL,1,&Task2Handle);

		//The binary semaphore must be given first because it is initially empty, so it can not be taken directly
		xSemaphoreGive(SemHandle);

		//Start the scheduler
		vTaskStartScheduler();
	}

	for(;;);
}

void GPIO_LED_init()
{
	//Enable the GPIOD peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//GPIOD12 Init
	GPIO_TypeDef* GPIOx=GPIOD;
	GPIO_InitTypeDef GPIO_cfg;
	GPIO_cfg.GPIO_Pin=GPIO_Pin_12;
	GPIO_cfg.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_cfg.GPIO_OType=GPIO_OType_PP;
	GPIO_cfg.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_cfg.GPIO_Speed=GPIO_Low_Speed;
	GPIO_Init(GPIOx,&GPIO_cfg);
}

void Task1Handler(void* params)
{
	uint8_t queueData;
	BaseType_t sendStatus=NULL;

	while(1)
	{
		//Generate the random data in the queue
		queueData=(rand()&0xFF);
		//Post the data in the queue
		sendStatus=xQueueSend(QueueHandle,&queueData,portMAX_DELAY);
		if(sendStatus!=pdPASS)
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_RESET);
		}
		else
		{
			//Notify the task with less priority that there is data in the queue
			xSemaphoreGive(SemHandle);
			//Yield the processor
			taskYIELD();
		}
	}
}

void Task2Handler(void* params)
{
	uint8_t queueData;
	BaseType_t receiveStatus=pdFALSE;
	while(1)
	{
		//Take the sempahore
		xSemaphoreTake(SemHandle,0);
		//Receive the data from the queue
		receiveStatus=xQueueReceive(QueueHandle,&queueData,0);
		if(receiveStatus==pdPASS)
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_SET);
		}
	}
}

void vApplicationIdleHook()
{
	//Send the processor into sleep mode to save power
	__WFI();
}




