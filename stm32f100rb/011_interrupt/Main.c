
#include "stm32f10x_exti.h"            // Keil::Device:StdPeriph Drivers:EXTI ,interrupt için
#include "stm32f10x.h"                 // Device header  genel kütüphane c programdaki stdio.h gibi
               
uint16_t cnt=0;
void gpioConfig(){

	GPIO_InitTypeDef GPIOInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	// for led
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); // for button
	// led config
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP; // pull push
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	// buton yeri
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD; // input pull down
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_2;
	// no speed for button
	GPIO_Init(GPIOC,&GPIOInitStructure);
	
}

// interrupt config
void interruptconfig(){
	 
	EXTI_InitTypeDef extistructure;
	NVIC_InitTypeDef nvicstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  // alternative function
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);// 2 parametre alir, port source,pin source, yada araya | koy tek line de bitir isi
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);
	
	// external interrupt config
	extistructure.EXTI_Line=EXTI_Line6 | EXTI_Line2; 
	extistructure.EXTI_LineCmd=ENABLE;
	extistructure.EXTI_Mode=EXTI_Mode_Interrupt;
	extistructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&extistructure);
	
	// nvic part
	// priority kisminda 2 adet buton kullanacagi için  group1 sectik cünkü 0 ve 1 degeri var,misc.h kütüphanesinde priority diye bakarsan görürsün
	// line2 deki butona NVIC_IRQChannelPreemptionPriority=0 dedik  line6 dakine NVIC_IRQChannelPreemptionPriority=1 dedik
  	// böylelikle line2 önceligi var line6 ya göre iki butonada bassan öncelik line2 (PC2) pinine bagli buton fonksiyonu calisir
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// line2 pinine bagli buton interrupt config ,line2 priority önceligi var line 6 ya göre ,0 degerini verdik 
	// en büyük interrupt resetdir oda -3 degeri vardir
	// priority degeri düsük olanin önceligi daha fazladir
	nvicstructure.NVIC_IRQChannel=EXTI2_IRQn;
	nvicstructure.NVIC_IRQChannelCmd=ENABLE;
	nvicstructure.NVIC_IRQChannelPreemptionPriority=0;
	nvicstructure.NVIC_IRQChannelSubPriority=0; 
	NVIC_Init(&nvicstructure);
	
	// line6 pinine bagli buton interrupt config
	nvicstructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	nvicstructure.NVIC_IRQChannelCmd=ENABLE;
	nvicstructure.NVIC_IRQChannelPreemptionPriority=1; // priority belirleme
	nvicstructure.NVIC_IRQChannelSubPriority=0; 
	/*
	subpriority : Where multiple pending exceptions share the same group priority, 
	the sub-priority bit field resolves the priority within a group. 
	This is referred to as the sub-priority within the group. The combination of the group priority 
	and the sub-priority is referred to generally as the priority.
	
	kisaca özet gecelim supriority'i : PC0,PC1 lar senin interrupt larin olsun
	2 tane interrupt varsonra NVIC_PriorityGroup_1 sectin diyelim sonra bunlarin 
	NVIC_IRQChannelPreemptionPriority lerine 0 ve 0 verdin her ikisinede,iste burda hangisinin önce calisacagini subpriority 
	belirler,sayisi kücük olanin onceligi vardir.
	*/
	
	NVIC_Init(&nvicstructure);
	
}
void delay(uint16_t tt){
	while(tt--);
}

// PC6 deki buton intterupt fonksiyon
void EXTI9_5_IRQHandler(){
	
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){
		
		GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
		delay(36000000);
		
	}
	EXTI_ClearITPendingBit(EXTI_Line6); // bu interrupt i bir kez kullandiginda set edilmis olur,bu interrupt i tekrar kullanmak
	// için clear yapariz ki içi tekrar bos olsun 
}

// PC2 deki buton intterupt fonksiyon
void EXTI2_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){
		GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
		delay(36000000);
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}


int main(){	
	gpioConfig();	
	interruptconfig();
	int arr[3]={GPIO_Pin_0 , GPIO_Pin_1 , GPIO_Pin_2},i=0;
	while(1){
		for(int i=0;i<3;i++){
			GPIO_SetBits(GPIOB,arr[i]);
			delay(360000);
			GPIO_ResetBits(GPIOB,arr[i]);
			delay(360000);
		}
		for(int j=1;j<2;j++){
			GPIO_SetBits(GPIOB,arr[j]);
			delay(360000);
			GPIO_ResetBits(GPIOB,arr[j]);
			delay(360000);
		}
	}



	
}













