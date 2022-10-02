#include "stm32f10x.h"                  // Device header
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC

void gpio_config(){
	
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_AIN; // AIN: analog input girmemizin sebebi noise'i engellemek içindi
	init.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5; // dac'da PA4 channel1 ,PA5 channel 2 dir
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
}

void dac_config(){
	
	DAC_InitTypeDef init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	init.DAC_LFSRUnmask_TriangleAmplitude=DISABLE; // amplitude versin mi, vermesin dedik
	init.DAC_OutputBuffer=ENABLE;//empedans'i düsürmek için enable ettik, buffer olsun mu bir süre sonunda birikmis veriyi versin dedik
	init.DAC_Trigger=DAC_Trigger_None;// trigger ile bir is yapmiyoruz,tetikleme ile dac'yi aktif etmeyecegimiz için none dedik 
	init.DAC_WaveGeneration=DAC_WaveGeneration_None;//wave yani dalga üretsin mi ,bununlada bir is yapmadik
	// burda sadece led'i kara simsek misali 0 -3v  ve 3v->0v a degerleri verdik yanip sönmesini yavasca izledik
	
	DAC_Init(DAC_Channel_1,&init);
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
	DAC_Init(DAC_Channel_2,&init);
	DAC_Cmd(DAC_Channel_2,ENABLE);
		
}

void delay(uint16_t tt){
	while(tt--);
}

int main(){
	gpio_config();
	dac_config();
	int i=0;
	while(1){

		for(i=0;i<=4095;i++){
			// 1 inci yol tek tek yaparsin
			/*
			DAC_SetChannel1Data(DAC_Align_12b_R,i);
			DAC_SetChannel2Data(DAC_Align_12b_R,i);
			*/ 
			// 2 inci yol 1 line de yaparsin, sag tik,go to definition yap
			DAC_SetDualChannelData(DAC_Align_12b_R,i,i);// DAC_SetDualChannelData(uint32_t DAC_Align, uint16_t Data2, uint16_t Data1)
			delay(360000);
		}
		for(i=4095;i>=0;i--){
			//DAC_SetChannel1Data(DAC_Align_12b_R,i); 
			//DAC_SetChannel2Data(DAC_Align_12b_R,i); 
			DAC_SetDualChannelData(DAC_Align_12b_R,i,i);
			delay(360000);
		}

	}		
}

/*
DAC_SetChannel1Data(DAC_Align_8b_R,i) :  dac align_8b_r nin olayi su ,adc de oldugu gibi left or right'a 
yaslayacakti biz saga yaslasin verileri dedik yani the most significant bit sag tarafta olsun gibi bir sey 
ve 8b nin olayi da 8 bit ,yani 2^8 den 256 olur biz 255 olsun dedik asagi yukari 2.99 v gibi birseye denk gelir
DAC_SetChannel1Data : function aciklamasi sag tik definition goto yap bakarsin


*/