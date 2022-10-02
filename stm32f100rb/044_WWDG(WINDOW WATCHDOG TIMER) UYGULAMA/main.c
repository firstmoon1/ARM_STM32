#include "stm32f10x.h"                  // Device header
#include "stm32f10x_wwdg.h"             // Keil::Device:StdPeriph Drivers:WWDG


void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
  GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);

}

void WWDGConfig(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_DeInit(); // resetledik komple kendimiz ayar yaptik
	WWDG_SetPrescaler(WWDG_Prescaler_8); // 1,2,4,8 cesitleri var
	WWDG_SetWindowValue(120); // Sets the WWDG window value,This parameter value must be lower than 0x80. yani 128 den az olmali ,128 dahil degil
	WWDG_Enable(100); // Enables WWDG and load the counter value. This parameter must be a number between 0x40 and 0x7F. ,yani 64 ile 127 arasi degerler alir  
	WWDG_EnableIT(); //  Enables the WWDG Early Wakeup interrupt(EWI).

}

void NVICConfig(){
	
	NVIC_InitTypeDef NVICInitStructure;
	
	NVICInitStructure.NVIC_IRQChannel=WWDG_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&NVICInitStructure);
}
 
void WWDG_IRQHandler(){ // bu WWDG_IRQHandler ismi degistiremezsin sabit watchdoh function ismidir
	
	WWDG_ClearFlag();
	WWDG_SetCounter(100);
	GPIOB->ODR ^=GPIO_Pin_0; //registerlar yani low level ile yaptik ,^=  aslinda != bu olarak anlami var bitsel olarak kullaniliyor sanirim

}

int main(){
	gpioConfig();
	WWDGConfig();
	NVICConfig();
	
	while(1){
		////
	}
}

/* 

time formul = (1000/Clock degeri)*4096*prescaler*(WWDG_SetWindowValue-WWDG_SetCounter)
clock stm32f100rb kartinda 24MHz dir. 

TIME=(1000/24000000)*4096*PRESCALER*(WINDOW-COUNTER)
TIME=(1000/24000000)*4096*8*(120-100)
TIME=27.3 ms dir ,yani her 27.3 ms de bir interrupt olucak


*/

