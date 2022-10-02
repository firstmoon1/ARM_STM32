#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

float distance=0;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//TRIG PIN CONFIG
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	
	//ECHO PIN CONFIG
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStructure);
}

/*
 24'ü aşagıdaki hesaplamadan bulduk, 24 ile carptık mesela time=1 olursa ve 24 ile carparsak 1 microsaniyeyi bulmus oldu gecen süre olarak

*/
void delayUS(uint32_t time){
  uint32_t newTime=time*24;
	while(newTime--);
}


float HCSR04_Read(){
	uint32_t time=0;
	float tempDistance=0;
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delayUS(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delayUS(10);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delayUS(10);
	
	while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		time++;
		delayUS(1);
	}
	
	
	
	tempDistance=(float)time/58;
	if(tempDistance>400) tempDistance=400;
	if(tempDistance<2) tempDistance=2;
	delayUS(10000);
	
	return tempDistance;
}

int main(){
	gpioConfig();
	
	while(1){
		
		distance=HCSR04_Read();
		
	}	

}

/*

clock hızımız =24000000   , 24*10^6  aslında

1 cycle=(1/24*10^6) =0.0000000417 saniye  =41.7 nano saniye =0.0417us (us=micro saniye)

1 cycle       0.0417us
?             1us
------------------------
? =23.98=24 cycle dır

farklı clock hızımız var ise 24*10^6 yerine onu yazarsın

24 ile carptık mesela time=1 olursa ve 24 ile carparsak 1 microsaniyeyi bulmus oldu gecen süre olarak

*/

