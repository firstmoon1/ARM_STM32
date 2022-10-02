#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include <stdio.h>
#include "delay.h"


uint16_t data=0;
char message[20];

void gpioConfig(void);
void adcConfig(void);
uint16_t readADC(void);
void delay(uint32_t time);
void uartConfig(void);
void uartTransmit(char *string);

void gpioConfig(){
   GPIO_InitTypeDef init;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
   
	
	// adc
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_0;  // PA0 adc için
	GPIO_Init(GPIOA,&init);
   
   
   	//TX->PA9 AND RX->PA10
   
   	// Tx
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_9; // PA9 Tx için
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	// Rx
	init.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	init.GPIO_Pin=GPIO_Pin_10; // PA10 Rx için
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
}


void adcConfig(){
   ADC_InitTypeDef ADCInitStructure;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
   
   ADCInitStructure.ADC_ContinuousConvMode=ENABLE;
   ADCInitStructure.ADC_DataAlign= ADC_DataAlign_Right;
   ADCInitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
   ADCInitStructure.ADC_Mode=ADC_Mode_Independent;
   ADCInitStructure.ADC_NbrOfChannel=1;
   ADCInitStructure.ADC_ScanConvMode=DISABLE;
   
   ADC_Init(ADC1,&ADCInitStructure);
   ADC_Cmd(ADC1,ENABLE);
}

void uartConfig(){
   USART_InitTypeDef init;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
   // Tx config == bu eleman datalari gönderiyor ,Rx ise datalari aliyor ,file konusundan fwrite burda Rx dir,fprintf ise Tx dir. termit ile kullanilir, veri gönderilen alinan yer için
   init.USART_BaudRate=9600;
   init.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // voltaj controlu yapmayacagiz, word dosyasinda 4 üncü sayfa
   init.USART_Mode=USART_Mode_Tx | USART_Mode_Rx ;
   init.USART_Parity=USART_Parity_No; //  word dosyasinda 5 inci sayfada
   init.USART_StopBits=USART_StopBits_1; // 0.5,1,1.5,2 secenekleri var, word dosyasinda yaziyor 1 yada 2 degeri alir 
   init.USART_WordLength=USART_WordLength_8b; // 8 veya 9 bit degerleri alir ,b nin olayi bit demek
	
   USART_Init(USART1,&init);
   USART_Cmd(USART1,ENABLE);
}

uint16_t readADC(){
   
   ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
   
   ADC_SoftwareStartConvCmd(ADC1,ENABLE);
   
   while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
   
   return ADC_GetConversionValue(ADC1);
}

void uartTransmit(char *string){
	
   while(*string){
      while(!(USART1->SR & 0x00000040));
      USART_SendData(USART1,*string);
      *string++; // fark etmiyor istersen string++ yap ,pointer adres increment yaptık ,içerik increment degil

   }
}



int main(){
   gpioConfig();
   adcConfig();
   uartConfig();
   DelayInit();
   
   while(1){
      data=readADC();
      sprintf(message,"%d\n",data);// bu sprintf int->string yapıyor aslında char array burda
      uartTransmit(message);
      delay_ms(500);
   }


}

