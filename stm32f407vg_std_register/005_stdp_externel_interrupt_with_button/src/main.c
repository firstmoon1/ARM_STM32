
#include "stm32f4xx.h"

void gpio_config(){
	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); // led  PD12,13,14,15
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); // button PA0 exti
	// led config
	init.GPIO_Mode= GPIO_Mode_OUT; // general purpose output
	init.GPIO_OType=GPIO_OType_PP; // push pull
	init.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL; // no pull push
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init);

	// button config
	init.GPIO_Mode= GPIO_Mode_IN; // general purpose input
	init.GPIO_OType=GPIO_OType_PP; // push pull
	init.GPIO_Pin=GPIO_Pin_0;
	init.GPIO_PuPd=GPIO_PuPd_DOWN;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);

}

void exti_config(){
	EXTI_InitTypeDef init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); // SYSCFG burda exti,interrupt için clock name,

	init.EXTI_Line=EXTI_Line0;
	init.EXTI_LineCmd=ENABLE;
	init.EXTI_Mode=EXTI_Mode_Interrupt;
	init.EXTI_Trigger=EXTI_Trigger_Rising;

	EXTI_Init(&init);

}

void nvic_config(){

	NVIC_InitTypeDef init;

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

	init.NVIC_IRQChannel=EXTI0_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=0;  // 0 to 15 arasý degerler alabilir ,sýnýrlar dahil
	init.NVIC_IRQChannelSubPriority=0;

	NVIC_Init(&init);
}

void delay(uint32_t tt){
	while(tt--);
}

void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET ){
		GPIO_ResetBits(GPIOD,GPIO_Pin_12  );
		delay(1680000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13  );
		delay(1680000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14  );
	    delay(1680000);
	    GPIO_ResetBits(GPIOD, GPIO_Pin_15 );
		delay(1680000);

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	//EXTI_ClearITPendingBit(EXTI_Line0); // þu an için fark etmez ,if içinde yazarsan tasarruf edersin sürekli calýþmaz bu komut
}

int main(void){

	gpio_config();
	exti_config();
	nvic_config();

  while (1){

	  GPIO_SetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
	  delay(1680000);


  }
}
