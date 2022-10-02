#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_pwr.h"              // Keil::Device:StdPeriph Drivers:PWR


void gpioConfig(){
	
   GPIO_InitTypeDef GPIOInitStructure;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//led
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//button
   
   
   //led
   GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
   GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 ;
   GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_Init(GPIOB,&GPIOInitStructure);
   
   //button
   GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
   GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
   GPIO_Init(GPIOC,&GPIOInitStructure);

}


void extiConfig(){
   EXTI_InitTypeDef EXTIInitStructure;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
   
   
   //external interrupt part
   EXTIInitStructure.EXTI_Line=EXTI_Line6;
   EXTIInitStructure.EXTI_LineCmd=ENABLE;
   EXTIInitStructure.EXTI_Mode=EXTI_Mode_Event; // stop modu kullancagimiz için pwr da ,bu interrupt i belirttik buraya
   EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
   
   EXTI_Init(&EXTIInitStructure);

}

void delay(uint32_t time){
   while(time--);
}

int main(){
	gpioConfig();
	extiConfig();
	
	
	while(1){
		
		PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);
		
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		delay(360000);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		delay(360000);
			
	}
}

/*

kitap sayfa 54,56 felan

The voltage regulator is always enabled after Reset. It works in three different modes
depending on the application modes.
• In Run mode, the regulator supplies full power to the 1.8 V domain (core, memories
and digital peripherals).
• In Stop mode the regulator supplies low-power to the 1.8 V domain, preserving
contents of registers and SRAM
• In Standby Mode, the regulator is powered off. The contents of the registers and SRAM
are lost except for the Standby circuitry and the Backup Domain.


Stop mode (all clocks are stopped)

WFI (Wait for Interrupt) or WFE (Wait for Event), butona basmak bir event dir ve stop modundan cikar led yanar söner 
*/
