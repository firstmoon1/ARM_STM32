#include "stm32f4xx.h"

uint32_t cnt=0;

void gpio_config(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOD,&init);


}

void timer_config(){

	TIM_TimeBaseInitTypeDef init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	// period sayma islemi period a verdigin sayýya kadar gider,max deger 32 bit,
	// neden tim2 ve tim5 32bit,tim3,tim4,tim9,tim10,11,12,13,14 ise 16 bit genel amaclý timer,
	init.TIM_ClockDivision=TIM_CKD_DIV1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=3999;
	init.TIM_Prescaler=41999;
	init.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM3,&init);
	TIM_Cmd(TIM3,ENABLE);
}

void nvic_conf(){

	NVIC_InitTypeDef init;

	init.NVIC_IRQChannel=TIM3_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=0;
	init.NVIC_IRQChannelSubPriority=0;

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); // timer ile nvic'i iliþkilendirdik
	NVIC_Init(&init);

}

void TIM3_IRQHandler(){
	GPIO_ToggleBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); // flag'ý default moda getiriyoruz ,bunu yapmaz isek flag hep 1 kalýr ve tekrar interrupt'a girmez
}

int main(void){

	gpio_config();
	timer_config();
	nvic_conf();
  while (1){

	  cnt=TIM_GetCounter(TIM3);

  }


}

/*


*/



















