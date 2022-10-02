#include "stm32f10x.h" 

void config(void);

void config(){
	
	GPIO_InitTypeDef eyup;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	eyup.GPIO_Mode=GPIO_Mode_Out_PP;
	eyup.GPIO_Pin=GPIO_Pin_0;
	eyup.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&eyup);
	
}

int main(){
	config();
	int i;
	while(1){
		
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		for( i=0;i<360000;i++);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		for( i=0;i<750000;i++);
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		for( i=0;i<21000;i++);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		for( i=0;i<100000;i++);
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		for( i=0;i<80000;i++);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		for( i=0;i<25000;i++);
		
	}
}

/*
	for( i=0;i<360000;i++); // delay icin yaptik


*/
