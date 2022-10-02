#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
uint16_t adcvalue=0;

void gpio_config(){
	
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_4;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&init);

}

void adc_config(){
	
	ADC_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	init.ADC_ContinuousConvMode=ENABLE;
	init.ADC_DataAlign=ADC_DataAlign_Right;
	init.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	init.ADC_Mode=ADC_Mode_Independent;
	init.ADC_NbrOfChannel=1;
	init.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&init);
	ADC_Cmd(ADC1,ENABLE);
	
}

uint16_t adcfunction(){
	// kullandigigin pin adc : PA0 ,dac-led :PA4 
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5); 
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetITStatus(ADC1,ADC_IT_EOC)==RESET);
	
	return ADC_GetConversionValue(ADC1);
}

void dac_config(){
	
	DAC_InitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	init.DAC_LFSRUnmask_TriangleAmplitude=DISABLE;
	init.DAC_OutputBuffer=ENABLE;
	init.DAC_Trigger=DAC_Trigger_None;
	init.DAC_WaveGeneration=DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_1,&init);
  DAC_Cmd(DAC_Channel_1,ENABLE);
	
}

int main(){
	gpio_config();
	adc_config();
	dac_config();
	while(1){
		adcvalue=adcfunction();
		DAC_SetChannel1Data(DAC_Align_12b_R,adcvalue); // bu dogru olan hoca yanlýs yapmýs videoda
		//DAC_SetChannel1Data(DAC_Channel_1,adcvalue); // hoca bunu yapti ama yanlýs bu hoca dedi
	}
	
}