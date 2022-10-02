
#include "stm32f10x.h"  

uint32_t flag=0,btn=0;

void gpioConfig(){

	GPIO_InitTypeDef GPIOInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	

	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	
	GPIO_Init(GPIOC,&GPIOInitStructure);
}
void delay(uint16_t tt){
	while(tt--);
}

int main(){	
	gpioConfig();	
	while(1){
		
		btn=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
		while( GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)!=0 ){ // bu while olayi: elin basiliyken  hala basiyormus gibi olmasin diye 
			// cem göstermisti button bouncy  olayi 
			
			if(btn==1){
				delay(360000);
				if(flag==0){
					flag=1;
					GPIO_SetBits(GPIOB,GPIO_Pin_0);
				}else{
					flag=0;
					GPIO_ResetBits(GPIOB,GPIO_Pin_0);
				}
			}
		}
	}	
}