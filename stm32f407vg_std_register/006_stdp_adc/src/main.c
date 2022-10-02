
#include "stm32f4xx.h"

uint16_t adc_value=0;
float voltage=0;

void gpio_config(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//adc
	init.GPIO_Mode=GPIO_Mode_AN; // GPIO Analog Mode
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_1;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);

}

void adc_config(){

	ADC_InitTypeDef init;
	ADC_CommonInitTypeDef init1;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);

	init1.ADC_Mode=ADC_Mode_Independent;
	init1.ADC_Prescaler=ADC_Prescaler_Div4; //APB2 clock hatt� 84MHz,adc ise max 36MHz okuma yap�yor bunun i�in 84/4 yapt�k 21 MHz de okusun,adc h�z�n� belirledik asl�nda
	ADC_CommonInit(&init1); // adc_init gibi d���n.


	init.ADC_Resolution=ADC_Resolution_12b;// 12bit lik okuma 4095,8bit ise 256 max deger okumas� yapar, c�z�n�rl�k artar,
	// bit say�s� artt�kca ayn� zamanda okuma s�reside artar c�z�n�rl�k artarken , n=bit say�s� ,((2^n)-1)
	// 8b dedik normalde 256 max g�rmesi gerekir ama kart m� bozuk anlamad�m , 16 bit g�r�yor s�rekli,
	// 12b yapt�m max 4095 g�rmesi gerek gene 65535 16 bit g�r�yor
	// 6b yapt�m  max 256 g�r�yor anlamad�m.b�y�k ihtimal atollic ile ilgili problem
	/*
	// hoca bunlar� yapmad� ama yapay�m ben ,alttakileri hoca yazmad� ben yazd�m
	//init.ADC_ContinuousConvMode =ENABLE;
	//init.ADC_DataAlign=ADC_DataAlign_Right;

	//init.ADC_ExternalTrigConv// trigger l�k bir �ey yapmad�k
	//init.ADC_ExternalTrigConvEdge

	//init.ADC_NbrOfConversion=1;
	//init.ADC_ScanConvMode=DISABLE;
	*/
	// hoca init ve cmd yazd�
	ADC_Init(ADC2,&init);
	ADC_Cmd(ADC2,ENABLE);

}

uint32_t adc_function(){

	ADC_RegularChannelConfig(ADC2,ADC_Channel_1,1,ADC_SampleTime_56Cycles); // ADC1 de, channel 0 sectik c�nk� PA0  ,channel 1 secseydik PA1 de okuma yapard�k
	ADC_SoftwareStartConv(ADC2);
	while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==RESET); //normal adc okumas� i�in getflagstatus,adc'yi interrupt �eklinde kullan�rsan get�tstatus kullan
	// ,okuma yapmas� i�in bir s�reye ihtiyac duyar,page 392 ,adc conversion time, okuma bittiginde set yani 1 durumuna gecer flag
	return ADC_GetConversionValue(ADC2); // adc degeri returnledik

}

int main(void){

	gpio_config();
	adc_config();

  while (1){

	  adc_value=adc_function(); // max 4095 al�r�z, c�nk� resolution 12b :12 bit yapt�k ,8b secseydik max 256 g�r�rd�k potansiyometreden
	  voltage=adc_value * (3.3 / 4095 ); // 12 bit i voltaj a g�re cevirme yapt�m math i�lemi ,max 4095 g�r�r�z potansiyometreden.d



  }
}
