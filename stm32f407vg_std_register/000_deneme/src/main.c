#include "stm32f4xx.h"

GPIO_InitTypeDef init0;
TIM_TimeBaseInitTypeDef init1;
uint32_t a_tim2=0,a_tim3=0;
void gpio_fun(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	init0.GPIO_Mode=GPIO_Mode_OUT;
	init0.GPIO_OType=GPIO_OType_PP;
	init0.GPIO_Pin=GPIO_Pin_12;
	init0.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init0.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&init0);


}
void tim_fun(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	init1.TIM_ClockDivision=TIM_CKD_DIV1;
	init1.TIM_CounterMode=TIM_CounterMode_Up;
	init1.TIM_Period=1999;
	init1.TIM_Prescaler=41999;
	init1.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM2,&init1);
	TIM_Cmd(TIM2,ENABLE);

	init1.TIM_ClockDivision=TIM_CKD_DIV4;
	init1.TIM_CounterMode=TIM_CounterMode_Up;
	init1.TIM_Period=1999;
	init1.TIM_Prescaler=41999;
	init1.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM3,&init1);
	TIM_Cmd(TIM3,ENABLE);

}


int main(){
	// multi timer basic counter yaptým öyle calýþtý
	gpio_fun();
	tim_fun();

	while(1){
		a_tim2=TIM_GetCounter(TIM2);
		a_tim3=TIM_GetCounter(TIM3);

	}
}







/*
#include "stm32f4xx.h"

GPIO_InitTypeDef init;
ADC_InitTypeDef init1;
ADC_CommonInitTypeDef init2;
TIM_TimeBaseInitTypeDef init3;
TIM_OCInitTypeDef init4;

uint32_t adc_value=0,cnt_systic=0,map_value=0;

void SysTick_Handler(void)
{
	if(cnt_systic>0)
		cnt_systic--;
}

void delay(uint32_t tt){
	cnt_systic=tt;
	while(tt--);
}


void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//adc
	init.GPIO_Mode=GPIO_Mode_AN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_3; // adc PA3 // ADC123_IN3
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);


	//tim
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
	init.GPIO_Mode=GPIO_Mode_AF;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_2; // timer PA2, TIM2_CH3
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);


}

void adc_conf(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	init2.ADC_Mode=ADC_Mode_Independent;
	init2.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInit(&init2);

	init1.ADC_ContinuousConvMode=ENABLE;
	init1.ADC_DataAlign=ADC_DataAlign_Right;
	init1.ADC_NbrOfConversion=1;
	init1.ADC_Resolution=ADC_Resolution_12b;
	init1.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&init1);
	ADC_Cmd(ADC1,ENABLE);



}

void tim_conf(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); // 84 MHz clock line

	init4.TIM_OCMode=TIM_OCMode_PWM1;
	init4.TIM_OCPolarity=TIM_OCPolarity_High;
	init4.TIM_OutputState=TIM_OutputState_Enable;

	init3.TIM_ClockDivision=TIM_CKD_DIV1;
	init3.TIM_CounterMode=TIM_CounterMode_Up;
	init3.TIM_Period=8399; // arifde 8399 1KHz için, kösede 99 10KHz için, kösede 199 20KHz için
	init3.TIM_Prescaler=10; // arifde 10 , kösede 83 , kösede 20
	init3.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(TIM2,&init3);
	TIM_Cmd(TIM2,ENABLE);

	SysTick_Config(SystemCoreClock/1000); // milisaniye cinsinden gercekci delay olsun diye.


}

void timer_function(uint32_t value){  // tim2 channel3

	init4.TIM_Pulse=value;
	TIM_OC3Init(TIM2,&init4);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);


}


uint32_t adc_function(){

	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while((ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))==RESET );
	return ADC_GetConversionValue(ADC1);

}


float map(float value,float cmax,float cmin,float max,float min){
	return value*((cmax-cmin)/(max-min));
}

int main(){

	gpio_conf();
	adc_conf();
	tim_conf();

	while(1){

		// bc237 transistor ile motor calýstýr felan yaptým
		adc_value=adc_function();
		//map_value=map(adc_value,8399,0,4095,0);
		map_value=map(adc_value,8399,0,4000,0);
		timer_function(map_value);




	}

}
*/
/*
arif mandal'ýn hesabýna göre yaptým
clock line 84MHz ,
frekans hesaplama  1KHz için :
pwmfrekans=(Timerclock)/(timerperiod+1) , 1kHz=1000Hz
1kHz=(8.4*(prescaler=10)/(timerperiod+1) ,prescaler'i isleme dahil etmeyiz ama 84Mhz den calar ,yukarda period ile prescaler ters yazmýsýz .
1kHz=(8.4*10^6)/(8399+1)
biz timerclock'u ve prescaler'i belirledik ,
clock'u 8.4 olarak ayarlamisiz ,biliyon port yolunda 42MHz ama apb1 tim hattý 84Mhz yaziyor ,data sheet port pin diagraminda

init.TIM_Period=8399;
init.TIM_Prescaler=10;

tim pulse hesaplama:
TIMpulse=(((Timerperiod+1)*(DutyCycle))/100)-1
timpulse(1%)=((8400*1)/100)-1 =83
timpulse(25%)=((8400*25)/100)-1 =2099
timpulse(50%)=((8400*50)/100)-1 =4199  , %50 cycle
timpulse(75%)=((8400*75)/100)-1 =6299
timpulse(100%)=((8400*100)/100)-1 =8399


muhammed fatih köse formulleri
 period = (timer_tick_frequency / pwm_frequency)-1
 pwm_frequency = timer_tick_frequency/(period+1)
 timer_tick_frequency = timer_clk/(prescaler+1)
 pulse = [(period+1)*(duty cycle)]-1


 timer clock==Timer_CLK=84MHz == 84*10^6

mesela elimizde bir motor var ve 100KHz ile calýþmasý gerek,o zaman pwm frequency 100KHz olmalý

 pwm_frequency =10000 olmasý için,sanýrým 10KHz
 timer_tick_frequency=840000000/(prescaler+1) , prescaler 83 olsun
 timer_tick_frequency = 1000000 olur
 period=(1000000/10000)-1=99 ,yani period : 99 , bizim max duty cycle sýnýrýmýz,%100 ulaþýlabilecek yer 99


pwm_frequency = 20000 olmasý için ,böyle daha hýzlý oldu, max ckýp min e gidiyor, 20KHz sanýrým
timer_tick_frequency =84*10^6 / (20+1)   ,prescaler = 20
timer_tick_frequency = 4*10^6
period = (4000000/20000)-1  = 200-1  = period  : 199


// bir önceki ornekten kalma degiþtirmedik
PD12 için %100 duty cycle ,pulse :9999 = [(9999+1)*(100/100)]-1
PD13 için %75 duty cycle ,pulse  :7499 = [(9999+1)*(75/100)]-1
PD14 için %50 duty cycle ,pulse  :4999 = [(9999+1)*(50/100)]-1
PD15 için %25 duty cycle ,pulse  :2499 = [(9999+1)*(25/100)]-1
*/




























