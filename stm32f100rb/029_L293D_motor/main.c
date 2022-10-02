

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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);  //button
	
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
	init.GPIO_Pin=forward_pin | backward_pin;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(port,&init);
	
}
void gpio_config(){
	
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// pwm signal için
	init.GPIO_Mode=GPIO_Mode_AF_PP; 
	init.GPIO_Pin=GPIO_Pin_2; // PA2 yi pwm signal olarak kullancaz, bunlarda motor forward backward için PA0,PA1
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
}

void timer_config(){
	TIM_TimeBaseInitTypeDef init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	init.TIM_ClockDivision=1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=2399;
	init.TIM_Prescaler=10;
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
	timer_pwm_function( map(speed,255,0,2399,0) );  // 255 yerine 100 de diyebilirdin formul ile alakali   ,gönderdigin ile bu 255 sadelesiyor ya kalanlada 2399 carpiliyor
}																									// max 2399 gider motor'a kisaca

void gobackward(GPIO_TypeDef *port, uint16_t forward_pin,uint16_t backward_pin,uint16_t speed){
	GPIO_ResetBits(port,forward_pin);
	GPIO_SetBits(port,backward_pin);
	timer_pwm_function( map(speed,255,0,2399,0) );  // 255 yerine 100 de diyebilirdin formul ile alakali   ,gönderdigin ile bu 255 sadelesiyor ya kalanlada 2399 carpiliyor
}																									// max 2399 gider motor'a kisaca

void stop(GPIO_TypeDef *port, uint16_t forward_pin,uint16_t backward_pin){
	GPIO_ResetBits(port,forward_pin | backward_pin);
		
}


int main(){
	
	motor_init(portA,pin0,pin1);
	gpio_config();
	timer_config();
	
	
	while(1){
		goforward(portA,pin0,pin1,200);
		delay(3600000);
		gobackward(portA,pin0,pin1,200);
		delay(3600000);
		stop(portA,pin0,pin1);
	}
	
}


