

// 94. L293D ENTEGRESI ILE DC MOTOR SÜRME KODLAYALIM 2(ILERI-GERI-STOP-FUNCTIONS)


#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define portA GPIOA
#define portB GPIOB
#define portC GPIOC

#define pin0 (uint16_t)GPIO_Pin_0
#define pin1 (uint16_t)GPIO_Pin_1
#define pin2 (uint16_t)GPIO_Pin_2
#define pin3 (uint16_t)GPIO_Pin_3
#define pin4 (uint16_t)GPIO_Pin_4
#define pin5 (uint16_t)GPIO_Pin_5
#define pin6 (uint16_t)GPIO_Pin_6
#define pin7 (uint16_t)GPIO_Pin_7
#define pin8 (uint16_t)GPIO_Pin_8
#define pin9 (uint16_t)GPIO_Pin_9
#define pin10 (uint16_t)GPIO_Pin_10
#define pin11 (uint16_t)GPIO_Pin_11
#define pin12 (uint16_t)GPIO_Pin_12
#define pin13 (uint16_t)GPIO_Pin_13
#define pin14 (uint16_t)GPIO_Pin_14
#define pin15 (uint16_t)GPIO_Pin_15


void delay(uint16_t tt){
	while(tt--);
}

float map(uint16_t value,float max,float min,float cmax,float cmin){
	return value*((cmax-cmin)*(max-min));
}

void motor_init(GPIO_TypeDef* port,uint16_t forward_pin,uint16_t backward_pin){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); // aslinda butonlar için açmasak da acalim diye 
	
	if(port==portA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	}else if(port==portB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	}else if(port==portC){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	}else{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	}
	GPIO_InitTypeDef init;
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=forward_pin | backward_pin; // mesela PA0, PA1
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(port,&init);
	
}

void gpio_config(){
	
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	// PA2 = pwm signal olarak kullancaz, TIM2_CH3
	// PA0,PA1 = motor forward backward için  mesela yani yukarda tanimladik
	// PA3,PA4,PA5 = buton forward,backward,stop
	init.GPIO_Mode=GPIO_Mode_AF_PP; 
	init.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; 
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
}

void exti_config(){
	
	EXTI_InitTypeDef init;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
	
	init.EXTI_Line=EXTI_Line3 | EXTI_Line4 | EXTI_Line5;  // 3=forward ,4=backward ,5=stop
	init.EXTI_LineCmd=ENABLE;
	init.EXTI_Mode=EXTI_Mode_Interrupt;
	init.EXTI_Trigger=EXTI_Trigger_Rising;
	
	EXTI_Init(&init);
}

void nvic_config(){
	
	NVIC_InitTypeDef init;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 3 buton kullancaz
	
	init.NVIC_IRQChannel=EXTI3_IRQn; // 3=forward button, PA3
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=1;
	init.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&init);
	
	init.NVIC_IRQChannel= EXTI4_IRQn ; // 4=backward button PA4
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=2;
	init.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&init);
	
	init.NVIC_IRQChannel=EXTI9_5_IRQn; // 5=stop button PA5
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=0;
	init.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&init);
	
}


void timer_config(){
	TIM_TimeBaseInitTypeDef init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//  Specifies the clock division. This parameter can be a value of @ref TIM_Clock_Division_CKD
	// normalde 1 felan yaziyoz ama stm32f10x_tim.h de line 63 de böyle yaz diyor
	init.TIM_ClockDivision=TIM_CKD_DIV1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=2399; // 1KHz için  (2.4*10^6(prescaler=10))/(period+1)= Hz verir ,neden 2.4 degilde 24 cünkü prescaler, normalde bu clock hat 24mhz dir 24*10^6 
	init.TIM_Prescaler=10; // ve bu formude prescaler i dahil  etme sonuc (2.4*10^6)/(period+1) =hz eger prescaler 10 ise ,100 ise 0.24*10^6 
	init.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM2,&init);
	TIM_Cmd(TIM2,ENABLE);
	
}

void timer_pwm_function(uint16_t value){
	TIM_OCInitTypeDef init;
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable;
	init.TIM_Pulse=value;
	
	TIM_OC3Init(TIM2,&init);
	TIM_OC3PreloadConfig(TIM2,TIM_PSCReloadMode_Update);
}



void goforward(GPIO_TypeDef *port, uint16_t forward_pin,uint16_t backward_pin,uint16_t speed){
	GPIO_SetBits(port,forward_pin);
	GPIO_ResetBits(port,backward_pin);
	timer_pwm_function( map(speed,100,0,2399,0) );  // 100 gönderdim önceki derste 255 gönderdik, map formul ile alakali ,gönderdigin ile bu 255 sadelesiyor ya kalanlada 2399 carpiliyor
}																									// max 2399 gider motor'a kisaca

void gobackward(GPIO_TypeDef *port, uint16_t forward_pin,uint16_t backward_pin,uint16_t speed){
	GPIO_ResetBits(port,forward_pin);
	GPIO_SetBits(port,backward_pin);
	timer_pwm_function( map(speed,100,0,2399,0) );  // 100 gönderdim önceki derste 255 gönderdik, map formul ile alakali ,gönderdigin ile bu 255 sadelesiyor ya kalanlada 2399 carpiliyor
}																									// max 2399 gider motor'a kisaca

void stop(GPIO_TypeDef *port, uint16_t forward_pin,uint16_t backward_pin){
	GPIO_ResetBits(port,forward_pin | backward_pin);
		
}


// forward go PA3
void EXTI3_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET){
		goforward(portA,pin0,pin1,50);  // max 100 hiz olur yukarda map 100 yaptim, half hizda calissin
		delay(360000);
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}
// backward go  PA4
void EXTI4_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET){
		gobackward(portA,pin0,pin1,50);
		delay(360000);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}
// stop PA5
void EXTI9_5_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET){
		stop(portA,pin0,pin1);
		delay(360000);
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
}


int main(){
	
	motor_init(portA,pin0,pin1);
	gpio_config();
	timer_config();
	exti_config();
	nvic_config();
	
	while(1){
	;
	}
}


