/*
 * it.c
 *
 *  Created on: Sep 23, 2021
 *      Author: Admin
 */

void Systick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
