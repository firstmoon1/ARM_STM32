#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI ,interrupt için
#include "stm32f10x.h"                  // Device header  genel kütüphane c programdaki stdio.h gibi
               
uint16_t cnt=0;
void gpioConfig(){

	GPIO_InitTypeDef GPIOInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	// for led
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); // for button
	// led config
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP; // pull push
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	// buton yeri
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD; // input pull down
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	// no speed for button
	GPIO_Init(GPIOC,&GPIOInitStructure);
	
}

// interrupt config
void interruptconfig(){
	 
	EXTI_InitTypeDef extistructure;
	NVIC_InitTypeDef nvicstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);// 2 parametre alir, port source,pin source
	
	extistructure.EXTI_Line=EXTI_Line6; 
	extistructure.EXTI_LineCmd=ENABLE;
	extistructure.EXTI_Mode=EXTI_Mode_Interrupt;
	extistructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&extistructure);
	
	nvicstructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	nvicstructure.NVIC_IRQChannelCmd=ENABLE;
	nvicstructure.NVIC_IRQChannelPreemptionPriority=1;
	nvicstructure.NVIC_IRQChannelSubPriority=1; 
	NVIC_Init(&nvicstructure);
	
}

void EXTI9_5_IRQHandler(){
	
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){
		cnt++; // butona basinca cnt  artsin simulasyondan takip ederiz
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
	
}




int main(){	
	gpioConfig();	
	interruptconfig();
	int arr[3]={GPIO_Pin_0 , GPIO_Pin_1 , GPIO_Pin_2},i=0;
	while(1){
		for(int i=0;i<3;i++){
			GPIO_SetBits(GPIOB,arr[i]);
			delay(3600000);
			GPIO_ResetBits(GPIOB,arr[i]);
			delay(3600000);
		}
		for(int i=1;i<2;i++){
			GPIO_SetBits(GPIOB,arr[i]);
			delay(3600000);
			GPIO_ResetBits(GPIOB,arr[i]);
			delay(3600000);
		}
	}	
}
/*

AF li  bir þey gördügünde mesela RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
bilki bunun line config'i vardýr mutlaka. burda stm32f100rb kartý için.
stm32f100rb kartýnda  hatýrladýgým kadarýyla bir tek exti_interrupt larda gördüm
 
stm32f407vg kartý için ise biz AF yani alternative function'ý hatýrladýgým
kadarýyla timer pwm lerde kullandýk 

*/

















