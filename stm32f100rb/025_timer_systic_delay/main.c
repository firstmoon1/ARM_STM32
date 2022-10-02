#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "delay.h"

void gpioConfig(){
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
  GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);

}

int main(){
	 
	gpioConfig();
	DelayInit();

	
	while(1){
		
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		delay_ms(1000); 
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		delay_ms(1000);

	}

}

/*
burda kütüphane olusturup sisteme ekledik
delay.c ve delay.h bizim kütüphanelerimiz
 burda yaptýgýmýz þey ,1000ms=1sn dir ,1sn de bir ledi yakýp söndürmek ,yanacak 1sn bekler,söner,1sn bekler ve repeat
*/

