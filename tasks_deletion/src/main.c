/**
  ******************************************************************************
  * @file    main.c

  * @author  Firas Hamdi
  * @version V1.0
  * @date    15-January-2015
  * @brief   Delete tasks using vTasksDelete API.
  * NOTE 	 The deleted tasks TCBand stack will be deleted only
  * when the Idle task runs
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "freeRTOS.h"
#include "task.h"

//Tasks handle
TaskHandle_t task1Handle=NULL;
TaskHandle_t task2Handle=NULL;

//Function prototypes
void task1_handler(void* params);
void task2_handler(void* params);
void GPIO_LED_init();
void GPIO_Button_init();
void delay(uint32_t time_in_ms);

int main(void)
{
	//SEGGER SysView timestamp
	DWT->CYCCNT |=1;

	//Start recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	GPIO_LED_init();
	GPIO_Button_init();

	//Tasks creation
	xTaskCreate(task1_handler,"Task1",500,NULL,1,&task1Handle);
	xTaskCreate(task2_handler,"Task2",500,NULL,2,&task2Handle);

	//Start scheduler
	vTaskStartScheduler();
	for(;;);
}


void task1_handler(void* params)
{
	while(1)
	{
		SEGGER_SYSVIEW_Print("task1");
		delay(200);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	}
}

void task2_handler(void* params)
{
	while(1)
	{
		SEGGER_SYSVIEW_Print("task2");
		if (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			delay(1000);
			GPIO_WriteBit(GPIOD,GPIO_Pin_12,Bit_SET);
		}
		else
		{
			//Delete the task 2
			vTaskDelete(NULL);
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

void delay(uint32_t time_in_ms)
{
	TickType_t current_tick=xTaskGetTickCount();
	TickType_t time_in_ticks=(time_in_ms*configTICK_RATE_HZ)/1000;
	while(xTaskGetTickCount()<current_tick+time_in_ticks);
}

