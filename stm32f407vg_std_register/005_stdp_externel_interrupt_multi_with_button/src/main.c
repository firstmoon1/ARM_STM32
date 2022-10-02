
#include "stm32f4xx.h"

void delay(uint32_t tt){
	while(tt--);
}
int i=0,i1=0;

void gpio_conf(){
	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //PD12,13,14,15 led,  PD2  buton olucak ,istersen PD1 ,PD2 yap buton olarak exti de
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //  PA2  buton olucak

	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init);

	init.GPIO_Mode=GPIO_Mode_IN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_1; //  pin1 ye ekle exti için eger d portunda yapacaksan
	init.GPIO_PuPd=GPIO_PuPd_DOWN;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init);

	init.GPIO_Mode=GPIO_Mode_IN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin= GPIO_Pin_2;
	init.GPIO_PuPd=GPIO_PuPd_DOWN;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);

}

void exti_conf(){
	EXTI_InitTypeDef init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	//Selects the GPIO pin used as EXTI Line.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource2);
	/*
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource1); //Selects the GPIO pin used as EXTI Line.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource2); // bunuda yapabilirsin sýnýrlama yok yeterki PA0 ,PB0 gibi yapma sayýlar farklý olsun portlar farketmez
	*/
	init.EXTI_Line=EXTI_Line1 | EXTI_Line2;
	init.EXTI_LineCmd=ENABLE;
	init.EXTI_Mode=EXTI_Mode_Interrupt;
	init.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&init);

}

void nvic_conf(){
	NVIC_InitTypeDef init;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	init.NVIC_IRQChannel=EXTI1_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=0;
	init.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&init);

	init.NVIC_IRQChannel=EXTI2_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=1;
	init.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&init);
}

void EXTI1_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET){
		i++;
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		delay(1680000);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		delay(1680000);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay(1680000);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay(1680000);

		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI2_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
		i1++;
		GPIO_ResetBits(GPIOD, GPIO_Pin_All); // yukardaki ile ayný aslýnda :  GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay(1680000);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay(1680000);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		delay(1680000);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		delay(1680000);

		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

int main(void){

	gpio_conf();
	exti_conf();
	nvic_conf();


  while (1){

	  GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);



  }
}

