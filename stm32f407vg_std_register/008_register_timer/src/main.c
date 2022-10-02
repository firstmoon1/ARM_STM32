
#include "stm32f4xx.h"

uint16_t cnt=0;

void rcc_conf(){

	RCC->CR |= 0x00010000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4    aslýnda 20       8
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168  aslýnda  488     320 // yalan a.q 168 degil 90 bu ,aþagýda anlattým en altta
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2   aslýnda   2 debug da baktýgýmda gördügüm   2

	// kendimce aslýnda tek satýrdada yapabiliriz gibime geliyor mesela  RCC->PLLCFGR |= 0x00402A04; // tabi denemedim PLLCFGR için bu
	// hocanýnkine göre aslýnda tek satýrdada yapabiliriz gibime geliyor mesela  RCC->PLLCFGR |= 0x00405A04; // tabi denemedim PLLCFGR için bu

	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2 ,bence bu HSE/8 yaptý bunu temsil ediyor 0000 0000 0000 1000 0000 0000 0000 0000 budur yani
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4 ,APB2 Prescaler = not divided oldu APB2 bos aslýnda ,bence bura yanlýs yaptý hoca , olmasý gereken RCC->CFGR |= 0x00009400;  olmalý ki APB1 Prescaler = 4 binary 101, APB2 Prescaler = 2  binary 100   ,kitapta sayfa 166
	RCC->CIR  |= 0x00080000;		// HSERDY Flag clear
	RCC->CIR  |= 0x00800000;		// CSS Flag clear
}

void tim_conf(){

	TIM2->CR1 |= 0<<4; // tim2 clock enable,page 627
	TIM2->CR1 |= 0<<5; // counter mode is upcounter
	TIM2->CR1 |= 0<<8; // center aligned mode edge-aligned mode
	TIM2->SMCR |= 0<<0; // set the clock division to 1,page 630
	TIM2->EGR |= 1<<0; //internal clock source,page 635
	TIM2->PSC =41999; //timer update generation,page 642
	TIM2->ARR=4000; // page 642
	TIM2->CR1 |=1<<0;// týmer2 counter enable,page630


}


int main(void){

	rcc_conf();
	tim_conf();

  while (1){

	  cnt=TIM2->CNT;

  }
}
