
// uart&usart denemeleri

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stdio.h"
#include "delay.h"

static char test[25]="ARIF MANDAL\r\n";
static uint16_t data=0;

void gpio_config(){
	
	// PA9->USART1_Tx  ,PA10->USART1_Rx
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// Tx
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_9; // PA9 Tx için
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	// Rx
	init.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	init.GPIO_Pin=GPIO_Pin_10; // PA10 Rx için
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	// led
	
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0; // PA0 led için
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	// adc
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_1;  // PA1 adc için
	GPIO_Init(GPIOA,&init);
	
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

uint16_t adc_function(){
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,0,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetITStatus(ADC1,ADC_IT_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
}


void uart_uarts_config(){
	
	USART_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	// Tx config == bu eleman datalari gönderiyor ,Rx ise datalari aliyor ,file konusundan fwrite burda Rx dir,fprintf ise Tx dir. termit ile kullanilir, veri gönderilen alinan yer için
	init.USART_BaudRate=9600;
	init.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // voltaj controlu yapmayacagiz, word dosyasinda 4 üncü sayfa
	init.USART_Mode=USART_Mode_Tx | USART_Mode_Rx ;
	init.USART_Parity=USART_Parity_No; //  word dosyasinda 5 inci sayfada
	init.USART_StopBits=USART_StopBits_1; // 0.5,1,1.5,2 secenekleri var, word dosyasinda yaziyor 1 yada 2 degeri alir 
	init.USART_WordLength=USART_WordLength_8b; // 8 veya 9 bit degerleri alir ,b nin olayi bit demek
	
	USART_Init(USART1,&init);
	USART_Cmd(USART1,ENABLE);
}

void uart_usart_function(char *value_string){
	while(*value_string){
		
		while(!(USART1->SR & 0x00000040));
		USART_SendData(USART1,*value_string);
		*value_string++;
		// *value_string++ bunu neden value_string++ böyle yapmadi anlamadim basina * koyunca bir sey degismiyor 
	}
	
}


int main(){
	
	gpio_config();
	uart_uarts_config();
	adc_config();
	
	while(1){
		
		//uart_usart_function(test);
		/*
		data=USART_ReceiveData(USART1);
		if(data==1){
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		}else if(data==0){
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		}
		*/
		data=adc_function();
		sprintf(test,"%d\n",data);
		uart_usart_function(test);
		delay_ms(500); // systic timer kütüphanesi eklemen gerek
		
		

	}
}

















































/*  // genel deneme2
#include "stm32f10x.h"                  // Device header

int flag=0,flag1=0,btn1=0,adcvalue=0,temel_timer=0;

void gpio_config(){
	
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); //PC pinleri 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //PA pinleri 
	
	// led
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1; // PC0=PC1=LED
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&init);
	
	// button
	init.GPIO_Mode=GPIO_Mode_IPD;
	init.GPIO_Pin= GPIO_Pin_2; 				//PC2=button
	GPIO_Init(GPIOC,&init);
	
	// adc
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_3 | GPIO_Pin_4; // PC3 ,PC4
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&init);
	
	// dac
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_4; 			//	PA4
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	// timer_pwm
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_15; 			//	PA15
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
}

void adc_config(){
	ADC_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	init.ADC_ContinuousConvMode=ENABLE;
	init.ADC_DataAlign=ADC_DataAlign_Right;
	init.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	init.ADC_Mode=ADC_Mode_Independent;
	init.ADC_NbrOfChannel=1;
	init.ADC_ScanConvMode=DISABLE; // multi adclerde enable yap ,hepsini okusunmu anlami var
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_Init(ADC1,&init);
}

void dac_config(){
	DAC_InitTypeDef init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	init.DAC_LFSRUnmask_TriangleAmplitude=DISABLE;
	init.DAC_OutputBuffer=ENABLE;
	init.DAC_Trigger=DAC_Trigger_None;
	init.DAC_WaveGeneration=DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_1,&init);
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
}

void interrupt_config(){
	
	EXTI_InitTypeDef init_exti;
	NVIC_InitTypeDef init_nvic;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2); // GPIOC de PC2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4); // GPIOC de PC4 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // coklu interrupt için group aldik ,2 tane izin veriyor group 1
	
	init_exti.EXTI_Line=EXTI_Line2;
	init_exti.EXTI_LineCmd=ENABLE;
	init_exti.EXTI_Mode=EXTI_Mode_Interrupt;
	init_exti.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&init_exti);
	
	// btn1  PC2
	init_nvic.NVIC_IRQChannel=EXTI2_IRQn;
	init_nvic.NVIC_IRQChannelCmd=ENABLE;
	init_nvic.NVIC_IRQChannelPreemptionPriority=1;
	init_nvic.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&init_nvic);
	
	 // btn2 PC4
	init_nvic.NVIC_IRQChannel=EXTI4_IRQn;
	init_nvic.NVIC_IRQChannelCmd=ENABLE;
	init_nvic.NVIC_IRQChannelPreemptionPriority=1;
	init_nvic.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&init_nvic);
}

void timer_config(){
	TIM_TimeBaseInitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); // temel zamanlayicilar tim6,tim7 , 16 bit max deger alir 0 to 2^16 yani sayar kendi kendine
	
	init.TIM_ClockDivision=1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=11999;
	init.TIM_Prescaler=1999;
	init.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM6,&init);
	TIM_Cmd(TIM6,ENABLE);
}

void timer_interrupt_config(){
	TIM_TimeBaseInitTypeDef init_timer;
	//NVIC_InitTypeDef init_nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// TIM2
	init_timer.TIM_ClockDivision=1;
	init_timer.TIM_CounterMode=TIM_CounterMode_Up;
	init_timer.TIM_Period=11999; // 2 sn'de bir interrupt yapsin   ,((period+1)*(prescaler+1))/( 24bit yani 24*2^6 )  time verir sn cinsinden
	init_timer.TIM_Prescaler=3999;
	init_timer.TIM_RepetitionCounter=0;
	
	TIM_Cmd(TIM2,ENABLE);
	TIM_TimeBaseInit(TIM2,&init_timer);

//	 // multi interrupt da ayri function da aldim
//	init_nvic.NVIC_IRQChannel=TIM2_IRQn;
//	init_nvic.NVIC_IRQChannelCmd=ENABLE;
//	init_nvic.NVIC_IRQChannelPreemptionPriority=1;
//	init_nvic.NVIC_IRQChannelSubPriority=0;
//	
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//	NVIC_Init(&init_nvic);
	
	
	//TIM3
	init_timer.TIM_ClockDivision=1;
	init_timer.TIM_CounterMode=TIM_CounterMode_Up;
	init_timer.TIM_Period=11999; // 1 sn'de bir interrupt yapsin   ,((period+1)*(prescaler+1))/( 24bit yani 24*2^6 )  time verir sn cinsinden
	init_timer.TIM_Prescaler=1999;
	init_timer.TIM_RepetitionCounter=0;
	
	TIM_Cmd(TIM3,ENABLE);
	TIM_TimeBaseInit(TIM3,&init_timer);
	
	
}

void nvic_config(){
	
	NVIC_InitTypeDef init_nvic;
	
	init_nvic.NVIC_IRQChannel=TIM2_IRQn;
	init_nvic.NVIC_IRQChannelCmd=ENABLE;
	init_nvic.NVIC_IRQChannelPreemptionPriority=1;
	init_nvic.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	NVIC_Init(&init_nvic);
	
	init_nvic.NVIC_IRQChannel=TIM2_IRQn;
	init_nvic.NVIC_IRQChannelCmd=ENABLE;
	init_nvic.NVIC_IRQChannelPreemptionPriority=2;
	init_nvic.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_Init(&init_nvic);
}

void timer_pwm_config(){
	TIM_TimeBaseInitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
	
	init.TIM_ClockDivision=1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=10;
	init.TIM_Prescaler=2399; // 1 khz aliriz, mesela 10khz yapsan ledin yandigini göremezsin hizli olur ac kapa gibi
	init.TIM_RepetitionCounter=0; // advance timer larda kullanilir
	
	TIM_TimeBaseInit(TIM15,&init);
	TIM_Cmd(TIM15,ENABLE);
}


void delay(uint32_t tt) { while(tt--); }

float adc_map(uint16_t value,float max,float min,float cmax,float cmin) { return value*((cmax-cmin)/(max-min)); }


void pwm_config(uint16_t value){
	TIM_OCInitTypeDef init;
	
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable;
	init.TIM_Pulse=value;
	TIM_OC1Init(TIM2,&init);
	TIM_OC1PreloadConfig(TIM15,TIM_PSCReloadMode_Update); // 2 channel var OC1, OC2 data sheet sayfa 8 , PA3 için
}

void TIM2_IQRHandler(){
	if(flag1==0){
		flag=1;
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		delay(360000);
	}else{
		flag1=0;
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		delay(360000);
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

void TIM3_IQRHandler(){
	if(flag==0){
		flag=1;
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		delay(360000);
	}else{
		flag=0;
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		delay(360000);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void toggle_led(){
	uint16_t i=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0);
	if(i==Bit_SET){
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		delay(360000);
	}else{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		delay(360000);
	}
}



void EXTI2_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET){
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET);
		delay(3600000);
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET);
		delay(3600000);
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI4_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET);
		delay(3600000);
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET);
		delay(3600000);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

uint16_t adc_function(){
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,1,ADC_SampleTime_28Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetITStatus(ADC1,ADC_IT_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
}


int	main(){
	
	gpio_config();
	adc_config();
	dac_config();
	interrupt_config();
	timer_config();
	timer_interrupt_config();
	nvic_config();
	timer_pwm_config();
	
	int flag=0,btn1=0,adcvalue=0,temel_timer=0;
	
	while(1){
		
//		 // led deneme
//		GPIO_SetBits(GPIOC,GPIO_Pin_1);
//		delay(360000);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//		delay(360000);
	
		
//		 // buton deneme
//		btn1=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
//		if(btn1==1){
//			if(flag==0){
//				flag=1;
//				GPIO_SetBits(GPIOC,GPIO_Pin_1);
//				delay(360000);
//			}else{
//				flag=0;
//				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//				delay(360000);
//			}
//		}
		
		
//		  // adc deneme
//		adcvalue=adc_function();
//		if(adcvalue>2000){
//				GPIO_SetBits(GPIOC,GPIO_Pin_1);
//				delay(360000);
//		}else{
//				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//				delay(360000);
//		}
		
		
//		 // dac deneme    ,8b =8bit = 256  ,12b =4096 =12bit
//		for(int i=0;i<255;i++){
//			DAC_SetChannel1Data(DAC_Align_8b_R,i);
//		}
		
		
//		 // temerl timer tim6
//		temel_timer=TIM_GetCounter(TIM6);
//		if(temel_timer==3000){ // max deger 11999 olur 
//			GPIO_SetBits(GPIOC,GPIO_Pin_0 );
//		}else if(temel_timer==8000){
//			GPIO_ResetBits(GPIOC,GPIO_Pin_0 );
//		}
		
		
//		 // timer_pwm deneme max 2399 alirsin %100 perfonmans ile
//		pwm_config(599);
//		delay(3600000);
//		pwm_config(1199);
//		delay(3600000);
//		pwm_config(1799);
//		delay(3600000);
//		pwm_config(2399);
//		delay(3600000);
		
		
	}
}
*/


















/* // genel deneme1
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

uint16_t adcvalue=0,dacvalue=0,cnt=0;

void gpio_conf(){
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); // led
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); // buton
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); // adc,dac
	
	// led config 
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; // PB0,PB1,PB2  led yaptim
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&init);
	
	// but config
	init.GPIO_Mode=GPIO_Mode_IPD;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1; // PC0 buton, PC1 interrupt buton exti
	GPIO_Init(GPIOC,&init);
	
	// adc
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_0;
	
	GPIO_Init(GPIOA,&init);
	
	// dac
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_4;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
}

// interrupt config
void exti_conf(){
	EXTI_InitTypeDef init_exti;
	NVIC_InitTypeDef init_nvic;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1); // Selects the GPIO pin used as EXTI Line.
	
	// exti
	init_exti.EXTI_Line=EXTI_Line1;
	init_exti.EXTI_LineCmd=ENABLE;
	init_exti.EXTI_Mode=EXTI_Mode_Interrupt;
	init_exti.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&init_exti);
	
	//NVIC_GetPriorityGrouping(NVIC_PriorityGroup_0);// kullanmasanda olur misc.h de ,2 tane interrupt kullanacaksan öncelik istersen group1 de  preemprioty sub priority den ayarla 
	//nvic
	init_nvic.NVIC_IRQChannel=EXTI1_IRQn;
	init_nvic.NVIC_IRQChannelCmd=ENABLE;
	init_nvic.NVIC_IRQChannelPreemptionPriority=1;
	init_nvic.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&init_nvic);
}
// interrupt function
void EXTI1_IRQHandler(){
		
		if(EXTI_GetITStatus(EXTI_Line1)!=RESET){
			
			// whatever you want like blink led
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			for(int i=0;i<360000;i++);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			for(int ii=0;ii<360000;ii++);
			
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
}

// adc config
void adc_conf(){
	
	ADC_InitTypeDef init;
	
	// bu channel olayi adc1 bir channel dir.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // stm32vldisvoery pdf inde page 8 de adc1 var bir tek, adc2 ve adc3 stm32f4 lerde var 
	
	init.ADC_ContinuousConvMode=ENABLE;
	init.ADC_DataAlign=ADC_DataAlign_Right;
	init.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	init.ADC_Mode=ADC_Mode_Independent; // bagimsiz olsun dedik kisaca
	init.ADC_NbrOfChannel=1; // 1 tane adc kullandigimiz için 
	init.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&init);
	ADC_Cmd(ADC1,ENABLE);
}
// adc function
uint16_t adcfunction(){
	// channel_0 cünkü biz PA0 pin'ini kullaniyoruz ,PA2 olsa idi, channel_2 olurdu  ,ADC1_IN0 manuel book ta IN0 var,channel_0 demek
	// hoca 1 yapti ben 0 yaptim farketmez herhalde
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,0,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetITStatus(ADC1,ADC_IT_EOC)==RESET);
	
	return  ADC_GetConversionValue(ADC1);
	
}

// dac config
void dac_conf(){
	
	DAC_InitTypeDef init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	init.DAC_LFSRUnmask_TriangleAmplitude=DISABLE;
	init.DAC_OutputBuffer=ENABLE;
	init.DAC_Trigger=DAC_Trigger_None;
	init.DAC_WaveGeneration=DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_2,ENABLE); // channel 2 PA5 dir, PA4 ise channel 1 de kullanilir
	DAC_Cmd(DAC_Channel_2,ENABLE);
	
	
}

// timer config
void tim_conf(){
	
	TIM_TimeBaseInitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	init.TIM_ClockDivision=1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=3999;
	init.TIM_Prescaler=5999;
	init.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM6,&init);
	TIM_Cmd(TIM6,ENABLE);
}

void tim_conf1(){
	
	TIM_TimeBaseInitTypeDef init;
	NVIC_InitTypeDef init_nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	init.TIM_ClockDivision=1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=11999;
	init.TIM_Prescaler=3999;
	init.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM3,&init);
	TIM_Cmd(TIM3,ENABLE);
	
	init_nvic.NVIC_IRQChannel=TIM3_IRQn;
	init_nvic.NVIC_IRQChannelCmd=ENABLE;
	init_nvic.NVIC_IRQChannelPreemptionPriority=1;
	init_nvic.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); // TIM_ITConfig : Enables or disables the specified TIM interrupts.
	NVIC_Init(&init_nvic);
}

void	toggle_led(){
	uint16_t ebru_red=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
	if(ebru_red == (uint16_t)Bit_SET ){
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}else{
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}
}

// bu function normalde stm32f4 kartlarinda içine gömülüdür ama stm32f100rb kartinda yok ,hocada kendi yazdi
void TIM3_IRQHandler(){
	toggle_led();
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}


int main(){
	gpio_conf();
	exti_conf();
	adc_conf();
	dac_conf();
	tim_conf();
	//tim_conf1();
	while(1){
		
		adcvalue=adcfunction(); // adc 
		
		//dac
		for(int i=0;i<255){
			DAC_SetChannellData(DAC_Align_8b_R,i); // 8b= 8bit=256 biz 255 yazalim kafaya göre ,12b= 12bit =4096
			//DAC_SetChannellData(DAC_Align_8b_R,adcvalue); // adc -> dac vereceksen mesela led yakacaksin voltaj'a göre
			for(int j=0;j<36000;j++);
		}
		
		//timer, tim_conf için ,
		cnt=TIM_GetCounter(TIM6);
		if(cnt>1999){
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			for(int i=0;i<360000;i++);
		}else if(cnt>3999){
			GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			for(int ii=0;ii<360000;ii++);
		}
		
		// tim_conf1 için bir sey yazmayiz timer_interrupt ,zamani geldimi auto interrupt eder.
	}
}
*/