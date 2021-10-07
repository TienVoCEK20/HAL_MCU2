/*
 * main.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Admin
 */

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define TRUE 1
#define FASLE 0

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler();

UART_HandleTypeDef huart2;



int main()
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	char msg[100];
	memset(msg,0,sizeof(clk_init));
	HAL_Init();
	memset(&osc_init,0,sizeof(osc_init));
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;

	SystemClockConfig();

	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}
	memset(&clk_init,0,sizeof(clk_init));
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
							RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_SYSCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init,FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_handler();
	}
    __HAL_RCC_ADC1_CLK_DISABLE();

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	UART2_Init();

	sprintf(msg,"SYSCLK: %ld\r\n",HAL_RCC_GetSysClockFreq());

	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	while(1);

	return 0;
}
void SystemClockConfig(void)
{

}
void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_handler();
	}
}

void Error_handler(void)
{

}
