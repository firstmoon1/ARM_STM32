#include "stm32f10x.h"                  // Device header
#include "stm32f10x_pwr.h"              // Keil::Device:StdPeriph Drivers:PWR


void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0; // led için actik  standby moduna girdiginde calismayacak ,resete basinda yada PA0, o zaman yanar söner
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void delay(uint32_t time){
	while(time--);


}
int main(){
	gpioConfig();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_WakeUpPinCmd(ENABLE);// PA0-> STANDBY MODU PASIF EDECEK PIN, uykudan uyandirmak için özel tanimlanmis bu karta
	// yada PA0 yerine reset butonuna bassanda olur ,tüm systemi resetler bastan alir kodlari
	// PA0 için bir pin port acmadik 
	
	while(1){
		
		for(int i=0; i<4; i++){// 4 adet blink islemi
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			delay(3600000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			delay(3600000);
		
		}
		PWR_EnterSTANDBYMode();//standby modu aktif
	}
}

/*

kitapta sayfa 58
The Standby mode allows to achieve the lowest power consumption.


The voltage regulator is always enabled after Reset. It works in three different modes
depending on the application modes.
• In Run mode, the regulator supplies full power to the 1.8 V domain (core, memories
and digital peripherals).
• In Stop mode the regulator supplies low-power to the 1.8 V domain, preserving
contents of registers and SRAM
• In Standby Mode, the regulator is powered off. The contents of the registers and SRAM
are lost except for the Standby circuitry and the Backup Domain.


Standby mode (1.8V domain powered-off) , bütün clock'lar kapali demektir

*/


