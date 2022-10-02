#include "stm32f10x.h"                  // Device header
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

void gpioConfig(void);
void uartConfig(void);
void uartTransmit(char *string);

static char test[25]="ARIF MANDAL\r\n";

void gpioConfig(){
	GPIO_InitTypeDef init;
	
	//TX->PA9 AND RX->PA10
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//TX
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_9;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	

}

void uartConfig(){
	USART_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	init.USART_BaudRate=9600;
	init.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //kullanmadik, voltaj kontrolu gibi seylerde kullanilir ,hardwareleri var word dosyasinda anlatiyor
	init.USART_Mode=USART_Mode_Tx; // Tx ve Rx gördük, Tx veri göndermek için, Rx veri almak için
	init.USART_Parity=USART_Parity_No; // control amacli kullanilir veriler için,
	init.USART_StopBits=USART_StopBits_1; // stop bit 1 veya 2 olabilir ,tabi 0.5,1.5 var , 
	init.USART_WordLength=USART_WordLength_8b; // 8 yada 9 bit alir, 8b = 8bir demek
	
	USART_Init(USART1,&init);
	USART_Cmd(USART1,ENABLE);

}

void uart_function(char *string){
	while(*string){
		while(!(USART1->SR & 0x00000040));
		USART_SendData(USART1,*string); // veriyi gönderdik termit consol'a
		*string++; // pointer address increment, not content increment  , * is unnecessary 
		//string++; //yada direk böyle yaz, pointer cünkü
	}
}


int main(){
	gpioConfig();
	uartConfig();
	
	while(1){
		uart_function(test); 
		
	}
}


