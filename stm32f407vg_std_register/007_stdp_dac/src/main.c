
#include "stm32f4xx.h"

uint32_t i=0;

void gpio_conf(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//dac
	init.GPIO_Mode=GPIO_Mode_OUT; // hal kütüphanesi ile yapýnca modu analog'a alýyor,mustafa ise output alýyor.
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_5;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);


}
void dac_conf(){

	DAC_InitTypeDef init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

	init.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
	init.DAC_Trigger=DAC_Trigger_None;
	init.DAC_WaveGeneration=DAC_WaveGeneration_None;

	DAC_Init(DAC_Channel_2,&init);
	DAC_Cmd(DAC_Channel_2,ENABLE);
}

void delay(uint32_t tt){
	while(tt--);
}
int main(void){

	gpio_conf();
	dac_conf();

  while (1){

	  for(i=0;i<4096;i++){
		  DAC_SetChannel2Data(DAC_Align_12b_R,i);
		  delay(10000);
	  }


  }
}
