#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC

uint16_t omur=0;
float voltage=0,mapvalue=0;

void gpioconf(){
	
	GPIO_InitTypeDef ebru;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	ebru.GPIO_Mode=GPIO_Mode_AIN; // AIN = analog in  adc i�in
	ebru.GPIO_Pin=GPIO_Pin_0; // PA0 pin ini kullandik
	GPIO_Init(GPIOA,&ebru);
	
	
}

void adcconf(){
	
	ADC_InitTypeDef adcinit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // ADC1,ADC2,ADC3 var bu kartta sadece biz channel ADC1 olani kullanicaz
	
	adcinit.ADC_ContinuousConvMode=ENABLE;// continuously veri �l�s�n diye enable yaptik continuous modu
	adcinit.ADC_DataAlign=ADC_DataAlign_Right; // most significant bit'in nereye dayali olacagini belirttik ,sag tarafa dayali olsun
	adcinit.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; // trigger kullanmadik,trigger'li bir seyler yapmicaz
	adcinit.ADC_Mode=ADC_Mode_Independent;
	adcinit.ADC_NbrOfChannel=ADC_Channel_1; // ka� channel kullandigimizi belirtiriz biz 1 tane kullandigimiz i�in 1 yazdik ,1 den fazla adc kullanirsan o kadar yaz
	adcinit.ADC_ScanConvMode=DISABLE; //  multi channel larda kullanilir, digerlerininde taranmasini saglar
	
	ADC_Init(ADC1,&adcinit); //2 parametre alir: channel ve class objesini alir inittype diye gecer
	ADC_Cmd(ADC1,ENABLE); // ADC_Cmd = Enables or disables the specified ADC peripheral.
}
// adc c�z�n�rl�g� 12 bitdir, biz 16 kullansak yeterlidir fonksiyon tipi olarak
// adc fonksiyonu
uint16_t ceylan(){
	//adc nin channel config ayarlari yapacagiz
	/*  4 parametre alir
	parametre 1 : hangi adc oldugunu belirttik kartta ADC1,ADC2,ADC3 var biz ADC1 kullandik
	parametre 2 : hangi channel'i kullandigimizi belirttik, PA0 pinini kullandigimiz i�in channel_0 kullandik
	parametre 3 : rank belirttik ,1 den fazla adc kullandiginda �nceligi belirtir,
	parametre 4 : adc lerde 1 sampling yapilmaz dogrulugunu arttirmak i�in bunun i�inde belli zaman araliginda check eder
	bunada cycle s�resi belirtiriz, cycle s�res� 55 olarak belirttik
	
	*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	// adc yi baslattik bunun ile,parametreleri ,hangi adc oldugu ve enable mi disable mi olacagini belirtiyoruz
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	// ADC_GetFlagStatus : Checks whether the specified ADC flag is set or not.
	// ADC_FLAG_EOC : End of conversion flag
	// reset e esit ise while de d�ns�n,reset degilse return'lesin
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	// ADC_GetConversionValue : Returns the last ADCx conversion result data for regular channel.
	return ADC_GetConversionValue(ADC1);
	
	
}

float map(float ebru,float max,float min,float conmax,float conmin){
	return ebru*((conmax-conmin)/(max-min));
}

int main(){
		gpioconf();
		adcconf();
	
		while(1){
			omur=ceylan(); // adc1 12 bit oldugu i�in 0 to 4095 aras� degerler g�r�rs�n  potansiyometreden
			voltage=omur*(2.98/4095); // bu bize 0 ile 3 volt arasi degerler verir formul bu
			// 2.98 : 3 volt ver potansiyometreye ,multimetre ile �lc gnd ile vcc bacagini ,2.98 g�r�rs�n,erosyondan 
		  	// 4095 : adc'ler 12 bit yani ((2^n)-1) olarak �lc�l�r c�z�n�rl�g�, ((2^12)-1) =4095 dir
			mapvalue=map(omur,4095,0,180,0); // 0-180 aras� ac� g�r�yoruz
		}
		
	
}
