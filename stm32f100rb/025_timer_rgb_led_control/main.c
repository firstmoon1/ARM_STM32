
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void gpio_conf(){
	
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&init);
}

void timer_conf(){
	TIM_TimeBaseInitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	init.TIM_ClockDivision=TIM_CKD_DIV1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=10; 
	init.TIM_Prescaler=2399;  // word dosyası sayfa 12 de, 1khz almak için
	init.TIM_RepetitionCounter=0;
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
}

void pwm1_conf(uint32_t tt){
	TIM_OCInitTypeDef init;
	
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable;
	init.TIM_Pulse = tt;
	
	TIM_OC1Init(TIM2,&init);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
}

void pwm2_conf(uint32_t tt){
	
	TIM_OCInitTypeDef init;
	
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable;
	init.TIM_Pulse = tt;
	
	TIM_OC2Init(TIM2,&init);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
}

void pwm3_conf(uint32_t tt){
	
	TIM_OCInitTypeDef init;
	
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable;
	init.TIM_Pulse = tt;
	
	TIM_OC3Init(TIM2,&init);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable); 
	// oc1,oc2,oc3 channel dir,ayridir birbirlerinden.
}

void delay(uint32_t tt){
	while(tt--);
}

int main(){
	
	gpio_conf();
	timer_conf();
	
	while(1){
		
		for(int i=0;i<2399;i++){
			pwm1_conf(i);
			delay(3600);
		}
		for(int i=0;i<2399;i++){
			pwm2_conf(i);
			delay(3600);
		}
		for(int i=0;i<2399;i++){
			pwm3_conf(i);
			delay(3600);
		}
		// rgb led in baglandigi bacaklara baktik hangi renkleri vericek diye.
		pwm1_conf(2399);
		pwm2_conf(0);
		pwm3_conf(0);
		delay(360000);
		
		pwm1_conf(0);
		pwm2_conf(2399);
		pwm3_conf(0);
		delay(360000);
		
		pwm1_conf(0);
		pwm2_conf(0);
		pwm3_conf(2399);
	  delay(360000);
		
	}
}


/*



*/

