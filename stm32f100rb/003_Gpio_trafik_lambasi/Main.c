#include "stm32f10x.h"               

void canan(){
	GPIO_InitTypeDef ilkay;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	ilkay.GPIO_Mode = GPIO_Mode_Out_PP;
	ilkay.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	ilkay.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&ilkay);	
	
}
void delay(uint32_t tt){
	while(tt--) {}
}

int main(){
	canan();
	int i=0,arr[3]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2};
	while(1){
		for(i=0;i<3;i++){
			GPIO_SetBits(GPIOB,arr[i]);
			delay(720000);
			GPIO_ResetBits(GPIOB,arr[i]);
			delay(360000);
			if(i==1){
				GPIO_SetBits(GPIOB,arr[i]);
				delay(360000);
				GPIO_ResetBits(GPIOB,arr[i]);
				delay(360000);
			}
		}
	}
	
}


