
#include "stm32f4xx.h"

uint16_t adc_value=0;

void rcc_config(){

	RCC->CR |= 0x00030000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2
	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CFGR |= 0x0000000A;	// SW1:10 SYSCLK PLL STATUS FLAG ,SW0:10 SYSCLK PLL,
	RCC->CIR |= 0x00800000;		// CSS Flag clear
	/*
	// hocanýn yaptýklarýný kendimce yaptým ama  = kullanýnca system default olanlarýda bozuyoruz sýkýntý yani ,hocanýn bazý yanlýþlarýný düzelttim
	RCC->CR = 0x010B0000; //pin24,19,17,16 set 1, page 224
	RCC->PLLCFGR = 0x00402A04;
	RCC->CFGR = 0x0000940A; //PPRE2:100:APB2/2,PPRE1:101 APB1/2,HPRE:0000 AHB /1, SW1:10 SYSCLK PLL STATUS FLAG ,SW0:10 SYSCLK PLL,
	RCC->CIR = 0x00800000;
	*/
}

void gpio_config(){

	RCC->AHB1ENR |= 0x00000001;	// GPIOA Clock Enable

	GPIOA->MODER |= 0x00000103;	// Pin 0 Analog ,PA4 led
	GPIOA->OSPEEDR |= 0x00000003;	// Pin 0 100MHz;
	GPIOA->OSPEEDR |= 0x00000000;
	/*// kendim yazdým .
	RCC->AHB1ENR |= 0x00000001;
	GPIOA->MODER =  0x00000003;
	GPIOA->OSPEEDR= 0x00000002; // Pin0 high speed;
	*/
}

void adc_config(){

	RCC->APB2ENR |= 0x00000100;	// ADC1 Clock enable,page 248
	ADC1->CR1 |= 0x02000000;	// Resolution 8 bit page 416 , anlamadým stdp ile yapýnca olmamýstý ibne burda oldu demekki stdp de bokluk var
	ADC1->CR2 |= 0x00000001;	// ADC enable //page 418
	ADC1->SMPR2 |= 0x00000003;	// 56 Cycles
	ADC->CCR |= 0x00010000;		// Div 4

	/*
	RCC->APB2ENR |= 0x00000100;// PAGE 248
	ADC1->CR1 |= 0x02000000; // PAGE 416 ,
	ADC1->CR2 |= 0x00000001;
	ADC1->SMPR2 |= 0x00000003;
	ADC->CCR |= 0x00010000;
	*/
}

uint16_t read_adc(){

	uint16_t value;
	ADC1->CR2 |= 0x40000000; //page 418
	while(!(ADC1->SR & 0x00000002)); // page 415
	value= ADC1->DR; // page 425
	return value;

}

int main(void){
	rcc_config();
	gpio_config();
	adc_config();

  while (1){
	  adc_value=read_adc();
	  if(adc_value>100){
		  GPIOA->ODR =0x00000010; // max 256 oluyor 8bit ,100 üstü gelirse led yanar PA4 ,
	  }else{
		  GPIOA->ODR =0x00000000; // led söner
	  }
  }

}
