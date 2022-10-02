#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x.h"                  // Device header

void gpio_conf(){
	
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&init);
	
}

void timer_conf(){
	
	 // general purpose katagorisindedir tim2,3,4,5,8,9,10 to 17 bunlar interrupt edilebilir
	
	TIM_TimeBaseInitTypeDef init_time; // timer objesi
	NVIC_InitTypeDef init_nvic; // nvic aslinda interruptlari kontrol etmek için kullanilir burda timer_interrupt kullandik 
	// buton a basinca interrupt etmeyecek yani direk her 2 sn de led toggle edicek
	
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	init_time.TIM_ClockDivision=1; // 65535'i bölmedik ,oldugu gibi 65535 'e kadar saysin dedik
	init_time.TIM_CounterMode=TIM_CounterMode_Up; // 1 to 65535 e kadar saysin
	init_time.TIM_Period=11999; // 2sn de bir interrupt
	init_time.TIM_Prescaler=3999;
	init_time.TIM_RepetitionCounter=0; // advance timerlarda kullanilir, general usage timerlarinda gerek yoktur
	
	TIM_TimeBaseInit(TIM3,&init_time);
	TIM_Cmd(TIM3,ENABLE);
	
	init_nvic.NVIC_IRQChannel=TIM3_IRQn;
	init_nvic.NVIC_IRQChannelCmd=ENABLE;
	init_nvic.NVIC_IRQChannelPreemptionPriority=1;
	init_nvic.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_Init(&init_nvic);
	
}

// bu function normalde stm32f4 kartlarinda içine gömülüdür ama stm32f100rb kartinda yok ,hocada kendi yazdi
void toggle_led(){
	uint16_t ebru_red=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
	if(ebru_red == (uint16_t)Bit_SET ){
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}else{
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	}
}


void TIM3_IRQHandler(){
	toggle_led();
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); // sag tik ,go to definition,simply : Clears the TIMx's interrupt pending bits.
}

int main(){
	
	gpio_conf();
	timer_conf();
	// bir sey yapmaya gerek yok while de ,timer interrupt'i burda 2 sn yeye ayarladik 11999 ve 3999 hesaplamasi ile stm32 klasor  timers word dosyasinda
	// formuller var,asagiyada yazdim
	while(1){
	
	}
}




/*

TIM_ITConfig : Enables or disables the specified TIM interrupts. ,sag tik then go to definition
TIM_IT_Update felan hepsi definitionda var mesela : 
TIM10, TIM11, TIM13, TIM14, TIM16 and TIM17 can have TIM_IT_Update or TIM_IT_CC1. 


Bit_SET : led yada ne varsa set edilmis, logic de 1 anlami tasir
Bit_RESET : led yada ne varsa reset edilmis, logic de 0 anlami tasir


bunlari nasil oldu :
init_time.TIM_Period=11999;
init_time.TIM_Prescaler=3999; 
bir önceki derste formul vardi event time diye o frekans cinsinden biz  bunu time a cevirelim ,frekans=1/time

time= ( (period+1)*(prescaler+1) ) / (timerclock)
2sn= ((11999+1)*(3999+1)) / (24*10^6) ,biz bu 11999 ve 3999 'u kendimiz tahminen veriyoruz uyduruyoz, kosul 16 bit olan 65535'i geçmemekdir


*/