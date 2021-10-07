/*
 * msp.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Admin
 */
#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
	//Low level processor specific inits

	//1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; // usg fault, memory fault, bus fault system exception

	//3. configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	//1.Enable the clock for the USART peripheral and GPIOA peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//2.Do the pin muxing configurations
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2; //USART2 TX
	HAL_GPIO_Init(GPIOA,&gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3; //USART2 RX
	HAL_GPIO_Init(GPIOA,&gpio_uart);
	//3.Enable the IRQ and set up the priority
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn,15,0);
}
