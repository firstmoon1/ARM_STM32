/*
 * BMP180.c
 *
 *  Created on: Apr 1, 2023
 *      Author: ilkay
 */

#include "BMP180.h"

extern I2C_HandleTypeDef hi2c1;

void BMP180_Init(){

	if(HAL_I2C_IsDeviceReady(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 1, 1000000) != HAL_OK){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	}
		BMP180_Get_Calibration_Value();
}


void BMP180_Get_Calibration_Value(){

	int a=0;
        //calibBuff : i2c 8 bitlik okuma yapar ,bmp180 calibration coefficient 16 bit islem görüyor, bu nedenle 2 katı yer aldık,sonra ardı ardına veriyi depoluyacagız.
	uint8_t calibBuff[BMP180_CALIBRATION_VALUE_LENGTH] = {0};
	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, BMP180_CALIBRATION_START_ADDRESS, 1, calibBuff, BMP180_CALIBRATION_VALUE_LENGTH,100000); // 1 byte için 1 yazdık cünkü 0xAA nin uzunlugu 1 byte=8 bit

	// 8 bit saga kaydırdık cünkü 0-7 arası ayrı cünkü MSB ,8-15 arası ayrı alınıyor cünkü LSB, datasheet inde yazıyor.
	AC1 = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2; // calibBuff 8 bit, biz 16 bite cevirdik.
	AC2 = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	AC3 = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	AC4 = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	AC5 = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	AC6 = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	B1  = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	B2  = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	MB  = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	MC  = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;
	MD  = (uint16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2;

	if(AC1 == 0x00  | AC1 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(AC2 == 0x00  | AC2 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(AC3 == 0x00  | AC3 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(AC4 == 0x00  | AC4 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(AC5 == 0x00  | AC5 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(AC6 == 0x00  | AC6 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(B1 == 0x00  | B1 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(B2 == 0x00  | B2 == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(MB == 0x00  | MB == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(MC == 0x00  | MC == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
	if(MD == 0x00  | MD == 0xFFFF){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		while(1);
	}
}



void BMP180_Get_Temparature(){

	tX1 = (((int32_t)unCompensatedTemperature - (int32_t)AC6 ) * (int32_t)AC5) / 32768;
	tX2 = ((int32_t)MC * 2048 ) / (tX1 + (int32_t)MD);
	if(tX1 == 0 && MD == 0){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		while(1);
	}
	tB5 = tX1+tX2;
	temperature = ((float)(tB5+8) / 16)*0.1;

}

void BMP180_Get_Uncompansated_Temparature(void){
	uint8_t wData[1],rData[2] ={0};
	wData[0] = 0x2E;
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(5); // 5ms
	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 2, 100000);
	unCompensatedTemperature =(uint16_t)((rData[0] << 8) | rData[1]); // bitwise da, veya kapısı toplamadır yani

}


void BMP180_Get_Pressure(void){
	BMP180_Get_Uncompansated_Pressure();
	pB6 =tB5 - 4000;
	pX1 =(B2* (pB6*pB6/4096))/2048;
	pX2 =AC2 * pB6 /2048;
	pX3 =pX1+pX2;
	pB3 =(((AC1 *4 +pX3)<< (uint8_t)0x03 )+2)/4;
	pX1 =AC3*pB6/8192;
	pX2 =(B1 * (pB6*pB6 /4096))/65536;
	pX3 =((pX1+pX2)+2)/4;
	pB4 = AC4 * (unsigned long)(pX3+32768) /32768;
	pB7 =((unsigned long)unCompensatedPressure-pB3)*(50000 >> (uint8_t)0x03);
	if(pB7 < 0x80000000){
		presure=(pB7*2)/pB4;
	}else{
		presure =(pB7/pB4)*2;
	}
	pX1 =(presure/256)*(presure/256);
	pX1=(pX1*3038)/65536;
	pX2=(-7357 *presure)/65536;
	presure = presure + (pX1 + pX2+3791 )/16;
	presureATM=presure * 0.0000986923;



}
void BMP180_Get_Uncompansated_Pressure(void){
	uint8_t rData[3]={0};
	uint8_t wData[1];
	wData[0] = 0x34 | (0x03 <<6 );
	HAL_I2C_Mem_Write(&hi2c1, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000);
	HAL_Delay(26); //26ms
	HAL_I2C_Mem_Read(&hi2c1, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 3, 100000);

	unCompensatedPressure = (rData[0] << 16 | rData[1] << 8 | rData[2]) >> (8 - (uint8_t)(0x03));

}








