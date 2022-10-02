#include "stm32f4xx.h"

GPIO_InitTypeDef init;
ADC_InitTypeDef init1;
ADC_CommonInitTypeDef init2;
TIM_TimeBaseInitTypeDef init3;
TIM_OCInitTypeDef init4;

uint32_t adc_value=0,cnt_systic=0,map_value=0;

void SysTick_Handler(void)
{
	if(cnt_systic>0)
		cnt_systic--;
}

void delay(uint32_t tt){
	cnt_systic=tt;
	while(tt--);
}


void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//adc
	init.GPIO_Mode=GPIO_Mode_AN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_3; // adc PA3 // ADC123_IN3
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);


	//tim
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
	init.GPIO_Mode=GPIO_Mode_AF;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_2; // timer PA2, TIM2_CH3
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);


}

void adc_conf(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	init2.ADC_Mode=ADC_Mode_Independent;
	init2.ADC_Prescaler=ADC_Prescaler_Div4;// adc max 36MHz de calýsýr ama clock hattý(APB2 hattý) 84MHz,bu sebeple 4 e böldük 21MHz ile calýssýn
	ADC_CommonInit(&init2);

	init1.ADC_ContinuousConvMode=ENABLE;
	init1.ADC_DataAlign=ADC_DataAlign_Right;
	init1.ADC_NbrOfConversion=1;
	init1.ADC_Resolution=ADC_Resolution_12b;
	init1.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&init1);
	ADC_Cmd(ADC1,ENABLE);



}

void tim_conf(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); // 84 MHz clock line

	init4.TIM_OCMode=TIM_OCMode_PWM1;
	init4.TIM_OCPolarity=TIM_OCPolarity_High;
	init4.TIM_OutputState=TIM_OutputState_Enable;

	init3.TIM_ClockDivision=TIM_CKD_DIV1; //apb1 hattý 42MHz,apb1 timer 84MHz, DIV1 diyerek timer'ý 84MHz olarak kullanýyoruz,DIV2 deseydin timer'ý 42 MHz olarak kullanacaktýk.
	init3.TIM_CounterMode=TIM_CounterMode_Up;
	init3.TIM_Period=8399; // arifde 8399 1KHz için, kösede 99 10KHz için, kösede 199 20KHz için
	init3.TIM_Prescaler=10; // arifde 10 , kösede 83 , kösede 20
	init3.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM2,&init3);
	TIM_Cmd(TIM2,ENABLE);

	SysTick_Config(SystemCoreClock/1000); // milisaniye cinsinden gercekci delay olsun diye.


}

void timer_function(uint32_t value){  // tim2 channel3

	init4.TIM_Pulse=value;
	TIM_OC3Init(TIM2,&init4);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);


}


uint32_t adc_function(){

	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while((ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))==RESET );
	return ADC_GetConversionValue(ADC1);

}


float map(float value,float cmax,float cmin,float max,float min){
	return value*((cmax-cmin)/(max-min));
}

int main(){

	gpio_conf();
	adc_conf();
	tim_conf();

	while(1){

		// bc237 transistor ile motor calýstýr felan yaptým
		adc_value=adc_function();
		//map_value=map(adc_value,8399,0,4095,0);
		map_value=map(adc_value,8399,0,4000,0); // noise var diye yaptým yoksa 4095 max.
		timer_function(map_value);




	}


/*
arif mandal'ýn hesabýna göre yaptým
clock line 84MHz ,
frekans hesaplama  1KHz için :
pwmfrekans=(Timerclock)/(timerperiod+1) , 1kHz=1000Hz
1kHz=(8.4*(prescaler=10)/(timerperiod+1) ,prescaler'i isleme dahil etmeyiz ama 84Mhz den calar ,yukarda period ile prescaler ters yazmýsýz .
1kHz=(8.4*10^6)/(8399+1)
biz timerclock'u ve prescaler'i belirledik ,
clock'u 8.4 olarak ayarlamisiz ,biliyon port yolunda 42MHz ama apb1 tim hattý 84Mhz yaziyor ,data sheet port pin diagraminda

init.TIM_Period=8399;
init.TIM_Prescaler=10;

tim pulse hesaplama:
TIMpulse=(((Timerperiod+1)*(DutyCycle))/100)-1
timpulse(1%)=((8400*1)/100)-1 =83
timpulse(25%)=((8400*25)/100)-1 =2099
timpulse(50%)=((8400*50)/100)-1 =4199  , %50 cycle
timpulse(75%)=((8400*75)/100)-1 =6299
timpulse(100%)=((8400*100)/100)-1 =8399





muhammed fatih köse formulleri
 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = timer_tick_frequency/(period+1)
 timer_tick_frequency = timer_clk/(prescaler+1)
 pulse = [(period+1)*(duty cycle)]-1


 timer clock==Timer_CLK=84MHz == 84*10^6

mesela elimizde bir motor var ve 100KHz ile calýþmasý gerek,o zaman pwm frequency 100KHz olmalý

 pwm_frequency =10000 olmasý için,sanýrým 10KHz
 timer_tick_frequency=840000000/(prescaler+1) , prescaler 83 olsun
 timer_tick_frequency = 1000000 olur
 period=(1000000/10000)-1=99 ,yani period : 99 , bizim max duty cycle sýnýrýmýz,%100 ulaþýlabilecek yer 99


pwm_frequency = 20000 olmasý için ,böyle daha hýzlý oldu, max ckýp min e gidiyor, 20KHz sanýrým
timer_tick_frequency =84*10^6 / (20+1)   ,prescaler = 20
timer_tick_frequency = 4*10^6
period = (4000000/20000)-1  = 200-1  = period  : 199


// bir önceki ornekten kalma degiþtirmedik
PD12 için %100 duty cycle ,pulse :9999 = [(9999+1)*(100/100)]-1
PD13 için %75 duty cycle ,pulse  :7499 = [(9999+1)*(75/100)]-1
PD14 için %50 duty cycle ,pulse  :4999 = [(9999+1)*(50/100)]-1
PD15 için %25 duty cycle ,pulse  :2499 = [(9999+1)*(25/100)]-1



*/
}
