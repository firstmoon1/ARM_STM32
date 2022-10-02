

#include "stm32f4xx.h"

extern uint32_t SystemCoreClock;
uint32_t system_clock;



void rcc_config(){  // refereans manuelde sayfa 161

	// 8 000 000 -> 168 000 000

	RCC->CR &= ~(1<<0); // HSI OFF
	RCC->CR |= (1<<16); // HSE ON
	while( !(RCC->CR & (1<<17)) ); // HSE FLAG SECURITY
	RCC->CR |= (1<<19);  // SECURITY
	RCC->PLLCFGR = 0x00000000; // 32 BIT ALL ZEROS NOW
	RCC->PLLCFGR |= (1<<22);  //  1 : HSE oscillator clock selected as PLL and PLLI2S clock entry
	RCC->PLLCFGR |=(4<<0); // PLL M  4
	RCC->PLLCFGR |= (168<<6); // PLL N 168

	RCC->CR |= ( 1 << 24 );  // PLL ON
	while(!(RCC->CR & ( 1 << 25 ))); // wait PLL active

	RCC->CFGR &= ~(1 << 0); // PLL selected as system clock ,page 167
	RCC->CFGR |= ( 1<< 1); // PLL selected as system clock
	while(!(RCC->CFGR & (1 << 1)));  //select system clock is pll clock ,

}

void gpio_config(){

	RCC->AHB1ENR |= 1 << 3;  //GPIOD clock enable  , 3 bir sola kaydýrma iþlemi yaptýk 1 = high degerini

	// pin ayarý
	GPIOD->MODER |= (1<<24);  //24'ü 1 yaptýk  , PD12 , 01: General purpose output mode , page 281 , default olarak hepsi input gelir yani 00 olarak
	GPIOD->MODER &= ~(1<<25); //25'i 0 yaptýk  , 0 to 31 , toplam 32 bit var ,her iki bit 1 yeri temsil eder mesela PD12  24 ve 25 inci yeri kapsýyor

	GPIOD->MODER |= (1<<26);  //PD13
	GPIOD->MODER &= ~(1<<27);

	GPIOD->MODER |= (1<<28);  //PD14
	GPIOD->MODER &= ~(1<<29);

	GPIOD->MODER |= (1<<30);  //PD15
	GPIOD->MODER &= ~(1<<31);

	// speed ayarý
	GPIOD->OSPEEDR |= 0xFF000000; // 11 : very high speed yapalým ,32 bit hesap makinesinden yaz 1111 1111 0000 0000 0000 0000 0000 0000 aslýnda
	//GPIOD->OSPEEDR |= (0xFF <<2 4); // bunuda yapabilirsin mesela  yukardakine alternatif ama hoca denemedi

}


int main(void){

	// refereans manuelde sayfa 267 de ise gpio giris kýsmý,gpio register ise  281

	rcc_config();
	SystemCoreClockUpdate(); // 168 MHz cevirdik

	gpio_config(); // bir þeyi unuttuk burda biz GPIOD yi actif etmedik  , page 180 de RCC den GPIOD yi aktif et


  while (1){

	  GPIOD->ODR |= 1 << 12; // 12.pin set yani 1 yani high
	  GPIOD->ODR |= 1 << 13; // 13.pin set
	  GPIOD->ODR |= 1 << 14; // 14.pin set
	  GPIOD->ODR |= 1 << 15; // 15.pin set


	  for(int i=0;i<1680000;i++){

		  GPIOD->ODR &= ~(1 << 12); // 12.pin reset yani 0 yani low
		  GPIOD->ODR &= ~(1 << 13); // 13.pin reset
		  GPIOD->ODR &= ~(1 << 14); // 14.pin reset
		  GPIOD->ODR &= ~(1 << 15); // 15.pin reset
	  }

	  for(int i=0;i<1680000;i++);

  }

}

























