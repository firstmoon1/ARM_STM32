/*
 * BMP180.h
 *
 *  Created on: Apr 1, 2023
 *      Author: ilkay
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stm32f4xx_hal.h"

// bmp180 device address
#define BMP180_DEVICE_WRITE_REGISTER_ADDRESS 0xEE
#define BMP180_DEVICE_READ_REGISTER_ADDRESS 0xEF
// i2c 8 bitlik okuma yapar ,bmp180 calibration coefficient 16 bit islem görüyor, bu nedenle 2 katı aldık ardı ardına vericez.
#define BMP180_CALIBRATION_VALUE_LENGTH 22 
// BMP180 calibration address
#define BMP180_CALIBRATION_START_ADDRESS 0xAA

extern I2C_HandleTypeDef hi2c1;
// Calibration values
uint16_t AC1,AC2,AC3,AC4,AC5,AC6,B1,B2,MB,MC,MD;

// temperature values
uint16_t unCompensatedTemperature;
float temperature;
uint32_t tX1,tX2,tB5;

// pressure values
uint32_t unCompensatedPressure;
float presure,presureATM;
int32_t pX1,pX2,pX3,pB3,pB4,pB6,pB7;


// functions
void BMP180_Init();
void BMP180_GetCalibration();
void BMP180_Get_Calibration_Value();
void BMP180_Get_Uncompansated_Temparature(void);
void BMP180_Get_Temparature(void);
void BMP180_Get_Uncompansated_Pressure(void);
void BMP180_Get_Pressure(void);


#endif /* INC_BMP180_H_ */
