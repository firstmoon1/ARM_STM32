#include "stm32f10x.h"                  // Device header
#include "stm32f10x_pwr.h"              // Keil::Device:StdPeriph Drivers:PWR
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void gpioConfig(){
   GPIO_InitTypeDef GPIOInitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
   
   GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
   GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
   GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   
   GPIO_Init(GPIOB,&GPIOInitStructure);

}
void timerConfig(){
   
   TIM_TimeBaseInitTypeDef TIMERInitStructure;
   NVIC_InitTypeDef        NVICInitStructure;
   
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
   
   TIMERInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
   TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
   TIMERInitStructure.TIM_Period=5999;
   TIMERInitStructure.TIM_Prescaler=3999;
   TIMERInitStructure.TIM_RepetitionCounter=0;
   
   TIM_TimeBaseInit(TIM3,&TIMERInitStructure);
   TIM_Cmd(TIM3,ENABLE);
   
   NVICInitStructure.NVIC_IRQChannel=TIM3_IRQn;
   NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
   NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
   NVICInitStructure.NVIC_IRQChannelSubPriority=0;
   
   TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
   NVIC_Init(&NVICInitStructure);
	 
	 NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,ENABLE);//sleep mode aktif
}


void TIM3_IRQHandler(){
   TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	 GPIOB->ODR ^=GPIO_Pin_0;// toggle foksiyonu, PA0 için register seviyesinde toggle yapti , 
	 // ^= bit olarak is görür yani 1 0  ise cevap 1 , eger  1 1 veya 0 0 ise cevap 0 olur,bunu discrete math'de gördük
}


int main(){
	gpioConfig();
	timerConfig();
	
	while(1){
		
		__WFI();// wait for interrut komutu aktif ,__wfi olarak ta yazilabilir ayni sey
		//__WFE(); buton felan kullanmadik timer interrupt yaptik yani wfi ye göre yazdik kodlari
	
	}
}
/*
referance manuel'de sayfa 54'de anlatiyor

Sleep mode (CPU clock off, all peripherals including Cortex®-M3 core peripherals like NVIC, SysTick, etc. are kept running)

sayfa 54 de tablo 8 de ,sleep yada stop  yada standby a girince nasil wakeup yapilacaginida gösteriyor

WFI veya wfi (Wait For Interrupt) yani interrup ile uyanir system
WFE (Wait for Event) yani bir butona bastiginda uyanir hoca böyle yapti pwr stop modunda ,burda sleep modu kullandi
*/


