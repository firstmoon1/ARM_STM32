#include "stm32f10x.h"               


void confi(){
	
	GPIO_InitTypeDef concon;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); // for led
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);// for button
	//led config
	concon.GPIO_Mode=GPIO_Mode_Out_PP;
	concon.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	concon.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&concon);
	/*
	//button config
	concon.GPIO_Mode=GPIO_Mode_IPD; // pull down
	concon.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOC,&concon);
	*/
}

int main(){
	confi();
	uint32_t i=0;
	while(1){
		//btn=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
		
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			for(i=0;i<360000;i++);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			for(i=0;i<360000;i++);
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			for(i=0;i<360000;i++);
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			for(i=0;i<360000;i++);
		
	}
}
	





