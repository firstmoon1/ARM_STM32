
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC

void gpio_config(){
	
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//GPIO
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; // pin0=motor, pin1=led,pin2=led
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&init);
	
	//ADC
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_1; // ADC1_IN0  ,PA0 için
	GPIO_Init(GPIOA,&init);
	
	//TIMER
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_0; // PA0 timer, channel_1 dir 
	GPIO_Init(GPIOA,&init);

	//BUTTON
	init.GPIO_Mode=GPIO_Mode_IPD;
	init.GPIO_Pin=GPIO_Pin_3; //PB3 buton
	GPIO_Init(GPIOB,&init);
	
}

void timer_config(){
	TIM_TimeBaseInitTypeDef init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	init.TIM_ClockDivision=TIM_CKD_DIV1; // or just write 1
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=2399;
	init.TIM_Prescaler=10; // 1/4 sn de bir tim pulse vericek ,en yüksek 2399 degeri verir pwm de
	init.TIM_RepetitionCounter=DISABLE;
	
	TIM_TimeBaseInit(TIM2,&init);
	TIM_Cmd(TIM2,ENABLE);
}

void adc_config(){
	
	ADC_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	init.ADC_ContinuousConvMode=ENABLE;
	init.ADC_DataAlign=ADC_DataAlign_Right;
	init.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	init.ADC_Mode=ADC_Mode_Independent;
	init.ADC_NbrOfChannel=1;
	init.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&init);
	ADC_Cmd(ADC1,ENABLE);
}

void delay(uint16_t tt){
	while(tt--);
}

float map(uint16_t adc_value,float max,float min,float comax, float comin){
	return adc_value*((comax-comin)/(max-min));
}

// adc function
uint16_t adc_pwm_function(){
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,0,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetITStatus(ADC1,ADC_IT_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
	
}

// pwm function inside timer
void timer_function(uint32_t value){
	
	TIM_OCInitTypeDef init;
	
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable;
	init.TIM_Pulse=value;
	
	TIM_OC1Init(TIM2,&init);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
}

int main(){
	
	gpio_config();
	timer_config();
	adc_config();
	int btn_condition=0,flag=0,adc_value=0,map_value=0;
	
	while(1){
		
		
		/*
		// motor calisiyor mu denemesi  
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		delay(3600000);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		*/
		
		
		/*
		// buton ile kontrol ,yalniz motor butona basili haldeyken calisir, 
		// 2v ile 12v arasi calisma gerilimi biz 3.3v cikis var oda high yani digital cikis 1 demek
		// digital cikis 0 yani low ise 0v dur. 
		btn_condition=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3); // butona bastin 1 ,elini cektin 0 ,switch degil yani 
		if(btn_condition==1){
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 );
			GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		}else{
			GPIO_SetBits(GPIOB,GPIO_Pin_2 );
			GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 );
		}
		*/
		
		/*
		// butona 1 kez bastiginda calisir until bir kez daha basarsan ,kalici buton
		btn_condition=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
		if(btn_condition==1){
			if(flag==1){
				flag=0;
				GPIO_SetBits(GPIOB,GPIO_Pin_2 );
				GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 );
			}else{
				flag=1;
				GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 );
				GPIO_ResetBits(GPIOB,GPIO_Pin_2);
			}
		}
		*/
		
		/*
		// pwm ile kontrol ,stop dan son hiza cikarma, motor hiz kontrolu
		// son hiz period daki sayidir yukarda timerda
			for(int i=0;i<2399;i++){
				adc_pwm_function(i);
				delay(360000);
			}
		*/
		
		/*
		// potansiyometre ile hiz kontrolu, adc ve timer pwm kullanildi
		// sikinti cikmasin diye 4095 yerine 4030 versek daha tatli olur hani sinirlari zorluyoruz gibi bir sey 4095 12 bit ya adc de almayabilir potansiyometreden
		adc_value=adc_pwm_function();
		map_value=map(adc_value,4030,0,2399,0);
		timer_function(map_value);
		*/
		
		
		
		
		
		
		
	}
}











