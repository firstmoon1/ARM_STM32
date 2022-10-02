
#include "stm32f4xx.h"
#include "stdio.h"

char rx_buff[50];
uint8_t i=0;

void gpio_conf(){

	GPIO_InitTypeDef init;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	init.GPIO_Mode=GPIO_Mode_AF;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&init);

}


void usart_conf(){

	USART_InitTypeDef init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	init.USART_BaudRate=9600;
	init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	init.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	init.USART_Parity=USART_Parity_No;
	init.USART_StopBits=USART_StopBits_1;
	init.USART_WordLength=USART_WordLength_8b;

	USART_Init(USART2,&init);
	USART_Cmd(USART2,ENABLE);

}



void usart_send_function(USART_TypeDef *usartx,char *s){

	while(*s){

		while(!(usartx->SR & 0x00000040));
		USART_SendData(usartx,*s);
		s++;

	}
}


int main(void){

	gpio_conf();
	usart_conf();
	int i=0;

  while (1){

	  i=USART_ReceiveData(USART2);
	  if(i==1){
		  sprintf(rx_buff,"received is 1.\n");
		  usart_send_function(USART2,rx_buff);
	  }else if(i==2){
		  sprintf(rx_buff,"received is 2.\n");
		  usart_send_function(USART2,rx_buff);
	  }else{
		  sprintf(rx_buff,"received is more than 2.\n");
		  usart_send_function(USART2,rx_buff);
	  }

  }
}




/*
 burada sadece bir mesaj gönderme islemi yapalým yani TX kullancaz, mesag almak için RX kullanýlýr
 usart1  PA9 TX ve PA10 RX dir mesela bunlara stm32bubeide den bakýlýr usart larda asynchronous
 usart2  PA2 TX ve PA3 RX dir


C's volatile keyword is a qualifier that is applied to a variable when it is declared.
It tells the compiler that the value of the variable may change at any time--without any
action being taken by the code the compiler finds nearby.


 */
