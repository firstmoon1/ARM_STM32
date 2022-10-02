#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void gpioConfig(){
   
   GPIO_InitTypeDef GPIOInitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//A port clock aktif
	
   GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // alternative funtion push pull
   GPIOInitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1;//tim2 channel1 
   GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
   GPIO_Init(GPIOA, &GPIOInitStructure);

}


void timerConfig(){
   TIM_TimeBaseInitTypeDef TIMERInitStructure;
   
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	 // 20ms -> 50Hz için yaptik, 20ms yani timer interrupt time 
   // timer period time ,eger prescaler 10 secseydik formule göre, genel olarak asil formul sayfa 12  timer word dosyasinda basliyor
   TIMERInitStructure.TIM_Prescaler = 100; // timer word dosyasinda sayfa 16 da formul
   TIMERInitStructure.TIM_Period = 4799; // 16 bit 65535 i geçmemeli sayilar, bu sebeple prescaler 100 olsun dedik
   TIMERInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIMERInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM2, &TIMERInitStructure);
   TIM_Cmd(TIM2, ENABLE);

}

void pwmConfig(uint16_t timPulse){
	
   TIM_OCInitTypeDef TIM_OCInitStruct;

   TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStruct.TIM_Pulse = timPulse;
   TIM_OC1Init(TIM2, &TIM_OCInitStruct);
   TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

}

void pwm1Config(uint16_t timPulse){
   TIM_OCInitTypeDef TIM_OCInitStruct;

   TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
   TIM_OCInitStruct.TIM_Pulse = timPulse;
   TIM_OC2Init(TIM2, &TIM_OCInitStruct);
   TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	// OC1,OC2,OC3,OC4 , 4 tane kanal var secebilecegin
	// stm32vldiscovery pdf inde sayfa 14 de PA0,PA1 için alternate function da tim2_ch1,tim2_ch2 yaziyor
	// yani kafamiza göre channel ve tim1 veya tim2 secmiyoruz
	// mesela PB0 için tim3_ch3 olur sayfa 15 de
}
void delay(uint32_t time){
   
   while(time--);


}

int main(){
   gpioConfig();
   timerConfig();

   
   
   while(1){
		 
		 // bu 359,480,240 lar timer word dosyasi sayfa 17 deki calculationlardan gelir servo motor aci belirtmek için hesapladik
		 // 1 INCI STEP MOTOR
       pwmConfig(359);//0 derece
       delay(7200000);
       pwmConfig(480);//90 derece
       delay(7200000);
       pwmConfig(240);//180 derece
       delay(7200000);
		 // 2 INCI STEP MOTOR
		   pwm1Config(359);//0 derece
       delay(7200000);
       pwm1Config(480);//90 derece
       delay(7200000);
       pwm1Config(240);//180 derece
       delay(7200000);
   }

}


/*
formuller sayfa 10 da baslar ,buradaki formul  sayfa 12 ile 16 ya bak
f=1/t 
f= 1/(20*10^-3) -> 50Hz
// 20 milisaniye hedefledik ,servo motorlar 20ms de bir gönderim yapar. 20 ms içinde yüksek yada alcak sinyal gönderirler mesela uydurdum bunu %60 yüksek gönderir ise degree 80 olur servo motor acýsý mesela
50Hz= (timerclock*(Prescaler=100))/(timeperiod+1)
50Hz= ((0.24*10^4))/(4799+1) dir
period = 4799
prescaler=100


*/




















