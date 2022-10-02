#include "stm32f10x.h"                  // Device header
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

char arr[50]="ilkay koc\r\n";
void gpio_config(){
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	//TX PA9
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_9;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
}

void usart_config(){
	USART_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	init.USART_BaudRate=9600;
	init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	init.USART_Mode=USART_Mode_Tx;
	init.USART_Parity=USART_Parity_No;
	init.USART_StopBits=USART_StopBits_1;
	init.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&init);
	USART_Cmd(USART1,ENABLE);
}


void usart_ch_to_str_transmit(char *string){
	
	while(*string){
		while( !(USART1->SR & 0x00000040) );
		USART_SendData(USART1,*string);
		string++;
	}
}

int main(){
	
	gpio_config();
	usart_config();
	
	while(1){
		usart_ch_to_str_transmit(arr);
		for(int i=0;i<3600000;i++);
	}
	
}