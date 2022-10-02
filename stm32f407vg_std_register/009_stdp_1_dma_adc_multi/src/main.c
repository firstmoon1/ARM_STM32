#include "stm32f4xx.h"

#define BufferLength  2

uint16_t ADC1ConvertedValue[BufferLength] = {0, 0};

uint16_t adc_value1;
uint16_t adc_value2;

GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
DMA_InitTypeDef DMA_InitStruct;

void config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ADC_DMA_config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream0); // �nceki tan�mlamalar� resetler ve default de�erleri y�kler.

	DMA_InitStruct.DMA_Channel = DMA_Channel_0; // ADC1
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh; // �ncelik atamas� yap�l�r
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR; // de�erin nereden okundu�unu veren ifadedir ADC_DR adres de�eri
	// vermek zorunday�z.// 0x4001204C olarakta yaz�labilirdi. ADC1 registerinin adresidir.
	// Peripheral base adresi oldu�u gibi memory base adreside var. Bu da de�i�kenimizin adresidir.
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) &ADC1ConvertedValue;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory; // nereden nereye yazd���m�z� belirtmemiz gereken ifadedir.
	DMA_InitStruct.DMA_BufferSize = BufferLength; // kanal say�s�
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // yazca��m�z veri yeri sabit mi de�i�ken mi oldu�unu belirtti�imiz nokta
	// Disable yapt�k yani adres sabit de�i�kenlik yok (okuyaca��m�z register hep ayn� oldu�u i�in)
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // Haf�zaya yazma i�lemi hep yanyana yaz�lacak bu sebeple
	// farkl� noktalarda yaz�laca�� i�in memory adresi s�rekli de�i�ecek diyoruz.
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // okunacak veri 12bit ama 16b demektir bu
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // Yaz�lacak verinin data size 12bit yazcaz o y�zden halfword 16bit
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular; // DMA n�n nas�l �al��aca��n� girece�iz. Devaml� diyoruz.
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable ; // First Input First Output
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; // e�ik de�erini verir. Yar� dolu dedik
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single; // burst patlatmak memeory ve peripheral i�in ayn� olmal�
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //

	DMA_Init(DMA2_Stream0, &DMA_InitStruct);

	DMA_Cmd(DMA2_Stream0, ENABLE);

	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // E�er �ok kanall� DMA okuyacaksak aktif etme
	// miz gerekir. Tek kanal okudu�umuz i�in disable edebiliriz.
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; // iki �rnek aras� bekleme

	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // herhangi bir yerden harici tetik almayacak
	ADC_InitStruct.ADC_ExternalTrigConv = 0; // �evrim teti�i almayacak
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // Sa�a dayal� m� sola dayal�m�
	ADC_InitStruct.ADC_NbrOfConversion = BufferLength;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // i�lemin s�rekli olarak devam etmesini sa�l�yoruz

	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_DMACmd(ADC1, ENABLE);



	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_56Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}

int main(void)
{
 	config();
	ADC_DMA_config();

	ADC_SoftwareStartConv(ADC1);	// ADC1 i�in ADC i�lemini yaz�l�msal olarak ba�latt�k
  while (1)
  {
	  adc_value1 = ADC1ConvertedValue[0];
	  adc_value2 = ADC1ConvertedValue[1];
  }
}
