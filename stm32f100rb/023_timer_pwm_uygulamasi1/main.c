#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void gpio_config(){
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_AF_PP; // AF_PP : alternative function pull push or push pull
	init.GPIO_Pin=GPIO_Pin_0;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&init);
}

void timer_config(){
	
	TIM_TimeBaseInitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	init.TIM_ClockDivision=TIM_CKD_DIV1; //  apb1 clock hattýndaki tim2 nin frekansýný bölmek için kullanýlýr,ne kadar geliyorsa oldugu gibi gelsin,DIV2 yaparsan 2 ye bölersin.
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period=2399; // bu pwm frequency and time formulleri ile bulundu ,asagida formul var
	init.TIM_Prescaler=10;
	init.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM2,&init);
	TIM_Cmd(TIM2,ENABLE);
	
}
// bunlarda clock aktif etmemize gerek yoktur
void pwm_config(uint32_t ebru){
	
	TIM_OCInitTypeDef init;
	
	init.TIM_OCMode=TIM_OCMode_PWM1;
	init.TIM_OCPolarity=TIM_OCPolarity_High;
	init.TIM_OutputState=TIM_OutputState_Enable; //
	init.TIM_Pulse=ebru;
	
	TIM_OC1Init(TIM2,&init);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
}

void delay(uint16_t tt){
	while(tt--);
}


int main(){
	
	gpio_config();
	timer_config();
	
	// led parlaklýgýný ayarladýk ,pwm sinyaller ile
	while(1){
		
		pwm_config(599);
		delay(3600000);
		pwm_config(1199);
		delay(3600000);
		pwm_config(1799);
		delay(3600000);
		pwm_config(2399);
		delay(3600000);
	}

}

/*
GPIO_Mode_AF_PP: ilk derslerde söylemis ,pwm gibi kullanimlarda cikis alacaksak bunu kullanacagiz,alternate function push pull ur
led felan yakarkende  GPIO_Mode_Out_PP bunu kullaniyorduk

frekans hesaplama :
pwmfrekans=(Timerclock)/(timerperiod+1) , 1kHz=1000Hz
1kHz=(2.4*(prescaler=10)/(timerperiod+1) ,prescaler'i isleme dahil etmeyiz ama 24Mhz den calar ,yukarda period ile prescaler ters yazmýsýz .
1kHz=(2.4*10^6)/(2399+1)  
biz timerclock'u ve prescaler'i belirledik ,
clock'u 2.4 olarak ayarlamisiz ,biliyon port yolunda 24MHz yaziyor ,data sheet port pin diagraminda

init.TIM_Period=2399; 
init.TIM_Prescaler=10;

tim pulse hesaplama:
TIMpulse=(((Timerperiod+1)*(DutyCycle))/100)-1
timpulse(25%)=((2400*25)/100)-1 =599
timpulse(50%)=((2400*50)/100)-1 =1199  , %50 cycle 
timpulse(75%)=((2400*75)/100)-1 =1799
timpulse(100%)=((2400*100)/100)-1 =2399


TIM_OCMode: cikis karsilastirma modu
TIM_Pulse: cikti tetiklemek için kullanilir ,pwm olusturdugumuz için veriyoruz  
TIM_OutputState: cikis almak için enable ettik
TIM_OCPolarity: high or low olabilir ,
geri kalan yapmadiklarimiz ise advance timerlar içindir ,bu 4  özellik yeterli

TIM_OC1Init : C1 channel 1 demek ,eger channel 2 kullanacaksak TIM_OC2Init yapariz
TIM_OC1PreloadConfig : Enables or disables the TIMx peripheral Preload register on CCR1.

PWM1 ile PWM2 farki :
pwm1 de baslangicta %75 high ,sonra %25 low da baslar 
pwm2 de baslangicta %25 high ,sonra %75 low da baslar ,polarity' ye high diyerek de high dan baslattik
TIM_OCPolarity : buna yukarda high dedik



*/



















