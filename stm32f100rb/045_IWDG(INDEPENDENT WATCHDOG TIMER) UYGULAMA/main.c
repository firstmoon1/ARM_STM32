#include "stm32f10x.h"                  // Device header
#include "stm32f10x_iwdg.h"             // Keil::Device:StdPeriph Drivers:IWDG


void gpioConfig(){
	GPIO_InitTypeDef GPIOInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
}

void delayUS(uint32_t time){
	uint32_t newTime=time*24; // asagida aciklamasi var neden 24 ile carptigimizin, kisaca time 1 olursa  ,1 micro saniye delay olur
	while(newTime--); // 
}

void IWDGConfig(){
	
	RCC_LSICmd(ENABLE); //  Enables or disables the Internal Low Speed oscillator (LSI).
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_4); // referance manuel kitabinda sayfa 484 de 
	IWDG_SetReload(0xFFF); // referance manuel kitabinda sayfa 484 de  0xFFF olarak tanimlariz ve min süre ile max süre vermis ,400ms dir tabloda yazan max süre
	IWDG_ReloadCounter();
	IWDG_Enable();
	
}
int main(){
	gpioConfig();
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	delayUS(300000); // watchdog toplamda 400ms de bir resetler systemi ,biz 300ms burda harcadik ,kaldi 100ms bunude led setbits için kullandik sonra system resetler.
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	IWDGConfig(); // systemi resetliyor 
	
	while(1){
	}
}


/*

clock hizimiz =24000000   , 24*10^6  aslinda

1 cycle=(1/24*10^6) =0.0000000417 saniye  =41.7 nano saniye =0.0417us (us=micro saniye)

1 cycle       0.0417us
?             1us
------------------------
? =23.98=24 cycle dir

farkli clock hizimiz var ise 24*10^6 yerine onu yazarsin

24 ile carptik mesela time=1 olursa ve 24 ile carparsak 1 microsaniyeyi bulmus oldu gecen süre olarak

*/