/*
#include "stm32f4xx.h"

GPIO_InitTypeDef init;
ADC_InitTypeDef init1;
ADC_CommonInitTypeDef init2;
DAC_InitTypeDef init3;

uint32_t adc_value=0;

void gpio_conf(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//adc
	init.GPIO_Mode=GPIO_Mode_AN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_3; // adc PA3 // ADC123_IN3
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);


	//dac
	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_4; // dac PA4 // DAC_OUT1
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);

	//gpio_output_for _transistor
	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_5; //  PA5
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);

}

void adc_conf(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	init2.ADC_Mode=ADC_Mode_Independent;
	init2.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInit(&init2);

	init1.ADC_ContinuousConvMode=ENABLE;
	init1.ADC_DataAlign=ADC_DataAlign_Right;
	init1.ADC_NbrOfConversion=1;
	init1.ADC_Resolution=ADC_Resolution_12b;
	init1.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&init1);
	ADC_Cmd(ADC1,ENABLE);



}

void dac_conf(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

	init3.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
	init3.DAC_Trigger=DAC_Trigger_None;
	init3.DAC_WaveGeneration=DAC_WaveGeneration_None;

	DAC_Init(DAC_Channel_1,&init3);
	DAC_Cmd(DAC_Channel_1,ENABLE);
}

uint32_t adc_function(){

	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while((ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))==RESET );
	return ADC_GetConversionValue(ADC1);

}




int main(){

	gpio_conf();
	adc_conf();
	dac_conf();

	while(1){

		// bc237 transistor ile motor calýstýr felan yaptým
		adc_value=adc_function();
		if(adc_value>3500){
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
		}else{
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		}


		// PA4 ile dac aldýk mosfet ile ile motor hýz denemesi yaptým ama olmadý  mosfet gate bacagý 5v istiyor ama biz dac 12 bit ile 3v alýyoz.3v da calýþmadý mosfet
		DAC_SetChannel1Data(DAC_Align_12b_R,adc_value);

	}

}
*/




