
#include "stm32f4xx.h"

GPIO_InitTypeDef init; // global variable olarak aldýk farklýlýk olsun diye.
TIM_TimeBaseInitTypeDef init1;
TIM_OCInitTypeDef init2;


void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //alternate function olarak ayarladýk timer pwm için.
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);

	init.GPIO_Mode=GPIO_Mode_AF;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init);

}

void tim_conf(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	init1.TIM_ClockDivision=TIM_CKD_DIV2; // apb1 clock hattý 84 mhz, burda div1 dersen 84mhz olarak kullanýrsýn ,div2 dersen 42mhz ,bölme yapýyor yani
	init1.TIM_CounterMode=TIM_CounterMode_Up;
	init1.TIM_Period=9999;
	init1.TIM_Prescaler=8399;
	init1.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM4,&init1);
	TIM_Cmd(TIM4,ENABLE);

	init2.TIM_OCMode=TIM_OCMode_PWM1; // mode belirledik
	init2.TIM_OutputState=ENABLE; // cýkýs almak istiyorsak enable
	init2.TIM_OCPolarity=TIM_OCPolarity_High;

}

int main(void){

	gpio_conf();
	tim_conf();

  while (1){

	  //PD12 %100 duty cycle ile
	  init2.TIM_Pulse=9999;
	  TIM_OC1Init(TIM4,&init2);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); // tim4->ch1 enable

	  //PD13 %75 duty cycle ile
	  init2.TIM_Pulse=7499;
	  TIM_OC2Init(TIM4,&init2);
	  TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); // tim4->ch1 enable

	  //PD14 %50 duty cycle ile
	  init2.TIM_Pulse=4999;
	  TIM_OC3Init(TIM4,&init2);
	  TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); // tim4->ch1 enable

	  //PD15 %25 duty cycle ile mesela
	  init2.TIM_Pulse=2499;
	  TIM_OC4Init(TIM4,&init2);
	  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); // tim4->ch1 enable

  }


}



/*
 formuller:

 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = timer_tick_frequency/(period+1)
 timer_tick_frequency = timer_clk/(prescaler+1)
 pulse = [(period+1)*(duty cycle)]-1

misal mesela elimizde bir motor var ve 100KHz ile calýþmasý gerek,o zaman pwm frequency 100KHz olmalý

 timer_tick_frequency = timer_clk/(prescaler+1)
 10000 = 84000000/8400
 timer_tick_frequency = 10000
 timer clock = 84MHz == 84*10^6
 prescaler :8399 olsun +1 yapýnca 8400 olur.

 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = (timer_tick_frequency)/(period+1)
 1 = 10000 / (9999+1)
 period :9999 olsun +1 yapýnca 10000
 pwm_frequency = 1Hz



PD12 için %100 duty cycle ,pulse :9999 = [(9999+1)*(100/100)]-1
PD13 için %75 duty cycle ,pulse  :7499 = [(9999+1)*(75/100)]-1
PD14 için %50 duty cycle ,pulse  :4999 = [(9999+1)*(50/100)]-1
PD15 için %25 duty cycle ,pulse  :2499 = [(9999+1)*(25/100)]-1

 */
























