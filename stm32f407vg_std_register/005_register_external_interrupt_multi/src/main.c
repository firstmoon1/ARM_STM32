
#include "stm32f4xx.h"

void delay(uint32_t tt){
	while(tt--);
}

void gpio_config(){

	RCC->AHB1ENR = 0x00000009; // GPÝOD,GPÝOA yý active ettik
	// led config
	GPIOD->MODER = 0x55000000; // genreal purpose output mode
	GPIOD->OTYPER = 0x00000000; // output push pull
	GPIOD->OSPEEDR = 0x00000000; // LOW SPEED
	GPIOD->PUPDR = 0x00000000;
	// buton config e gerek yok default olarak hepsi input,push pull, pull down low speed de ,GPIOA config yapmadýk yani ,ama yapmak istersende haberin olsun ,GPIOA->MODER yapýnca sapýtýyor digerlerinde sýkýntý yok
}


void exti_config(){

	RCC->APB2ENR = 0x00004000; //page 187 ,SYSCFGEN: System configuration controller clock enable, EXTI'ler SYSCFG olarak geciyor

	NVIC_EnableIRQ(EXTI0_IRQn); //PA0 pin,The function enables a device-specific interrupt in the NVIC interrupt controller.
	NVIC_EnableIRQ(EXTI1_IRQn); //PA1 pin
	NVIC_EnableIRQ(EXTI2_IRQn); //PA2 pin

	SYSCFG->EXTICR[0] = 0x00000000; // EXTI0, active ettik ,page 291 ,SYSCFG external interrupt configuration register 1
	SYSCFG->EXTICR[1] = 0x00000000; // EXTI1, active ettik
	SYSCFG->EXTICR[2] = 0x00000000; // EXTI2, active ettik

	NVIC_SetPriority(EXTI0_IRQn,0); // priorityleri belirledik
	NVIC_SetPriority(EXTI1_IRQn,4);
	NVIC_SetPriority(EXTI2_IRQn,3);

	EXTI->IMR  = 0x00000007; //interrupt olarak gösterdik,page 384
	EXTI->RTSR = 0x00000007; // rising trigger enabled

}

void EXTI0_IRQHandler(){ // PA0

	if(EXTI->PR & (1<<0)){ //ben böyle yaptým: 0x00000001, 1: selected trigger request occurred PR0 için
		int i=0;
		do{
			GPIOD->ODR = 0x00001000; //PD12 ON, PAGE 283
			delay(1680000);
			GPIOD->ODR = 0x00000000; //  PD ALL OFF
			delay(1680000);
			i++;

		}while(i<5);
		EXTI->PR = (1<<0);

	}

}

void EXTI1_IRQHandler(){ // PA1
	if(EXTI->PR & (1<<1) ){ // ben böyle yaptým: 0x00000002 , hoca farklý yaptý ,1: selected trigger request occurred PR0 için
		int i=0;
		do{
			GPIOD->ODR = 0x00002000; //PD13 ON, PAGE 283
			delay(1680000);
			GPIOD->ODR = 0x00000000; // PD ALL OFF
			delay(1680000);
			i++;

		}while(i<5);
		EXTI->PR = (1<<1);

	}
}

void EXTI2_IRQHandler(){  // PA2
	if(EXTI->PR & (1<<2)){ //ben böyle yaptým: 0x00000004, 1: selected trigger request occurred PR0 için
		int i=0;
		do{
			GPIOD->ODR = 0x00004000; //PD12 ON, PAGE 283
			delay(1680000);
			GPIOD->ODR = 0x00000000; //  PD ALL OFF
			delay(1680000);
			i++;

		}while(i<5);
		EXTI->PR = (1<<2);

	}
}


int main(void){

	gpio_config();
	exti_config();

  while (1){

  }
}






