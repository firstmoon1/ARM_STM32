#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "delay.h"

void gpioConfig(){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  
	GPIO_InitTypeDef GPIOInitStructure;
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStructure);

}

void fullStep(){
	for(int i=0; i<512; i++){
			
			GPIO_SetBits(GPIOA,GPIO_Pin_0);//in1
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);//in2
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);//in3
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);//in4
			
			delay_ms(2);
		
		}


}

void halfStep(){
	
	for(int i=0; i<512; i++){
			
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2); //   
			
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
		
		}


}

void fullStepRotate(int step){
	
	switch(step){
		
		case 0:
			GPIO_SetBits(GPIOA,GPIO_Pin_0);//in1
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		break;
	  case 1:
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);//in2
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		break;
		case 2:
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);//in3
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		break;
		case 3:
		  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);//in4
		break;
	
	}

}

void setStepper(float angle, int direction, int speed){
	float perAngle = 0.703125; // 360/512=0.703125(her bir adimdaki açisi) ,bolum 98 de anlatiyor 
	int stepNumber = (int) (angle/perAngle);// istenilen açiya gelebilmesi için gereken tur.
	for(int seq=0; seq<=stepNumber; seq++){
		if(direction==0){
			for(int step=0; step<4; step++){//saat yonunde 
				fullStepRotate(step);
				delay_ms(speed);
			}	
		}else if(direction==1){// saat yonunun tersi
			for(int step=3; step>=0; step--){
				fullStepRotate(step);
				delay_ms(speed);
			}
		}
	}
}
int main(){
	
	gpioConfig();
	DelayInit();
	
	while(1){
		setStepper(360,0,2);
		delay_ms(500);	
	}
}

/*
11.25*4=45   // 11.25 videoda var parametre, 4 ise 4 tane pin var 
45*64= 2880
2880/360 =8
8*4=32 ,iç çap'in almasi gereken adim 32 dir 1 tur için anladigim bu

dis carp için 32*64=2048
2048/4=512  ,4 pin var diye, 1 pin'de 512 adim gereklidir  1 tur için



*/




