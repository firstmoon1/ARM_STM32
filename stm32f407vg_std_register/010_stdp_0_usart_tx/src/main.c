
#include "stm32f4xx.h"
#include "stdio.h"

GPIO_InitTypeDef init0;
USART_InitTypeDef init1;

char str[100];

void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

	init0.GPIO_Mode=GPIO_Mode_AF;
	init0.GPIO_OType=GPIO_OType_PP;
	init0.GPIO_Pin=GPIO_Pin_2;
	init0.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init0.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&init0);
}

void usart_conf(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	init1.USART_BaudRate=9600;
	init1.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	init1.USART_Mode=USART_Mode_Tx;
	init1.USART_Parity=USART_Parity_No;
	init1.USART_StopBits=USART_StopBits_1;
	init1.USART_WordLength=USART_WordLength_8b;

	USART_Init(USART2,&init1);
	USART_Cmd(USART2,ENABLE);

}

void usart_datasend(USART_TypeDef *usartx,volatile char *s){

	while(*s){
		while(!(usartx->SR & 0x00000040)); // eger data gelirse while den cýkar.  ,usart2 gibi böyle gpioa gibi objeleri typedef ile alýrsýn.
		USART_SendData(usartx,*s);
		s++;

	}
}

int main(void){

	gpio_conf();
	usart_conf();

  while (1){

	  sprintf(str,"hellow word.\n");
	  usart_datasend(USART2,str);

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
