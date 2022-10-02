
#include "stm32f4xx.h"

uint16_t count=0;

void gpio_conf(){

	GPIO_InitTypeDef init;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init);
}

void delay_ms(uint32_t time){
	count=time;
	while(count);
}

int main(void)
{
	gpio_conf();
	SysTick_Config(SystemCoreClock/1000);
	/*
	 SystemCoreClock/1000    : 1 ms de bir interrupta dallanýr
	 SystemCoreClock/100000  : 10 us de bir interrupta dallanýr
	 SystemCoreClock/1000000 : 1 us de bir interrupta dallanýr

	 stm32f4xx_it.c kütüphanesinde line 141 deki void SysTick_Handler(void){}  function'i kullandýk oraya iþlemler yaptýk
	 kýsaca count-- functionýný oraya yazdýk
	 */

  while (1)
  {
	  GPIO_ToggleBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	  delay_ms(1000);
  }
}
