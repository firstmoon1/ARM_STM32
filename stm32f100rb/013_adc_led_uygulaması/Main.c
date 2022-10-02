#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
uint16_t ebru=0;
float mapvalue;
void gpioco(){
	
	GPIO_InitTypeDef gpioinit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// led
	gpioinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioinit.GPIO_Pin= GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	gpioinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpioinit);
	
	// adc
	gpioinit.GPIO_Mode=GPIO_Mode_AIN;
	gpioinit.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&gpioinit);
}

void adcconf(){
	
	ADC_InitTypeDef adcinit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
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
	while( ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET );
	return ADC_GetConversionValue(ADC1);
}

float map(float adc,float max,float min,float conmax,float conmin){
	return adc*((conmax-conmin)/(max-min));
}


int main(){
	gpioco();
	adcconf();
	while(1){
		ebru=adcfunction();
		mapvalue=map(ebru,180,0,4030,0); // gelen degerleri 0 ile 180 derece araliginda  degerlere dönüstürdük
		if(mapvalue>0 && mapvalue<60 ){
			GPIO_SetBits(GPIOB,GPIO_Pin_0 );
			GPIO_ResetBits(GPIOB,GPIO_Pin_1 | GPIO_Pin_2 );
		}else if(mapvalue>60 && mapvalue<120 ){
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 );
			GPIO_ResetBits(GPIOB,GPIO_Pin_2 );
		}else if(mapvalue>120 && mapvalue<180 ){
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
		}
		
		
	}
	
	
}