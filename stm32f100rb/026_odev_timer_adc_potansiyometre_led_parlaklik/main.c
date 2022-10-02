#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

uint16_t adcvalue=0,mapvalue=0,potansiyometre_degeri=0,timpulse=0;

void gpio_config(){
	
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// adc 
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_1; // ADC1_IN1 ,potansiyometre pin orta bacak
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	// timer 
	
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_0; // TIM2_CH1_ETR kullanacagiz ,led'e baglanacak pin
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	// burda amacim sadece GPIOA clock hattini kullanarak bu ödevi yapmak
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


void timer_config(){
	TIM_TimeBaseInitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //PA0 için TIM2_CH1_ETR
	
	init.TIM_ClockDivision=TIM_CKD_DIV1; // hoca istersen 1 yaz dedi ,kitapda TIM_CKD_DIV1 seklindeymis
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=10;
	init.TIM_Prescaler=2399; // 1KHz aliyoruz su an, led'e max bu degeri gönderebiliriz
	init.TIM_RepetitionCounter=0;
	
	
	TIM_TimeBaseInit(TIM2,&init);
	TIM_Cmd(TIM2,ENABLE);
}


float map(float adc,float max,float min,float conmax,float conmin){
	return adc*((conmax-conmin)/(max-min));
}

uint16_t adcfunction(){
	/*
	ADC_Channel_1 : cünkü PA1 kullandik data sheet'de ADC1_IN1 bu var anlami ise channel_1 kullan,IN1 den anlasayisi channel'i söyler  ,PA2 için channel_2 
	rank : 2 yide kafama göre sectim ,1 tane adc kullaniyoruz diye, 
	sample cycle da desiklik olsun diye yoksa 55cycles5 perfonsmans yüksekligi iyidir
	ADC1 : bunu sectim cünkü data sheet de adc2 yok ,sadece pinlerde adc1' ler var hep
	*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_41Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while((ADC_GetITStatus(ADC1,ADC_IT_EOC))==RESET);
	
	return ADC_GetConversionValue(ADC1);
	
}

void pwm_function(uint16_t value){
	
	TIM_OCInitTypeDef init;
	
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable;
	init.TIM_Pulse=value;
	
	TIM_OC1Init(TIM2,&init);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
}


int main(){
	
	gpio_config();
	adc_config();
	timer_config();
	
	while(1){
		
		adcvalue=adcfunction(); // potansiyometreden degeri aldik analog to digital ,adc yani, 0 ile 4095 arasi bir deger ,kart (diagramda,stm32vldiscovery pdf inde sayfa 8de,12bit adc1 yazar)semasinda 12 bit  adc1 yazdigi için
		
		potansiyometre_degeri=(adcvalue/4095)*100; // benim potansiyometreden 0 ile 4095 arasi degeri  0 ile 100 arasindaki bir degere dönüstürmem gerek duty cycle için
		
		timpulse=(((2399+1)*potansiyometre_degeri )/100)-1; // timer word dosyasi sayfa 12 deki formul, simdi 0 ile 100 arasi deger aldim
		
		//mapvalue=map(timpulse,4095,0,180,0);  bunu yapmak ne kadar dogru olur ki gerek varmi ?, direk timpulse gönder 0 ile 2399 arasi degerler alsin led
		// adc 12 bit 4095 olmali,
		pwm_function(timpulse); // pwm signal ,led'e potansiyometreden alinan degeri verdik,parlaklik
		
		
		/* 
		// baska biri direk söyle yapmis,mantikli aslinda ama bize voltaj lazim ,aci degil ,
		//burda 180,0 yazdik ki potansiyometre acisi 0 ile 180 arasinda deger versin diye aslnda 360,0 yapmak gerek var ya
		
		adcvalue=adcfunction(); 
		mapvalue=map(adcvalue,4095,0,2399,0); // bence bu yanlis 
		mapvalue=map(adcvalue,4095,0,2.98,0);  // voltaj lazim bize, eger potansiyometre full deger 4095 alirsak ,voltaj 2.98 olur
		pwm_function(timpulse); 
		*/
	}
}




