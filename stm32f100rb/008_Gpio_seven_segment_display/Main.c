

#include "stm32f10x.h"                  

void ebru_seven(){
	
	GPIO_InitTypeDef ceren;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	ceren.GPIO_Mode=GPIO_Mode_Out_PP;
	ceren.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	ceren.GPIO_Pin=GPIO_Pin_7;
	ceren.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&ceren);
	
}

void delay(uint32_t iko){
	while(iko--);
}

int main(){
	ebru_seven();
	int i=0,j=0,arr[8]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7};
	
	while(1){
		for(i=0;i<9;i++){
			if(i==1){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB,arr[1] | arr[2]  );  // a,b
			}else if(i==2){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB, arr[0] | arr[1] | arr[3] | arr[4] | arr[6] );  //a,b,d,e,g
			}else if(i==3){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB, arr[0] | arr[1] | arr[2] | arr[3] | arr[6]); //a,b,c,d,g  == 4 verir
				
			}else if(i==4){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB, arr[0] | arr[5] | arr[6] | arr[2] | arr[3] ); // a,f,g,c,d == 5 verir
			}else if(i==5){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB, arr[0] | arr[5] | arr[6] | arr[4] | arr[3]); // a,f,g,e,d == 6
			}else if(i==6){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB, arr[0] | arr[1] | arr[2] ); // a,b,c == 7 verir
			}else if(i==7){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB, arr[0] | arr[1] | arr[2] | arr[3] | arr[4] | arr[5] | arr[6] );  // hepsi 8 verir
			}else if(i==8){
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
				GPIO_SetBits(GPIOB, arr[0] | arr[1] | arr[2] | arr[3] | arr[5] | arr[6] );  // 9 verir
			}else{
				for(j=0;j<8;j++){
					GPIO_ResetBits(GPIOB,arr[j]);
				}
			}
			delay(360000);
		}
		
		for(;i>=0;i--){
			
		}
		
	}
	
	
}
