
#include "stm32f4xx.h"

uint32_t adc_value=0,i=0;

void gpio_conf(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//dac
	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_4; // 2 tane var kartta, PA4,PA5
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);

	//adc
	init.GPIO_Mode=GPIO_Mode_AN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_0; //ADC123_IN3 ,IN3 = channel 3 demek
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);

}

void adc_conf(){

	ADC_InitTypeDef init1;
	ADC_CommonInitTypeDef init2;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	init2.ADC_Mode=ADC_Mode_Independent;
	init2.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInit(&init2);

	//init1.ADC_ContinuousConvMode=ENABLE;
	//init1.ADC_DataAlign=ADC_DataAlign_Right;
	//init1.ADC_NbrOfConversion=1;
	init1.ADC_Resolution=ADC_Resolution_12b;
	//init1.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&init1);
	ADC_Cmd(ADC1,ENABLE);

}

void dac_conf(){

	DAC_InitTypeDef init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

	init.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
	init.DAC_Trigger=DAC_Trigger_None;
	init.DAC_WaveGeneration=DAC_WaveGeneration_None;

	DAC_Init(DAC_Channel_1,&init);
	DAC_Cmd(DAC_Channel_1,ENABLE);
}

void delay(uint32_t tt){
	while(tt--);
}

uint32_t adc_function(){
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while((ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))==RESET);
	return ADC_GetConversionValue(ADC1);
}

int main(void){

	gpio_conf();
	adc_conf();
	dac_conf();

  while (1){



	adc_value=adc_function(); // 12 bit görmüyoruz max 16 biz görüyoruz var bir yanlýþlýk

	DAC_SetChannel1Data(DAC_Align_12b_R,adc_value);
	//delay(1000);
	/*
	 for(i=0;i<4096;i++){
		DAC_SetChannel1Data(DAC_Align_12b_R,i);
		delay(10000);
	}
	*/


  }
}
