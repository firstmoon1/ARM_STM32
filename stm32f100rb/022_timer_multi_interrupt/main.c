#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
/*
bu define lari timer functionda biz port u gönderemedik
uint16_t value=GPIO_ReadInputDataBit( &port_bilgisi , pin_number ); ,port_bilgisi yok aga olmadi
cözüm olarak da bunu buldum #define ile yapariz
*/
/*
#define portA GPIOA
#define portB GPIOB
#define portC GPIOC
#define portD GPIOD
*/

void gpio_config(){
	
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&init);
}

void timer_config(){
	
	TIM_TimeBaseInitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	// tim2 config
	init.TIM_ClockDivision=1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=5999; // ((5999+1)*(999+1))/(24*10^6)  verir bize 1/4 sn'de bir interrupt
	init.TIM_Prescaler=999;
	init.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM2,&init);
	TIM_Cmd(TIM2,ENABLE);
	// tim3 config
	
	init.TIM_ClockDivision=1;
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=11999;   // ((11999+1)*(3999+1))/(24*10^6)  verir bize 2 sn'de bir interrupt
	init.TIM_Prescaler=3999;
	init.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM3,&init);
	TIM_Cmd(TIM3,ENABLE);
	
}

void nvic_config(){
	
	NVIC_InitTypeDef init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // multi interrupt da group kullanmalisin,stm32 klasor de group priority nvic fotolari var
	// kisaca : 2 interrupt var  group 1 de 0 ve 1 saglar preempriority için yani 2 tane interrupt kullanabilirsin max group 1 için
	
	init.NVIC_IRQChannel=TIM2_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=0;
	init.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //TIM_ITConfig : Enables or disables the specified TIM interrupts.
	NVIC_Init(&init);
	
	init.NVIC_IRQChannel=TIM3_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=1;
	init.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_Init(&init);
	
}

 // eksikler var port'u gönderemedim hata aliyorum, gpiob olarak nasil gönderebilim referans alabilir port_bilgisi unknown
 // cözüm olarak ,#define kullandim
 // void toggle_led( GPIO_InitTypeDef* port_bilgisi, uint16_t pin_number,  ){ // bu yöntemle olmadi alttaki ile oldu
 // eger alttaki yöntem ise yaramaz ise yukarda #define ile portA ,portB karsilastirmasi yaparsin if ile function içinde 
void toggle_led( GPIO_TypeDef* port_bilgisi, uint16_t pin_number){
	
	uint16_t value=GPIO_ReadInputDataBit( port_bilgisi , pin_number );
	if (value==(uint16_t)Bit_SET){
		GPIO_ResetBits( port_bilgisi , pin_number);
	}else{
		GPIO_SetBits( port_bilgisi , pin_number);
	}
}

void TIM3_IRQHandle(){
	toggle_led(GPIOB,GPIO_Pin_0);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
void TIM2_IRQHandler(){
	toggle_led(GPIOB,GPIO_Pin_1);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


/* // burda elle gönderdik 
void toggle_led( uint16_t pin_number ){
	
	uint16_t value=GPIO_ReadInputDataBit( GPIOB  , pin_number );
	
	if (value==(uint16_t)Bit_SET){
		GPIO_ResetBits( GPIOB , pin_number);
	}else{
		GPIO_SetBits( GPIOB , pin_number);
	}
}

void TIM3_IRQHandle(){
	toggle_led(GPIO_Pin_0);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void TIM2_IRQHandler(){
	toggle_led(GPIO_Pin_1);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
*/


int main(){
	
	gpio_config();
	timer_config();
	nvic_config();
	while(1){
		
		//
		
	}
}



/*
 bunun olayi 1/4 saniye de bir calisacak
init.TIM_Period=5999;
init.TIM_Prescaler=999;

 bunun olayi 1sn de bir calisacak
init.TIM_Period=11999;
init.TIM_Prescaler=3999;
*/