
#include "stm32f4xx.h"

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
	init.USART_StopBits=USART_StopBits_1; // 1 bit for stop bit.
	init.USART_WordLength=USART_WordLength_8b; // 8bit for wordlength

	USART_Init(USART2,&init);
	USART_Cmd(USART2,ENABLE);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); // USART_IT_RXNE: Receive Data register not empty interrupt
}

void nvic_conf(){
	NVIC_InitTypeDef init;

	init.NVIC_IRQChannel=USART2_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=1;
	init.NVIC_IRQChannelSubPriority=1;

	NVIC_Init(&init);

}


void usart_send_function(USART_TypeDef *usartx,volatile char *s){

	while(*s){

		while(!(usartx->SR & 0x00000040));
		USART_SendData(usartx,*s);
		s++;

	}
}

// usart receive için function yaptýk yani bir mesaj gönderirsen interrupt yaptýk
void USART2_IRQHandler(){

	if(USART_GetITStatus(USART2,USART_IT_RXNE)){
		//char rx_buffer=USART2->DR;  // register seviyesinde alma
		char rx_buffer=USART_ReceiveData(USART2);
		if(rx_buffer != '\n' && i < 50 ){ // 50 character sýnýrý koyduk.
			rx_buff[i]=rx_buffer;
			i++;
		}else{
			rx_buff[i] = rx_buffer; // son veri \n verisidir. enter e bastýgýmýz için termit'te.
			i=0;
			usart_send_function(USART2,rx_buff);
			// var olanýn üstüne yazmasýn tamamen boþaltmak için yaptýk
			for(int a=0;a<50;a++){
				rx_buff[a]='\0';
			}
		}
	}

}

int main(void){

	gpio_conf();
	usart_conf();
	nvic_conf();

  while (1){

  }
}




/*
 burada sadece bir mesaj gönderme islemi yapalým yani TX kullancaz, mesag almak için RX kullanýlýr
 usart1  PA9 TX ve PA10 RX dir mesela bunlara stm32bubeide den bakýlýr usart larda asynchronous
 usart2  PA2 TX ve PA3 RX dir

elimde ppl olmadýgý için deneyemedim. volatile olsa ne olmasa ne olur.
C's volatile keyword is a qualifier that is applied to a variable when it is declared.
It tells the compiler that the value of the variable may change at any time--without any
action being taken by the code the compiler finds nearby.


USART_IT_RXNE: Receive Data register not empty interrupt




 */