/*
#include "stm32f4xx.h"

uint32_t adc_value=0,fun0_value=0,fun1_value=0;

void gpio_conf(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	init.GPIO_Mode=GPIO_Mode_IN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1; // buton
	init.GPIO_PuPd=GPIO_PuPd_DOWN;
	init.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&init);

	init.GPIO_Mode=GPIO_Mode_AN; // adc  PB0
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_0;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOB,&init);

	init.GPIO_Mode=GPIO_Mode_OUT; // LED
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOD,&init);

}

void exti_conf(){

	EXTI_InitTypeDef init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0 | EXTI_PinSource1);


	init.EXTI_Line=EXTI_Line0 | EXTI_Line1;
	init.EXTI_LineCmd=ENABLE;
	init.EXTI_Mode=EXTI_Mode_Interrupt;
	init.EXTI_Trigger=EXTI_Trigger_Rising;

	EXTI_Init(&init);
}

void nvic_conf(){

	NVIC_InitTypeDef init;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	init.NVIC_IRQChannel=EXTI0_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=0;
	init.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&init);

	init.NVIC_IRQChannel=EXTI1_IRQn;
	init.NVIC_IRQChannelCmd=ENABLE;
	init.NVIC_IRQChannelPreemptionPriority=1;
	init.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&init);
}

void delay(uint32_t tt){
	while(tt--);
}

void fun0(){
	fun0_value++;
	GPIO_SetBits(GPIOD,GPIO_Pin_12);
	delay(1680000);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	delay(1680000);
	GPIO_SetBits(GPIOD,GPIO_Pin_12);
	delay(1680000);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	delay(1680000);
	GPIO_SetBits(GPIOD,GPIO_Pin_12);
	delay(1680000);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	delay(1680000);
}
void fun1(){
	fun1_value++;
	GPIO_SetBits(GPIOD,GPIO_Pin_14);
	delay(1680000);
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	delay(1680000);
	GPIO_SetBits(GPIOD,GPIO_Pin_14);
	delay(1680000);
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	delay(1680000);
	GPIO_SetBits(GPIOD,GPIO_Pin_14);
	delay(1680000);
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	delay(1680000);

}


void adc_conf(){

	ADC_InitTypeDef init;
	ADC_CommonInitTypeDef init1;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	init1.ADC_Mode=ADC_Mode_Independent;
	init1.ADC_Prescaler=ADC_Prescaler_Div4;

	init.ADC_ContinuousConvMode=ENABLE;
	init.ADC_DataAlign=ADC_DataAlign_Right;
	init.ADC_NbrOfConversion=1;
	init.ADC_ScanConvMode=DISABLE;
	init.ADC_Resolution=ADC_Resolution_12b;

	ADC_CommonInit(&init1);
	ADC_Init(ADC1,&init);
	ADC_Cmd(ADC1,ENABLE);

}

void EXTI0_IRQHandler(){

	if(EXTI_GetITStatus(EXTI_Line0)!=RESET){
		fun0();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler(){

	if(EXTI_GetITStatus(EXTI_Line1)!=RESET){
		fun1();
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

uint32_t adc_function(){

	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);

}

int main(){

	gpio_conf();
	exti_conf();
	nvic_conf();
	adc_conf();


	while(1){

		adc_value=adc_function();

		if(adc_value<2000){
			GPIO_SetBits(GPIOD,GPIO_Pin_13);
		}else{
			GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		}
	}

}

*/


















/*
#include "stm32f4xx.h"

uint16_t adc_value=0;
float voltage=0;

void gpio_config(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//adc
	init.GPIO_Mode=GPIO_Mode_AN; // GPIO Analog Mode
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_1;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);
	//led PA1
	init.GPIO_Mode=GPIO_Mode_OUT; // led okumasý yapýcaz,general purpose output
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_2;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);



}

void adc_config(){

	ADC_InitTypeDef init;
	ADC_CommonInitTypeDef init1;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	init1.ADC_Mode=ADC_Mode_Independent;
	init1.ADC_Prescaler=ADC_Prescaler_Div4; //APB2 clock hattý 84MHz,adc ise max 36MHz okuma yapýyor bunun için 84/4 yaptýk 21 MHz de okusun,adc hýzýný belirledik aslýnda
	ADC_CommonInit(&init1); // adc_init gibi düþün.

	init.ADC_Resolution=ADC_Resolution_8b;// 12bit lik okuma yaptýk yani 4095,8bit ise 256 max deger okumasý yapar, cözünürlük artar bit sayýsý arttýkca ayný zamanda okuma süreside artar cözünürlük artarken , n=bit sayýsý ,((2^n)-1)


	// hoca bunlarý yapmadý ama yapayým ben ,alttakileri hoca yazmadý ben yazdým
	//init.ADC_ContinuousConvMode =ENABLE;
	//init.ADC_DataAlign=ADC_DataAlign_Right;

	//init.ADC_ExternalTrigConv// trigger lýk bir þey yapmadýk
	//init.ADC_ExternalTrigConvEdge

	//init.ADC_NbrOfConversion=1;
	//init.ADC_ScanConvMode=DISABLE;

	// hoca init ve cmd yazdý
	ADC_Init(ADC1,&init);
	ADC_Cmd(ADC1,ENABLE);

}

uint32_t adc_function(){

	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_56Cycles); // ADC1 de, channel 0 sectik cünkü PA0 da okuma yapýcaz ,channel 1 secseydik PA1 de okuma yapardýk
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET); // okuma yapmasý için bir süreye ihtiyac duyar,page 392 ,adc conversion time, okuma bittiginde set yani 1 durumuna gecer flag
	return ADC_GetConversionValue(ADC1); // adc degeri returnledik

}

int main(void){

	gpio_config();
	adc_config();

  while (1){

	  adc_value=adc_function(); // max 4095 alýrýz, cünkü resolution 12b :12 bit yaptýk ,8b secseydik max 256 görürdük potansiyometreden
	  voltage=adc_value * (3.3 / 4095 ); // 12 bit i voltaj a göre cevirme yaptým math iþlemi ,max 4095 görürüz potansiyometreden.d
	  if(adc_value<2000){
		  GPIO_SetBits(GPIOA,GPIO_Pin_2);
	  }else if(adc_value>2000){
		  GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	  }


  }
}
*/



