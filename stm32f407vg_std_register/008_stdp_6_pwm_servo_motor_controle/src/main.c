

#include "stm32f4xx.h"


GPIO_InitTypeDef init0;
TIM_TimeBaseInitTypeDef init2;
TIM_OCInitTypeDef init3;



void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	//timer PD12
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //alternate function olarak ayarladýk timer pwm için.


	init0.GPIO_Mode=GPIO_Mode_AF;
	init0.GPIO_OType=GPIO_OType_PP;
	init0.GPIO_Pin=GPIO_Pin_12;
	init0.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init0.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init0);

}

void timer_conf(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	init2.TIM_ClockDivision=TIM_CKD_DIV1;//apb1 clock hattý 42Mhz, apb1 timer ise 84 Mhzdir, bölmedik oldugu gibi 84MHz olarak kullandýk
	init2.TIM_CounterMode=TIM_CounterMode_Up;
	init2.TIM_Period= 19999;
	init2.TIM_Prescaler= 83;
	init2.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM4,&init2);
	TIM_Cmd(TIM4,ENABLE);

	init3.TIM_OCMode=TIM_OCMode_PWM1;
	init3.TIM_OutputState=ENABLE;
	init3.TIM_OCPolarity=TIM_OCPolarity_High;

}

void delay(uint32_t tt){
	while(tt--);
}

int main(){

	gpio_conf();
	timer_conf();

  while (1){



	  init3.TIM_Pulse=500; // pd12
	  TIM_OC1Init(TIM4,&init3);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	  delay(21000000); // yaklasýk 1sn 'ye denk gelir

	  init3.TIM_Pulse=1500;
	  TIM_OC1Init(TIM4,&init3);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	  delay(21000000);

	  init3.TIM_Pulse=2500;
	  TIM_OC1Init(TIM4,&init3);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	  delay(21000000);



  }

}



/*
 formuller:

 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = timer_tick_frequency/(period+1)
 timer_tick_frequency = timer_clk/(prescaler+1)
 pulse = [(period+1)*(duty cycle)]-1
 timer_tick_frequency  = (period+1) * pwm_frequency
 prescaler = (timer_clk/timer_tick_frequency)-1

 timer clock==Timer_CLK=84MHz == 84*10^6

 mesela elimizde bir motor var ve 100KHz ile calýþmasý gerek,o zaman pwm frequency 100KHz olmalý


 pwm period 20ms(50Hz) = , 20 ms == 20000us
 servo motorlarda period =20msdir,20ms de bir tekrar eder her þey. 20ms=20000 micosaniye yapar

 timer_tick_frequency  = (period+1) * pwm_frequency
 1MHz = (20000) * 50

 prescaler = ((timer_clk/timer_tick_frequency))-1
 83 = ((84*10^6)/(1*10^6))-1

 pwm period 20ms(50Hz) = , 20 ms == 20000us

 timer_tick_frequency = 1MHz
 prescaler = 83
 period = 19999, +1 var ya o yüzden,normalde 20000 us bulduk


 ---- duty cycle ----- period 20ms (50Hz) ------
 90 degree = pulse width 0.5ms
 0 degree = pulse width 1.5ms
 -90 degree = pulse width 2.5ms

1 period 20 ms geciyorsa formul
duty cycle = (istenen pulse width / all pulse width)
(1/40) = 0.5 / 20

bizim max pwm period degerimiz servo motor da 20ms == 20000us (microsaniye) dir.peki ben 90 degree konumunu bulmak istiyorsak
20000us nin 1/40'ý ile iþ yapmalýyým yani carpmalýyýz

90 degree için
duty cycle = (istenen pulse width / all pulse width)
(1/40) = 0.5 / 20
pwm_period * duty cycle = pulse
20000 * (1/40) = 500
yani bagladýgýmýz servo motora 500 lük pulse degeri verirsek servo motor 90 degre konumuna gelir.

0 degree için
duty cycle = (istenen pulse width / all pulse width)
(0.075) = 1.5 / 20
pwm_period * duty cycle = pulse
20000 * (0.075) = 1500

-90 degree için
duty cycle = (istenen pulse width / all pulse width)
(0.125) = 1.5 / 20
pwm_period * duty cycle = pulse
20000 * (0.125) = 2500
 */

