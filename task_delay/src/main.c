/**
  ******************************************************************************
  * @file    main.c

  * @author  Firas Hamdi
  * @version V1.0
  * @date    15-December-2013
  * @brief   Using vTaskDelay API to block the task
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "freeRTOS.h"
#include "task.h"
#include <stdint.h>

//Function prototypes
void Task1Handler(void* params);
void Task2Handler(void* params);
void GPIO_LED_init();

//Tasks handles
TaskHandle_t Task1Handle=NULL;
TaskHandle_t Task2Handle=NULL;


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

	//Hardware init
	GPIO_LED_init();

	//Tasks creation
	xTaskCreate(Task1Handler,"Task1",400,NULL,2,&Task1Handle);
	xTaskCreate(Task2Handler,"Task2",400,NULL,3,&Task2Handle);

	//Start the scheduler
	vTaskStartScheduler();

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
	TickType_t	ticks=100;
	while(1)
	{
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		vTaskDelay(ticks);
	}
}

void Task2Handler(void* params)
{
	TickType_t	ticks=1000;
	while(1)
	{
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		vTaskDelay(ticks);
	}
}

void vApplicationIdleHook()
{
	//Send the processor into sleep mode to save power
	__WFI();
}




