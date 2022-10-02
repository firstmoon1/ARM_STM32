
#include "stm32f4xx.h"

GPIO_InitTypeDef init; // global variable olarak ald�k farkl�l�k olsun diye.
TIM_TimeBaseInitTypeDef init1;
TIM_OCInitTypeDef init2;

uint32_t delay_counter=0;

// gercikci bir delay function� kullan�yoruz
void SysTick_Handler(){

	if(delay_counter>0){
		delay_counter--;
	}
}
void delay_ms(uint32_t time){
	delay_counter=time;
	while(delay_counter);
}


void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //alternate function olarak ayarlad�k timer pwm i�in.
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

	init1.TIM_ClockDivision=TIM_CKD_DIV2; // apb1 clock hatt� 84 mhz, burda div1 dersen 84mhz olarak kullan�rs�n ,div2 dersen 42mhz ,b�lme yap�yor yani
	init1.TIM_CounterMode=TIM_CounterMode_Up;
	init1.TIM_Period=199;
	init1.TIM_Prescaler=20;
	init1.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM4,&init1);
	TIM_Cmd(TIM4,ENABLE);


	init2.TIM_OCMode=TIM_OCMode_PWM1; // mode belirledik
	init2.TIM_OutputState=ENABLE; // c�k�s almak istiyorsak enable
	init2.TIM_OCPolarity=TIM_OCPolarity_High;

	SysTick_Config(SystemCoreClock/1000); // gercekci delay olsun diye.

}


int main(void){

	tim_conf();
	gpio_conf();

  while (1){


	  for(int i=0;i<=100;i++){


		  init2.TIM_Pulse = i;
		  TIM_OC1Init(TIM4,&init2);
		  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 1
		  delay_ms(5); // 5ms gecikme gercekci ama bu delay

		  init2.TIM_Pulse = i;
		  TIM_OC2Init(TIM4,&init2);
		  TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 2
		  delay_ms(5);

		  init2.TIM_Pulse = i;
		  TIM_OC3Init(TIM4,&init2);
		  TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 3
		  delay_ms(5);

		  init2.TIM_Pulse = i;
		  TIM_OC4Init(TIM4,&init2);
		  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 4
		  delay_ms(5);

	  }

	  for(int i=100;i>=0;i--){

		  init2.TIM_Pulse = i;
		  TIM_OC1Init(TIM4,&init2);
		  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 1
		  delay_ms(5); // 5ms gecikme gercekci ama bu delay

		  init2.TIM_Pulse = i;
		  TIM_OC2Init(TIM4,&init2);
		  TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 2
		  delay_ms(5);

		  init2.TIM_Pulse = i;
		  TIM_OC3Init(TIM4,&init2);
		  TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 3
		  delay_ms(5);

		  init2.TIM_Pulse = i;
		  TIM_OC4Init(TIM4,&init2);
		  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); //for channel 4
		  delay_ms(5);

	  }
	  // tim4 �n channels 1,2,3,4  lerini kullanarak farkl� pwm duty cyclar�n� g�rd�k



  }

}





/*
 formuller:

 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = timer_tick_frequency/(period+1)
 timer_tick_frequency = timer_clk/(prescaler+1)
 pulse = [(period+1)*(duty cycle)]-1


 timer clock==Timer_CLK=84MHz == 84*10^6

mesela elimizde bir motor var ve 100KHz ile cal��mas� gerek,o zaman pwm frequency 100KHz olmal�

 pwm_frequency =10000 olmas� i�in
 timer_tick_frequency=840000000/(prescaler+1) , prescaler 83 olsun
 timer_tick_frequency = 1000000 olur
 period=(1000000/10000)-1=99 ,yani period : 99 , bizim max duty cycle s�n�r�m�z,%100 ula��labilecek yer 99


pwm_frequency = 20000 olmas� i�in ,b�yle daha h�zl� oldu, max ck�p min e gidiyor
timer_tick_frequency =84*10^6 / (20+1)   ,prescaler = 20
timer_tick_frequency = 4*10^6
period = (4000000/20000)-1  = 200-1  = period  : 199


// bir �nceki ornekten kalma degi�tirmedik
PD12 i�in %100 duty cycle ,pulse :9999 = [(9999+1)*(100/100)]-1
PD13 i�in %75 duty cycle ,pulse  :7499 = [(9999+1)*(75/100)]-1
PD14 i�in %50 duty cycle ,pulse  :4999 = [(9999+1)*(50/100)]-1
PD15 i�in %25 duty cycle ,pulse  :2499 = [(9999+1)*(25/100)]-1





init2.TIM_OCMode=TIM_OCMode_PWM1;  PWM1 ile PWM2 nin fark� ne
hat�rlarsak ilk derslerde bu kursun mod1 ve mod2 var demi�tik,
mod1 de %25 duty cycle verdigimizde dersek  %25 high high olur ,%75 de low olur.
mod2 de ise ,eger duty cycle da �25 verirsek  system de mod2 secili iken �25 de low olur
%75 de high olur ,high low derken led yanmas� s�nmesi gibi d���n.

burda pwm1 kulland�k yani %25 duty cycle verdigimizde , %25 likte high olsun
 



AF li  bir �ey g�rd�g�nde mesela RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
bilki bunun line config'i vard�r mutlaka. burda stm32f100rb kart� i�in.,
embed_stm32f100rb > 010_Interrupt dosyas�nda g�rd�m.
stm32f100rb kart�nda  hat�rlad�g�m kadar�yla bir tek exti_interrupt larda g�rd�m
 
stm32f407vg kart� i�in ise biz AF yani alternative function'� hat�rlad�g�m
kadar�yla timer pwm lerde kulland�k 

*/






