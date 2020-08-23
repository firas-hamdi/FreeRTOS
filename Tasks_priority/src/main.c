/**
  ******************************************************************************
  * @file    main.c

  * @author  Firas Hamdi
  * @version V1.0
  * @date    13-December-2014
  * @brief   Change tasks priorities using uxTaskPriorityGet and vTaskPrioritySet APIs
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "freeRTOS.h"
#include "task.h"
#include <stdint.h>
#define FALSE		0
#define TRUE		1

//Function prototypes
void Task1Handler(void* params);
void Task2Handler(void* params);
void GPIO_LED_init();
void GPIO_Button_init();
void interrupt_config();
void delay(uint32_t time_in_ms);

//Task handles
TaskHandle_t Task1Handle=NULL;
TaskHandle_t Task2Handle=NULL;
uint8_t prio_switch=FALSE;

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

	//Hardware initialization
	GPIO_LED_init();
	GPIO_Button_init();
	interrupt_config();

	//Tasks creation
	xTaskCreate(Task1Handler,"Task1",400,NULL,2,&Task1Handle);
	xTaskCreate(Task2Handler,"Task2",400,NULL,3,&Task2Handle);

	//Start scheduler
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

void Task1Handler(void* params)
{
	UBaseType_t p1,p2;
	while(1)
	{
		if(prio_switch==TRUE)
		{
			p1=uxTaskPriorityGet(Task1Handle);
			p2=uxTaskPriorityGet(Task2Handle);
			prio_switch=FALSE;
			vTaskPrioritySet(Task2Handle,p1);
			vTaskPrioritySet(Task1Handle,p2);
		}
		else
		{
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
			delay(100);
		}
	}
}

void Task2Handler(void* params)
{
	UBaseType_t p1,p2;
		while(1)
		{
			if(prio_switch==TRUE)
			{
				p1=uxTaskPriorityGet(Task1Handle);
				p2=uxTaskPriorityGet(Task2Handle);
				prio_switch=FALSE;
				vTaskPrioritySet(Task2Handle,p1);
				vTaskPrioritySet(Task1Handle,p2);
			}
		else
		{
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
			delay(1000);
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
	prio_switch=TRUE;
	traceISR_EXIT();

}

void delay(uint32_t time_in_ms)
{
	uint32_t current_ticks= xTaskGetTickCount();
	uint32_t time_in_ticks= (time_in_ms*configTICK_RATE_HZ)/1000;
	while(xTaskGetTickCount()<(current_ticks+time_in_ticks));
}
