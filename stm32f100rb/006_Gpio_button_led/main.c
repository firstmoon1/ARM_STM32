#include "stm32f10x.h"              

uint16_t btn=0,cnt=0;
void config(){

GPIO_InitTypeDef com;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	
com.GPIO_Mode=GPIO_Mode_Out_PP;
com.GPIO_Pin=GPIO_Pin_0;
com.GPIO_Speed=GPIO_Speed_50MHz;
	
GPIO_Init(GPIOB,&com);

com.GPIO_Mode=GPIO_Mode_IPD;
com.GPIO_Pin=GPIO_Pin_6;
	
GPIO_Init(GPIOC,&com);
}

void delay(uint32_t tt){
	while(tt--){ }
}

int main(){
	config();
	
	while(1){
		btn=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
		if(btn==1){
			cnt++;
			delay(360000);
			if( cnt%3 == 1 ){
				GPIO_SetBits(GPIOB,GPIO_Pin_0);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1 | GPIO_Pin_2);
			}else if( cnt%3 == 2 ){
				GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
				GPIO_ResetBits(GPIOB, GPIO_Pin_2);
			}else if( cnt%3 == 0 ){
				GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
			}else{
				GPIO_WriteBit(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 , Bit_RESET);
			}
			
		}	
		
	}
}

