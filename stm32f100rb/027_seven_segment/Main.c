#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include <stdlib.h> // random  sayi almak için kullandik
#include "seg7display.h"


//// port definitions
//#define portA GPIOA
//#define portB GPIOB
//#define portC GPIOC

//// pin definitions
//#define PIN0 (uint16_t)GPIO_Pin_0
//#define PIN1 (uint16_t)GPIO_Pin_1
//#define PIN2 (uint16_t)GPIO_Pin_2
//#define PIN3 (uint16_t)GPIO_Pin_3
//#define PIN4 (uint16_t)GPIO_Pin_4
//#define PIN5 (uint16_t)GPIO_Pin_5
//#define PIN6 (uint16_t)GPIO_Pin_6
//#define PIN7 (uint16_t)GPIO_Pin_7
//#define PIN8 (uint16_t)GPIO_Pin_8
//#define PIN9 (uint16_t)GPIO_Pin_9
//#define PIN10 (uint16_t)GPIO_Pin_10
//#define PIN11 (uint16_t)GPIO_Pin_11
//#define PIN12 (uint16_t)GPIO_Pin_12
//#define PIN13 (uint16_t)GPIO_Pin_13
//#define PIN14 (uint16_t)GPIO_Pin_14
//#define PIN15 (uint16_t)GPIO_Pin_15

//#define NUMBERSIZE 10
//#define LETTER 8
//#define CHARACTERSIZE 4 
//#define CLEANDISPLAYSIZE 6

// hexadecimal cinsinden rakamlar,harfler,felan sevensegmentdisplay için 
//uint32_t numberArray[]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7C,0x7,0x7F,0x67};
//uint32_t letterArray[]={0x77,0x7F,0x39,0x3F,0x79,0x71,0x7D,0x76};
//uint32_t characterArray[]={0x40,0x49,0x5C,0x52};
//uint32_t cleanDisplayArray[]={0x1,0x2,0x4,0x8,0x10,0x20};

//void seg7DisplayInit(GPIO_TypeDef* port,uint16_t pin0,
//																				uint16_t pin1,
//																				uint16_t pin2,
//																				uint16_t pin3,
//																				uint16_t pin4,
//																				uint16_t pin5,
//																				uint16_t pin6)
//{
//	if(port==portA){
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	}else if(port==portB){
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	}else if(port==portC){
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//	}else{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE); // stm32f100rb de port d için yeterli pin yok ama genede ekledik
//	}
//	GPIO_InitTypeDef init;
//	init.GPIO_Mode=GPIO_Mode_Out_PP;
//	init.GPIO_Speed=GPIO_Speed_50MHz;
//	init.GPIO_Pin=pin0 | pin1 | pin2 | pin3 | pin4 | pin5 | pin6;
//	GPIO_Init(port,&init);
//}

//void printNumber(GPIO_TypeDef* port,uint16_t number){
//	if(number==0){// ya gpioset gpioreset ile de yapilabilir yada register seviyesindede yapilabilir daha kolay diye register ile yapti
//		port->ODR=numberArray[0]; // ODR: output direct register demek
//	}else if(number==1){
//		port->ODR=numberArray[1]; 
//	}else if(number==2){
//		port->ODR=numberArray[2]; 
//	}else if(number==3){
//		port->ODR=numberArray[3]; 
//	}else if(number==4){
//		port->ODR=numberArray[4]; 
//	}else if(number==5){
//		port->ODR=numberArray[5]; 
//	}else if(number==6){
//		port->ODR=numberArray[6]; 
//	}else if(number==7){
//		port->ODR=numberArray[7]; 
//	}else if(number==8){
//		port->ODR=numberArray[8]; 
//	}else if(number==9){
//		port->ODR=numberArray[9]; 
//	}else{
//		port->ODR=characterArray[0]; // eger 1 to 9 sayi girmez ise bunu cikti versin
//	}
//}

//void delay(uint32_t tt){
//	while(tt--);
//}

//void printLetter(GPIO_TypeDef* port,char letter){
//	if(letter=='A' || letter=='a'){
//		port->ODR=letterArray[0];
//	}else if(letter=='B' || letter=='b'){
//		port->ODR=letterArray[1];
//	}else if(letter=='C' || letter=='c'){
//		port->ODR=letterArray[2];
//	}else if(letter=='D' || letter=='d'){
//		port->ODR=letterArray[3];
//	}else if(letter=='E' || letter=='e'){
//		port->ODR=letterArray[4];
//	}else if(letter=='F' || letter=='f'){
//		port->ODR=letterArray[5];
//	}else if(letter=='G' || letter=='g'){
//		port->ODR=letterArray[6];
//	}else if(letter=='H' || letter=='h'){
//		port->ODR=letterArray[7];
//	}else{
//		port->ODR=characterArray[0];
//	}
//}

//void countForward(GPIO_TypeDef* port,uint32_t time){
//	for(int i=0;i<NUMBERSIZE;i++){
//		port->ODR=numberArray[i];
//		delay(time);
//	}
//}

//void countBackward(GPIO_TypeDef* port,uint32_t time){
//	for(int i=NUMBERSIZE-1;i>=0;i--){
//		port->ODR=numberArray[i];
//		delay(time);
//	}
//}

//void cleanDisplay(GPIO_TypeDef* port){
//	for(int i=0;i<CLEANDISPLAYSIZE;i++){
//		port->ODR=cleanDisplayArray[i];
//		delay(1080000);
//	}
//	
//}

//void randomNumberGenerator(GPIO_TypeDef* port){
//	uint16_t random=rand()%9;
//	printNumber(port,random);
//	delay(3600000);
//	
//}

int main(){
	
//	seg7DisplayInit(portA,PIN0,PIN1,PIN2,PIN3,PIN4,PIN5,PIN6); // yukardaki kodlar ile kütüphenesiz
	seg7DisplayInit(portA,PIN0,PIN1,PIN2,PIN3,PIN4,PIN5,PIN6); // kütüphaneden kullandik
	while(1){
		/*
		printNumber(portA,7);
		delay(3600000);
		printNumber(portA,4);
		delay(3600000);
		printLetter(portA,'A');
		delay(3600000);
		printLetter(portA,'h');
		delay(3600000);
		*/
		
		/*
		countForward(portA,36000);
		countBackward(portA,36000);
		cleanDisplay(portA);
		*/
		
		//randomNumberGenerator(portA);
		
		printNumber(portA,5);
		delay(3600000);
		printLetter(portA,'a');
		delay(3600000);
		cleanDisplay(portA);
		delay(3600000);
		countForward(portA,3600000);
		delay(3600000);
		
		
	}

}

























