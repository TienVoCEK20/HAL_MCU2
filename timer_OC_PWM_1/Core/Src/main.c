/*
 * main.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Admin
 */
#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(uint8_t clock_freq);
void GPIO_Init(void);
void Error_handler(void);
void TIMER2_Init(void);
void TIMER6_Init(void);
void LSE_Configuration(void);
void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq);

TIM_HandleTypeDef htimer2;
TIM_HandleTypeDef htimer6;
UART_HandleTypeDef huart2;

uint32_t input_captures[2] = {0};
uint8_t count=1;
volatile uint8_t is_capture_done = FALSE;

uint32_t pulse1_value = 25000;
uint32_t pulse2_value = 12500;
uint32_t pulse3_value = 6250;
uint32_t pulse4_value = 3125;

volatile uint32_t ccr_content;

int main(void)
{
	uint32_t capture_difference =0;
	double timer2_cnt_freq=0;
	double timer2_cnt_res=0;
	double user_signal_time_period =0;
	double user_signal_freq=0;
	char usr_msg[100];
	HAL_Init();

	//SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);

	HAL_Init();


	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);

	GPIO_Init();

	UART2_Init();

	TIMER2_Init();

		if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_1) != HAL_OK)
		{
			Error_handler();
		}

		if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_2) != HAL_OK)
		{
			Error_handler();
		}

		if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_3) != HAL_OK)
		{
			Error_handler();
		}

		if ( HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_4) != HAL_OK)
		{
			Error_handler();
		}

	while(1);

	return 0;
}


void SystemClockConfig(uint8_t clock_freq )
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_Init.HSICalibrationValue = 16;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	     break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	     break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
	     break;

	  default:
	   return ;
	 }

		if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
	{
			Error_handler();
	}



	if (HAL_RCC_ClockConfig(&Clock_Init, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_handler();
	}


	/*Configure the systick timer interrupt frequency (for every 1 ms) */
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;
    uint8_t flash_latency=0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSI ;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 1;
	     break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 2;
	     break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
          flash_latency = 3;
	     break;

	  default:
	   return ;
	 }

		if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
	{
			Error_handler();
	}



	if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
	{
		Error_handler();
	}


	/*Configure the systick timer interrupt frequency (for every 1 ms) */
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);



 }

void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}

void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 9;
	htimer6.Init.Period = 50-1;
	if( HAL_TIM_Base_Init(&htimer6) != HAL_OK )
	{
		Error_handler();
	}

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
	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_handler();
	}


}


 void TIMER2_Init(void)
{
	 TIM_OC_InitTypeDef timer2OC_Config;

	 htimer2.Instance = TIM2;
	 htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	 htimer2.Init.Period = 10000-1;
	 htimer2.Init.Prescaler = 4999;
	 if ( HAL_TIM_IC_Init(&htimer2) != HAL_OK)
	 {
		 Error_handler();
	 }

	 timer2OC_Config.OCMode = TIM_OCMODE_PWM1 ;
	 timer2OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	 timer2OC_Config.Pulse = (htimer2.Init.Period * 25)/100;

	 if (HAL_TIM_PWM_ConfigChannel(&htimer2,&timer2OC_Config,TIM_CHANNEL_1))
	 {
		 Error_handler();
	 }
	 timer2OC_Config.Pulse = (htimer2.Init.Period * 45)/100;

	if (HAL_TIM_PWM_ConfigChannel(&htimer2,&timer2OC_Config,TIM_CHANNEL_2))
	{
		Error_handler();
	}
	timer2OC_Config.Pulse = (htimer2.Init.Period * 75)/100;

	if (HAL_TIM_PWM_ConfigChannel(&htimer2,&timer2OC_Config,TIM_CHANNEL_3))
	{
		Error_handler();
	}
	timer2OC_Config.Pulse = (htimer2.Init.Period * 95)/100;

	if (HAL_TIM_PWM_ConfigChannel(&htimer2,&timer2OC_Config,TIM_CHANNEL_4))
	{
		Error_handler();
	}
}

 void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
 {
    /* TIM3_CH1 toggling with frequency = 500 Hz */
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
 	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
 	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
    }

    /* TIM3_CH2 toggling with frequency = 1000 Hz */
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
 	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
 	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,ccr_content+pulse2_value);

    }

    /* TIM3_CH3 toggling with frequency = 2000 Hz */
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
 	   ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
 	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,ccr_content+pulse3_value);

    }

    /* TIM3_CH4 toggling with frequency = 4000 Hz */
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
 	    ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
 	   __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,ccr_content+pulse4_value);

    }
}

void LSE_Configuration(void)
{

	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_HSI,RCC_MCODIV_4);

}


 void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
#if 1
 if(! is_capture_done)
 {
	 if(count == 1)
	 {
		 input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		 count++;
	 }
	 else if (count == 2)
	 {
		 input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		 count =1;
		 is_capture_done = TRUE;
	 }

 }
#endif

}

void Error_handler(void)
{
	while(1);
}
