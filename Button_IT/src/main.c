/**
  ******************************************************************************
  * @file    main.c

  * @author  Firas Hamdi
  * @version V1.0
  * @date    01-December-2013
  * @brief   Synchronize an interrupt and a task, and visualize the events using SEGGER SYSVIEW
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "freeRTOS.h"
#include "task.h"
#include <stdint.h>

//Function prototypes
void buttonTaskHandler(void* params);
void GPIO_LED_init();
void GPIO_Button_init();
void interrupt_config();

//Task handle
TaskHandle_t * ButtonTaskHandle=NULL;

#define NOT_PRESSED		0
#define PRESSED			1

//Global variables
uint8_t Button_Status=NOT_PRESSED;

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

	GPIO_LED_init();
	GPIO_Button_init();
	interrupt_config();

	xTaskCreate(buttonTaskHandler,"Button_interrupt",configMINIMAL_STACK_SIZE,NULL,1,ButtonTaskHandle);

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

void buttonTaskHandler(void* params)
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

void interrupt_config()
{
	//Enable SYSCFG peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	//Enable the EXTI peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT,ENABLE);
	//System configuration for EXTI line
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	//Configuration of EXTI line 0
	EXTI_InitTypeDef EXTI_cfg;
	EXTI_cfg.EXTI_Line=EXTI_Line0;
	EXTI_cfg.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_cfg.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_cfg.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_cfg);
	//NVIC configuration
	NVIC_SetPriority(EXTI0_IRQn,5);
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler()
{
	traceISR_ENTER();
	EXTI_ClearITPendingBit(EXTI_Line0);
	Button_Status^=1;
	traceISR_EXIT();

}
