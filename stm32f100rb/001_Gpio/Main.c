#include "stm32f10x.h"   

void gpioConfig(){

	GPIO_InitTypeDef GPIOInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
}

void delay(uint32_t ebru){
		while(ebru--){		
		}		
}

int main(){
	gpioConfig();
	while(1){
		
	
	
	}
	
}


/*



*/	