/*
#include "stm32f4xx.h"

int cnt=0;

void rcc_config(){

	RCC->CR =  0x00000000; // 0000 0000 0000 0000 0000 0000 0000 0000,
	RCC->CR |= 0x010B0000; // 0000 0001 0000 1011 0000 0000 0000 0000,

	RCC->PLLCFGR = 0x00000000; // DUZENLE calýþmadý bu RCC_DeInit(); yapmayýnca calýþmýyor
	//RCC->PLLCFGR &= ~( 4095 << 0 ); // iþe yaramadý bu ilk 16 bit 0 yaptýk
	RCC->PLLCFGR |=0x00402A04;// 0000 0000 0100 0000 0000 1010 1000 0100, PLLN:168,PLLM:4,PLLP:2

	RCC->CFGR =  0x00000000; //  CFGR de olan toplam bu 0000 0000 0000 1000 1001 0100 0000 0010 budur yani , AHB Prescaler = 1 , 0xxx: system clock not divided demek 1 aslýnda
	RCC->CFGR |= 0x00029402; // HSE/2 , APB1 Prescaler = 4 binary 101, APB2 Prescaler = 2  binary 100   ,kitapta sayfa 166

	RCC->CIR = 0x00800000;  //DUZENLE  CSS Flag clear
	//RCC->CIR |= ( 1 << 23 ); // yukardaki ile ayný ama calýþmadý yukardaki
}
void gpio_config(){

	RCC->AHB1ENR =   0x00000009; // GPIOA ve GPIOD portlarý active ettik PAGE 180
	// LED
	GPIOD->MODER  =   0x55000000;   // PD12,PD13,PD14,PD15 pinleri active ettik, LED , page 281
	GPIOD->OTYPER |=  0x00000000;  // RESET STATE ,0: Output push-pull (reset state)
	GPIOD->OSPEEDR |= 0x55000000;  // MEDIUM SPEED
	GPIOD->PUPDR |=   0x00000000; // 00: No pull-up, pull-down page 283

	// BUTON ÝÇÝN CONF YAPMAYA GEREK YOK ASLINDA CUNKU HEPSI DEFAULT RESET OLARAK AYARLANMIS PULL PUSH , PUSH DOWN LOW SPEED, DEFAULT INPUT
	//GPIOA->MODER  =   0x00000000;   // 00 : ÝNPUT RESET STATE, PA0 BUTTON page 281  bu yorumu silince hata alyorum  anlamadým MODER deki olayý
	GPIOA->OTYPER |=  0x00000000;   // RESET STATE ,0: Output push-pull (reset state)
	GPIOA->OSPEEDR |= 0x00000000;   //  00 low speed  , GPIOA->OSPEEDR |= 0x00000003; 11: VERY HIGH SPEED PAGE 282
	GPIOA->PUPDR |=   0x00000002;   // 10: Pull-down page 283


}

void delay(uint32_t tt){
	while(tt--);
}

int main(void){

	//RCC_DeInit();
	rcc_config();
	gpio_config(); // bunda bokluk var düzelt

  while (1){

	  if(GPIOA->IDR & 0x00000001){  // GPIOA port is set
		  while(GPIOA->IDR & 0x00000001);
		  delay(1680000);
		  cnt++;
	  }
	  if(cnt%2==0){
		  GPIOD->ODR = 0x0000F000; // LED YANSIN  0000 F000   ilk 4 bit 0000 olan  reserve sayfa 283 kalan 4 bit  ile ilgileniyoruz yalnýz hex bunlar
	  }else{
		  GPIOD->ODR = 0x00000000; // LED SONSUN
	  }

  }
}
*/
