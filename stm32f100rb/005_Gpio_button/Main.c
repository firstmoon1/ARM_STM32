#include "stm32f10x.h"  


uint16_t btn1=0;

void config_yeri(){
	
	GPIO_InitTypeDef cursor;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); // led için GPIOB port aktif ettik
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); // buton için GPIOC port aktif ettik
	
	// led icin configurasyon
	cursor.GPIO_Mode=GPIO_Mode_Out_PP;
	cursor.GPIO_Pin=GPIO_Pin_0;
	cursor.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&cursor);
	
	// buton icin configurasyon
	cursor.GPIO_Mode=GPIO_Mode_IPD; // referans pdf'inde page 104 de pulldown pullup var VDD,VSS olan kisim
	cursor.GPIO_Pin=GPIO_Pin_6;
	// buton oldugu için  speed e gerek yoktur
	GPIO_Init(GPIOC,&cursor);
	
}

int main(){
	
	config_yeri();
	
	while(1){
		btn1=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
		if(btn1==1){
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		}else{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		}
		
	}
}

/*
pull down ,IPD ve direnci gnd kismina baglanir,ark olusmamasi ve direk buton ile gnd kisa devre 
olmasin diye  hoca 10k direnc koydu 

pull up ,IPU:  

vdd : 3.3 volt dur
GPIO ABCDE portlari hep APB2  hattina bagli stm32 vldiscovery pdf inde sayfa 8 de var diagram



*/

