/*
 * msp.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Admin
 */
#include "main.h"

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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	//1. Enable the clock for TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2.Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3.Setup the priority for TIM6
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);
}
