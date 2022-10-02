
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
	init1.ADC_Prescaler=ADC_Prescaler_Div4; //APB2 clock hattý 84MHz,adc ise max 36MHz okuma yapýyor bunun için 84/4 yaptýk 21 MHz de okusun,adc hýzýný belirledik aslýnda
	ADC_CommonInit(&init1); // adc_init gibi düþün.


	init.ADC_Resolution=ADC_Resolution_12b;// 12bit lik okuma 4095,8bit ise 256 max deger okumasý yapar, cözünürlük artar,
	// bit sayýsý arttýkca ayný zamanda okuma süreside artar cözünürlük artarken , n=bit sayýsý ,((2^n)-1)
	// 8b dedik normalde 256 max görmesi gerekir ama kart mý bozuk anlamadým , 16 bit görüyor sürekli,
	// 12b yaptým max 4095 görmesi gerek gene 65535 16 bit görüyor
	// 6b yaptým  max 256 görüyor anlamadým.büyük ihtimal atollic ile ilgili problem
	/*
	// hoca bunlarý yapmadý ama yapayým ben ,alttakileri hoca yazmadý ben yazdým
	//init.ADC_ContinuousConvMode =ENABLE;
	//init.ADC_DataAlign=ADC_DataAlign_Right;

	//init.ADC_ExternalTrigConv// trigger lýk bir þey yapmadýk
	//init.ADC_ExternalTrigConvEdge

	//init.ADC_NbrOfConversion=1;
	//init.ADC_ScanConvMode=DISABLE;
	*/
	// hoca init ve cmd yazdý
	ADC_Init(ADC2,&init);
	ADC_Cmd(ADC2,ENABLE);

}

uint32_t adc_function(){

	ADC_RegularChannelConfig(ADC2,ADC_Channel_1,1,ADC_SampleTime_56Cycles); // ADC1 de, channel 0 sectik cünkü PA0  ,channel 1 secseydik PA1 de okuma yapardýk
	ADC_SoftwareStartConv(ADC2);
	while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==RESET); //normal adc okumasý için getflagstatus,adc'yi interrupt þeklinde kullanýrsan getýtstatus kullan
	// ,okuma yapmasý için bir süreye ihtiyac duyar,page 392 ,adc conversion time, okuma bittiginde set yani 1 durumuna gecer flag
	return ADC_GetConversionValue(ADC2); // adc degeri returnledik

}

int main(void){

	gpio_config();
	adc_config();

  while (1){

	  adc_value=adc_function(); // max 4095 alýrýz, cünkü resolution 12b :12 bit yaptýk ,8b secseydik max 256 görürdük potansiyometreden
	  voltage=adc_value * (3.3 / 4095 ); // 12 bit i voltaj a göre cevirme yaptým math iþlemi ,max 4095 görürüz potansiyometreden.d



  }
}
