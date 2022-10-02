#include "stm32f10x.h"                  // Device header

void gpioconfig_canan(){
	
	GPIO_InitTypeDef gpioebru;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	gpioebru.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioebru.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; // 3 tane pin aldik | bu iaret veya bit olarak anlamina geliyor
	gpioebru.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&gpioebru);
	
}

void delay(uint32_t omur){
	while(omur--){ }
}


int main(){
	gpioconfig_canan();
	int i=0,arr[3]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2}; // biz array bu sekilde atayabiliriz ve burda tanimlayalim while içinde gereksiz
	// enerji tuketir
	while(1){
		/*
		// uzun hali bunu for döngüsü ilede yapilabilir
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
		delay(3600000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
		delay(3600000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
		delay(3600000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
		delay(3600000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_SET);
		delay(3600000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_RESET);
		delay(3600000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
		delay(3600000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
		delay(3600000);
		*/
		
		
		for( i=0;i<3;i++){
			GPIO_SetBits(GPIOB,arr[i]);
			delay(360000);
			GPIO_ResetBits(GPIOB,arr[i]);
			delay(360000);
		}
		for(int j=1;j>0;j--){
			GPIO_SetBits(GPIOB,arr[j]);
			delay(360000);
			GPIO_ResetBits(GPIOB,arr[j]);
			delay(360000);
		}
		
		
	}
}










