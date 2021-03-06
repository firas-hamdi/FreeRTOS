/**
  ******************************************************************************
  * @file    main.c


  * @author  Firas Hamdi
  * @version V1.0
  * @date    01-December-2013
  * @brief   How to create a task using xTaskCreate API.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "freeRTOS.h"
#include "task.h"
#include <string.h>
#include <stdint.h>

//Function prototypes
void buttonTaskHandler(void* params);
void LEDTaskHandler(void* params);
void GPIO_LED_init();
void GPIO_Button_init();

//Tasks handles
TaskHandle_t * ButtonTaskHandle=NULL;
TaskHandle_t * LEDTaskHandle=NULL;

#define NOT_PRESSED		0
#define PRESSED			!(NOT_PRESSED)

//Global variables
uint8_t Button_Status=NOT_PRESSED;

int main(void)
{
	//Activate the HSI in the default state
	RCC_DeInit();
	//Update the system core clock
	SystemCoreClockUpdate();

	//Hardware init
	GPIO_LED_init();
	GPIO_Button_init();

	//Create two task: Task for the button and task for the LED
	xTaskCreate(buttonTaskHandler,"Button task",configMINIMAL_STACK_SIZE,NULL,1,ButtonTaskHandle);
	xTaskCreate(LEDTaskHandler,"LED task",configMINIMAL_STACK_SIZE,NULL,1,LEDTaskHandle);

	//Start the scheduler
	vTaskStartScheduler();
	for(;;);
}


void buttonTaskHandler(void* params)
{
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			Button_Status=PRESSED;
		}
		else
		{
			Button_Status=NOT_PRESSED;
		}
	}
}

void LEDTaskHandler(void* params)
{
	while(1)
	{
		if (Button_Status==PRESSED)
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_RESET);
		}
	}
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

void GPIO_Button_init()
{
	//Enable the GPIOD peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//GPIOD12 Init
	GPIO_TypeDef* GPIOx=GPIOA;
	GPIO_InitTypeDef GPIO_cfg;
	GPIO_cfg.GPIO_Pin=GPIO_Pin_0;
	GPIO_cfg.GPIO_Mode=GPIO_Mode_IN;
	GPIO_cfg.GPIO_OType=GPIO_OType_PP;
	GPIO_cfg.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_cfg.GPIO_Speed=GPIO_Low_Speed;
	GPIO_Init(GPIOx,&GPIO_cfg);
}
