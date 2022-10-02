#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config()
{
	RCC->CR |= 0x00030000;										// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));								// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;										// CSS Enable
	RCC->CR |= 0x01000000;										// PLL ON
	RCC->PLLCFGR |= 0x00400000;									// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;									// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;									// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;									// PLL p = 2
	RCC->CFGR |= 0x00000000;									// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;									// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;									// APB1 Prescaler = 4
	RCC->CIR |= 0x00080000;										// HSERDY Flag clear
	RCC->CIR |= 0x00800000;										// CSS Flag clear
}

void GPIO_Config()
{
	RCC->AHB1ENR |= 1 << 3;

	GPIOD->MODER |= 2 << 24 | 2 << 26  |  2 << 28 | 2 << 30;	// Port D -> Pin 12 | Pin 13 | Pin 14 | Pin 15 Alternate Function selected
	GPIOD->AFR[1] |= 2 << 16 | 2 << 20 | 2 << 24 | 2 << 28;		// Port D -> Pin 12 | Pin 13 | Pin 14 | Pin 15 Tim4 -> CH selected
}

void TIM_Config()
{
	RCC->APB1ENR |= 1 << 2;										// TIM4 clock enable

	TIM4->CR1 |= 0 << 4;										// Timer counter mode is Upcounter
	TIM4->CR1 |= 0 << 5;										// Center-aligned mode edge-aligned mode selected
	TIM4->CR1 |= 0 << 8;										// Clock division 1
	TIM4->PSC = 8399;											// Prescaler value 8400
	TIM4->ARR = 9999;											// Period value 10000
	TIM4->CCMR1 |= 0 << 0 | 6 << 4 | 0 << 8 | 6 << 12;
	/*
	 * Capture/Compare 1 output selected
	 * Output Compare 1 pwm mode 1 selected
	 * Capture/Compare 2 output selected
	 * Output Compare 2 pwm mode 1 selected
	 */
	TIM4->CCMR2 |= 0 << 0 | 6 << 4  | 0 << 8| 6 << 12;
	/*
	 * Capture/Compare 3 output selected
	 * Output Compare 3 pwm mode 1 selected
	 * Capture/Compare 4 output selected
	 * Output Compare 4 pwm mode 1 selected
	 */
	TIM4->CCER |= 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12;
	/*
	 * Capture/Compare 1 output enable
	 * Capture/Compare 2 output enable
	 * Capture/Compare 3 output enable
	 * Capture/Compare 4 output enable
	 */
	TIM4->CCR1 |= 2499;											// TIM4 Channel 1 pulse value is 2499
	TIM4->CCR2 |= 4999;											// TIM4 Channel 2 pulse value is 4999
	TIM4->CCR3 |= 7499;											// TIM4 Channel 3 pulse value is 7499
	TIM4->CCR4 |= 9998;											// TIM4 Channel 4 pulse value is 9999
	TIM4->RCR = 0;												// TIM4 Repetition Counter Value 0
	TIM4->CR1 |= 1 << 0;										// Counter Enable
}

int main(void){

	RCC_Config();
	GPIO_Config();
	TIM_Config();

  while (1){


  }

}
/*
formuller:

 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = timer_tick_frequency/(period+1)
 timer_tick_frequency = timer_clk/(prescaler+1)
 pulse = [(period+1)*(duty cycle)]-1

misal mesela elimizde bir motor var ve 100KHz ile calýþmasý gerek,o zaman pwm frequency 100KHz olmalý

 timer_tick_frequency = timer_clk/(prescaler+1)
 10000 = 84000000/8400
 timer_tick_frequency = 10000
 timer clock = 84MHz == 84*10^6
 prescaler :8399 olsun +1 yapýnca 8400 olur.

 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = (timer_tick_frequency)/(period+1)
 1 = 10000 / (9999+1)
 period :9999 olsun +1 yapýnca 10000
 pwm_frequency = 1Hz



PD12 için %100 duty cycle ,pulse :9999 = [(9999+1)*(100/100)]-1
PD13 için %75 duty cycle ,pulse  :7499 = [(9999+1)*(75/100)]-1
PD14 için %50 duty cycle ,pulse  :4999 = [(9999+1)*(50/100)]-1
PD15 için %25 duty cycle ,pulse  :2499 = [(9999+1)*(25/100)]-1



 */















