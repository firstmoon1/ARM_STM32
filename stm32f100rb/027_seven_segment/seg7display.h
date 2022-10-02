
   
#ifndef __seg7display_H
#define __seg7display_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"                  
#include "stm32f10x_gpio.h"            
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//port definition
#define  portA GPIOA
#define  portB GPIOB
#define  portC GPIOC

//pin definition
#define  PIN0  (uint16_t)GPIO_Pin_0
#define  PIN1  (uint16_t)GPIO_Pin_1
#define  PIN2  (uint16_t)GPIO_Pin_2
#define  PIN3  (uint16_t)GPIO_Pin_3
#define  PIN4  (uint16_t)GPIO_Pin_4
#define  PIN5  (uint16_t)GPIO_Pin_5
#define  PIN6  (uint16_t)GPIO_Pin_6
#define  PIN7  (uint16_t)GPIO_Pin_7
#define  PIN8  (uint16_t)GPIO_Pin_8
#define  PIN9  (uint16_t)GPIO_Pin_9
#define  PIN10 (uint16_t)GPIO_Pin_10
#define  PIN11 (uint16_t)GPIO_Pin_11
#define  PIN12 (uint16_t)GPIO_Pin_12
#define  PIN13 (uint16_t)GPIO_Pin_13
#define  PIN14 (uint16_t)GPIO_Pin_14
#define  PIN15 (uint16_t)GPIO_Pin_15


#define  NUMBERSIZE 10
#define  LETTERSIZE 8
#define  CHARACTERSIZE 4
#define  CLEANDISPLAYSIZE 6

void seg7DisplayInit(GPIO_TypeDef* port, uint16_t pin0,
                                                             uint16_t pin1,
                                                             uint16_t pin2,
                                                             uint16_t pin3,
                                                             uint16_t pin4,
                                                             uint16_t pin5,
                                                             uint16_t pin6);
                                        
void printNumber(GPIO_TypeDef* port,uint16_t number);
void printLetter(GPIO_TypeDef* port,char letter);
void countForward(GPIO_TypeDef* port,uint32_t time);
void countBackward(GPIO_TypeDef* port,uint32_t time);
void cleanDisplay(GPIO_TypeDef* port);
void delay(uint32_t time);
void randomNumberGenerator(GPIO_TypeDef* port);


#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
