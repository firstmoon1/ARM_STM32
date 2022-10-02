#include "stm32f10x.h"                  // Device header
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

void gpioConfig(void);
void uartConfig(void);
void uartTransmit(char *string);

static char test[25]="ARIF MANDAL\r\n";
static uint16_t data=0;

void gpioConfig(){
	GPIO_InitTypeDef GPIOInitStructure;
	
	//TX->PA9 AND RX->PA10
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//TX
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_9; // PA9 Rx için
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//RX
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_10; // PA10 Rx için
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//LED
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0; // PB0 led için
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);

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

void uartTransmit(char *string){
	while(*string){
		while(!(USART1->SR & 0x00000040));
		USART_SendData(USART1,*string);
		*string++; // pointer adress increment , * gereksiz

	}
}


int main(){
	gpioConfig();
	uartConfig();
	
	
	while(1){
		//uartTransmit(test); // file konusundaki fscanf gibi verileri dosyaya yazdırıyor, sendata char by char gönderiyor ama biz string göndermek istiyoruz
		data=USART_ReceiveData(USART1); // veri alıyor , file konusundaki fprintf gibi dosyadan verileri cekiyor
		
		if(data=='1'){
			GPIO_SetBits(GPIOB,GPIO_Pin_0);

		}if(data=='0'){
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		}
		
	}




}

