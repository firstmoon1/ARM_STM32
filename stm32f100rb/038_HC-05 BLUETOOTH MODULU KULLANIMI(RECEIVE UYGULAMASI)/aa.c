#include "stm32f10x.h"                  // Device header
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

char arr[50]="ilkay koc\r\n";
char gelenveri;
void gpio_config(){
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//TX PA9  ,ppl donüstürücüde ,ters iliski var ,TX ->RX , RX->TX e baglanir karttan ppl device'na
	init.GPIO_Mode=GPIO_Mode_AF_PP;
	init.GPIO_Pin=GPIO_Pin_9;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	//RX PA10
	init.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	init.GPIO_Pin=GPIO_Pin_10;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	
	//led
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 ;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&init);
	
}

void usart_config(){
	USART_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	init.USART_BaudRate=9600;
	init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	init.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
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
	 // ders 137 ,bluetooth ile kontrol ,telefondan 0,1,2,3 gönder ledleri yak söndür 
	gpio_config();
	usart_config();
	
	while(1){
		gelenveri=USART_ReceiveData(USART1);
		if(gelenveri=='0'){
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		}else if(gelenveri=='1'){
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		}else if(gelenveri=='2'){
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
		}else if(gelenveri=='3'){
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		}
		
	}
	
}



/*
9600 baud rate ne demek? : bir iletisim baglantisinda verinin ne kadar hizli aktarildigini tanimlar. 
genellikle bist-per-second (bps) ile ölçülür. 
örnegin bir seri port için "9600 baud" denilmisse, 
bu o portun bir saniyede maximum 9600 bit tasiyabilecegi anlamina gelir.

*/