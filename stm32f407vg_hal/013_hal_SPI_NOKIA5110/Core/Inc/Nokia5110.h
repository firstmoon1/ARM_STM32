/*
 * Nokia5110.h
 *
 *  Created on: Apr 2, 2023
 *      Author: ilkay
 */

#ifndef INC_NOKIA5110_H_
#define INC_NOKIA5110_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_tx;



bool Nokia5110_Init(void);
void Nokia5110_Reset(void);
bool Nokia5110_Write(uint8_t address,uint8_t data);
void Nokia5110_Clear(void);



#endif /* INC_NOKIA5110_H_ */
