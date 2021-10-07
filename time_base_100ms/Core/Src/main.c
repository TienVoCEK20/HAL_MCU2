/*
 * main.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Admin
 */

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void Error_handler();
void TIMER6_INIT(void);
void GPIO_Init(void);

TIM_HandleTypeDef htimer6;

int main()
{
	HAL_Init();

	SystemClockConfig();

	GPIO_Init();

	TIMER6_INIT();

	//Lets start timer
	HAL_TIM_Base_Start(&htimer6);

	while(1)
	{
	/*loop until the update event flag is set */
	while(! (TIM6->SR & TIM_SR_UIF) );

	TIM6->SR = 0;
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	}


	return 0;
}
void SystemClockConfig(void)
{

}
void GPIO_Init(void)
{
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}
void TIMER6_INIT(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 24;
	htimer6.Init.Period = 64000-1;
	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		Error_handler();
	}
}
void Error_handler(void)
{

}
