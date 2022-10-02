#include "stm32f4xx.h"


void delay(uint32_t time)
{
	while(time--);
}

void CLK_Config()
{
	RCC->CR |= 0x00030000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE se�tik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168,yanl�� a.q 5A ya bak 90 veriyor 168 degil ,5A yerine 2A olmas� gerek 168 i�in 0000 0000 0000 0000 0010 1010 0000 0000 dir asl�nda
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2
	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;		// CSS Flag clear
}

void GPIO_Config()
{
	RCC->AHB1ENR = 0x00000009;	// GPIOA and GPIOD active

	GPIOD->MODER = 0x55000000;	// GPIOD pin12,pin13,pin14,pin15 output
	GPIOD->OTYPER = 0x00000000;
	GPIOD->OSPEEDR = 0xFF000000;
	GPIOD->PUPDR = 0x00000000;
}

void EXTI_Config()
{
	RCC->APB2ENR = 0x00004000;	// SYSCFG active (exti), page 249

	SYSCFG->EXTICR[0] = 0x00000000; // PA0,PA1,PA2 yi kulland�g�m�z i�in EXTICR[x] de x=0 yapt�k,PA0 i�in buton baglama c�nk� kartta mavi buton PA0 ,page 291

	NVIC_EnableIRQ(EXTI0_IRQn); // bunlar� bulamad� hoca kitaptan,  ama  nvic de enable ediyoruz �ey gibi d���n ,init �urdan geldi NVIC_InitTypeDef init;,  ,init.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_EnableIRQ(EXTI1_IRQn); // The function enables a device-specific interrupt in the NVIC interrupt controller.
	NVIC_EnableIRQ(EXTI2_IRQn);

	NVIC_SetPriority(EXTI0_IRQn, 0); // externel interrupt0 priority 0 (very high) , The function sets the priority of an interrupt.
	NVIC_SetPriority(EXTI1_IRQn, 1); // external interrupt1 priority 1 (high)
	NVIC_SetPriority(EXTI2_IRQn, 2); // external interrupt2 priority 2 (low)

	EXTI->IMR = 0x00000007;  // Interrupt mask register (EXTI_IMR),3 buton interrupt,hex kodunda 0x7 = 0111 dir,3 buton i�in ilk 3 bit 1 yapt�k,page 384
	EXTI->RTSR = 0x00000007; // Rising trigger selection register,page 385
}

void EXTI0_IRQHandler()
{
	if(EXTI->PR & 0x00000001)   // interrupt flag active  ,page 386
	{
	    GPIOD->ODR = 0x00001000; // gpiod pin 12 set other reset ,page 283
	}
	EXTI->PR = 0x00000001;      // interrupt flag reset

}

void EXTI1_IRQHandler()
{
	if(EXTI->PR & 0x00000002)
	{
	    GPIOD->ODR = 0x00002000; // GPIOD pin 13 set other reset
	}
	EXTI->PR = 0x00000002;
}

void EXTI2_IRQHandler()
{
	if(EXTI->PR & 0x00000004)
	{
	    GPIOD->ODR = 0x00004000; // GPIOD pin 14 set other reset
	}
	EXTI->PR = 0x00000004;
}

int main(void)
{
    CLK_Config();
    GPIO_Config();
    EXTI_Config();
    GPIOD->ODR = 0x00000000;   // reset all pins
  while (1)
  {

  }
}
