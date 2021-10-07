/*
 * it.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Admin
 */

#include "main.h"

extern UART_HandleTypeDef huart2;

void Systick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
