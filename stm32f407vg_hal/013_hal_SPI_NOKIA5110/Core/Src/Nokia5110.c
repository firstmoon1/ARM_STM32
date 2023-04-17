/*
 * Nokia5110.c
 *
 *  Created on: Apr 2, 2023
 *      Author: ilkay
 */


#include "Nokia5110.h"


bool Nokia5110_Init(void){
	Nokia5110_Reset();



}
void Nokia5110_Reset(void){

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
}

