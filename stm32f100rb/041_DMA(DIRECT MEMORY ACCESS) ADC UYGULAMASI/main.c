#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA

uint16_t adcvalue[1]; // 1 adet adc okuyacagiz diye,aslinda array yapmayada gerek yok

void gpio_config(){
	
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_0;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&init);
	
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
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE); // Enables or disables the specified ADC DMA request.
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	
}

void dma_config(){
	
	DMA_InitTypeDef init;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	// dma'nin önceki ayarlarini sifirladik ve cmd ile disable yaptik ,sonra ayarlarini yaptik ve cmd enable yaptik
	DMA_Cmd(DMA1_Channel1,DISABLE); // Enables or disables the specified DMAy Channelx.
	DMA_DeInit(DMA1_Channel1); // Deinitializes the DMAy Channelx registers to their default reset values.
	
	init.DMA_BufferSize=1;  //word dosyasinda var,coklu adc kullanimlarinda kaç tane kullancaksan adetini yazarsin.
	init.DMA_DIR=DMA_DIR_PeripheralSRC;//direction peripheral to source(memory ye).
	init.DMA_M2M=DISABLE;  // memory to memory lik bir isimiz yok
	
	init.DMA_MemoryBaseAddr=(uint32_t)adcvalue; // registerlar 32 bitlik tir,bu yüzden 32 bit type cast yaptik
	init.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; //data size dir, word=32 bit,halfword=16 bit,byte=8 bit
	init.DMA_MemoryInc=DMA_MemoryInc_Enable;// her bir okumadan sonra yani hafizaya yazdiktan sonra memorydeki adresi arttirdik
	init.DMA_Mode=DMA_Mode_Circular;//modlar: single or circular, single: 1 kez yapar bitirir tekrar talep gelene kadar ,circular ise sürekli okuma yapar
	
	init.DMA_PeripheralBaseAddr=(uint32_t) & ADC1->DR; // registerlar 32bit olmasi gerek,adc1 data register i buraya tanittik,buraya yazilmasini istiyoruz
	init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; // DMA_MemoryDataSize ile ayni olmasi gerek sikinti cikmasin diye
	init.DMA_PeripheralInc=DMA_PeripheralInc_Disable; // okuma yapacagimiz kismin artmasi yani degismesini istemiyoruz. ayni yerden okuma yapacagiz sürekli
	init.DMA_Priority=DMA_Priority_High;// priority yani coklu adc lerde önemi var. hangisinin önce ise alinacagini secer
	
	DMA_Init(DMA1_Channel1,&init);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	
}

int main(){
	
	gpio_config();
	adc_config();
	dma_config();
	
	while(1){
		
	}
	
}


/*

•	PeripheralBaseAddr: Çevre biriminde verinin adresini tutar. Farkli çevre birimleri farkli adreslere sahiptir.
•	MemoryBaseAddr: Verinin hafiza da yazildigi kisimdir. Hedef adrestir. 
•	DIR (Direction): DMA transfer yönünü tanimlar.
•	PeripheralInc: Çevre biriminde verinin yazilacagi adresi arttirmak için kullanilir. Uygulamalarda genellikle DISABLE olarak kalacaktir.
•	MemoryInc: Çevre biriminden alinan her bir deger için hafizadaki adresi arttirmak için kullanilir.
•	PeripheralDataSize: Kaynaktan alinacak verinin boyutunu tutar.
•	MemoryDataSize: Hafizada gelen verinin saklanacagi boyutu tutar. 
•	Mode: Veri alindiktan sonra islemin sonlanacagini veya tekrar edecegini belirler.
•	Priority: Kanal önceligini belirler.
•	M2M: Hafizadan hafizaya transferin yapilip yapilmayacagini belirtir. Genellikle DISABLE olarak seçilir.
•	BufferSize: Alacagimiz veri sayisini belirtir. Örnek, 3 adet adc kanali okuyacaksak 3 girilmesi gerekir.





*/