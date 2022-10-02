#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC

uint16_t ebru=0,canan=0;
float mapvalue;

void gpioco(){
	
	GPIO_InitTypeDef gpioinit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); // ir sensor
	
	// adc
	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
	gpioinit.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&gpioinit);
}

void adcconf(){
	
	ADC_InitTypeDef adcinit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // adc clock enable
	
	adcinit.ADC_ContinuousConvMode=ENABLE;
	adcinit.ADC_DataAlign=ADC_DataAlign_Right;
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	adcinit.ADC_Mode=ADC_Mode_Independent;
	adcinit.ADC_NbrOfChannel=1;
	adcinit.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&adcinit);
	ADC_Cmd(ADC1,ENABLE);
	
}

uint16_t adcfunction(){
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	
	return ADC_GetConversionValue(ADC1);
}

uint16_t anatodigwithformula(uint16_t sam){
	if(sam>3000){
		return 1;
	}else{
		return 0;
	}
}


int main(){
	gpioco();
	adcconf();
	while(1){
		ebru=adcfunction();
		canan=anatodigwithformula(ebru);
		
		
	}
	
	
}