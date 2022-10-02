#include "stm32f10x.h"                  // Device header
#include "stm32f10x_pwr.h"              // Keil::Device:StdPeriph Drivers:PWR
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_bkp.h"              // Keil::Device:StdPeriph Drivers:BKP
#include "stdio.h"

char buffer[80];
uint16_t counter=0;
uint16_t backup_counter = 0;

void gpioConfig(){
   
   GPIO_InitTypeDef GPIOInitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   
   //TX->PA9 AND RX->PA10
   
	//TX
   GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
   GPIOInitStructure.GPIO_Pin=GPIO_Pin_9;
   GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_Init(GPIOA,&GPIOInitStructure);

}

void usartConfig(){
   USART_InitTypeDef USARTInitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
   
   
   USARTInitStructure.USART_BaudRate=9600;
   USARTInitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
   USARTInitStructure.USART_Mode=USART_Mode_Tx;
   USARTInitStructure.USART_Parity=USART_Parity_No;
   USARTInitStructure.USART_StopBits=USART_StopBits_1;
   USARTInitStructure.USART_WordLength=USART_WordLength_8b;
   
   USART_Init(USART1,&USARTInitStructure);
   USART_Cmd(USART1,ENABLE);
   
}

void usartTranmit(char *string){
   
   while(*string){
      while(!(USART1 ->SR & 0x00000040));
      USART_SendData(USART1,*string);
      string++;
   }
}

void delay(uint32_t time){
   while(time--);


}


int main(){
   gpioConfig();
   usartConfig();

   
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
   
   PWR_BackupAccessCmd(ENABLE);
   
   while(1){
   backup_counter=BKP_ReadBackupRegister(BKP_DR1);
   backup_counter++;
   BKP_WriteBackupRegister(BKP_DR1,backup_counter);
   counter++;
   sprintf(buffer,"NM_DR: %d ||| BKP_DR1: %d\r\n",counter,backup_counter);
   usartTranmit(buffer);
   delay(3600000);

   }


}

/*
BKP_DRx = bu kart için 42 tane register vardir,her registerda 16 bit veri gönderilebilir


*/

