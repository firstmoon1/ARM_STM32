
#include "stm32f4xx.h"

GPIO_InitTypeDef init0;
ADC_InitTypeDef init1;
TIM_TimeBaseInitTypeDef init2;
TIM_OCInitTypeDef init3;
ADC_CommonInitTypeDef init4;

uint16_t adc_value=0,map_value=0;



void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//adc PA0
	init0.GPIO_Mode=GPIO_Mode_AN;
	init0.GPIO_OType=GPIO_OType_PP;
	init0.GPIO_Pin=GPIO_Pin_0;
	init0.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init0.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init0);

	//timer PD12,PD13,PD14,PD15
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //alternate function olarak ayarladýk timer pwm için.
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);

	init0.GPIO_Mode=GPIO_Mode_AF;
	init0.GPIO_OType=GPIO_OType_PP;
	init0.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	init0.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init0.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init0);

}
void adc_conf(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	init4.ADC_Mode=ADC_Mode_Independent;
	init4.ADC_Prescaler=ADC_Prescaler_Div4;// max 36Mhz de calýsýr ama clock hattý 84Mhz ,84/4 den 21 MHz olarak kullanýyoruz
	ADC_CommonInit(&init4);

	init1.ADC_ContinuousConvMode=ENABLE;
	init1.ADC_DataAlign=ADC_DataAlign_Right;
	init1.ADC_NbrOfConversion=1;
	init1.ADC_Resolution=ADC_Resolution_12b; // 12 bit 4096 max deger potansiyometreden
	init1.ADC_ScanConvMode=DISABLE;

	ADC_Init(ADC1,&init1);
	ADC_Cmd(ADC1,ENABLE);

}
void timer_conf(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	init2.TIM_ClockDivision=TIM_CKD_DIV1;//apb1 clock hattý 42Mhz, apb1 timer ise 84 Mhzdir, bölmedik oldugu gibi 84MHz olarak kullandýk
	init2.TIM_CounterMode=TIM_CounterMode_Up;
	init2.TIM_Period= 99;
	init2.TIM_Prescaler= 83;
	init2.TIM_RepetitionCounter=0;

	init3.TIM_OCPolarity=TIM_OCPolarity_High;
	init3.TIM_OCMode=TIM_OCMode_PWM1;
	init3.TIM_OutputState=TIM_OutputState_Enable;

	TIM_TimeBaseInit(TIM4,&init2);
	TIM_Cmd(TIM4,ENABLE);
}

uint16_t adc_function(){

	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while( (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) )==RESET);
	return ADC_GetConversionValue(ADC1);

}

float map(float value,float cmax,float cmin,float max,float min ){
	return value*((cmax-cmin)/(max-min));
}


int main(void){

	gpio_conf();
	adc_conf();
	timer_conf();


  while (1){

	  adc_value=adc_function();
	  map_value=map(adc_value,99,0,4096,0); // potansiyometreden sanýrým max 3860, min 164 görüyorum.


	  init3.TIM_Pulse=map_value;
	  TIM_OC1Init(TIM4,&init3);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);

	  init3.TIM_Pulse=map_value;
	  TIM_OC2Init(TIM4,&init3);
	  TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);

	  init3.TIM_Pulse=map_value;
	  TIM_OC3Init(TIM4,&init3);
	  TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);

	  init3.TIM_Pulse=map_value;
	  TIM_OC4Init(TIM4,&init3);
	  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);


  }

}


/*
 formuller:

 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = timer_tick_frequency/(period+1)
 timer_tick_frequency = timer_clk/(prescaler+1)
 pulse = [(period+1)*(duty cycle)]-1


 timer clock==Timer_CLK=84MHz == 84*10^6

mesela elimizde bir motor var ve 100KHz ile calýþmasý gerek,o zaman pwm frequency 100KHz olmalý

 pwm_frequency =10000 olmasý için
 timer_tick_frequency=(84*10^6)/(prescaler+1) , prescaler 83 olsun,ve timer_tick_frequency = 1*10^6 olur
 period=(1000000/10000)-1=99 ,yani period : 99 , bizim max duty cycle sýnýrýmýz,%100 ulaþýlabilecek yer 99


pwm_frequency = 20000 olmasý için ,böyle daha hýzlý oldu, max ckýp min e gidiyor
timer_tick_frequency =84*10^6 / (20+1)   ,prescaler = 20
timer_tick_frequency = 4*10^6
period = (4000000/20000)-1  = 200-1  = period  : 199



PD12 için %100 duty cycle ,pulse :9999 = [(9999+1)*(100/100)]-1
PD13 için %75 duty cycle ,pulse  :7499 = [(9999+1)*(75/100)]-1
PD14 için %50 duty cycle ,pulse  :4999 = [(9999+1)*(50/100)]-1
PD15 için %25 duty cycle ,pulse  :2499 = [(9999+1)*(25/100)]-1




 */
