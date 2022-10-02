#include "stm32f4xx.h"

uint32_t count=0;

void gpio_config(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOD,&init);


}

void timer_config(){

	TIM_TimeBaseInitTypeDef init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	init.TIM_ClockDivision=TIM_CKD_DIV1; // timer clock division bölme, biz bölmeyelim bölme sayýsý 1 olarak kalsýn,system freq 168MHz ise apb1 hattý 42MHz dir ,
	// stm32cubeide den bakarsanda apb1 timer clocks 84MHz dir,DIV1 yaparsan 84MHz olarak oldugu gibi kullanýrsýn,DIV2 yaparsan 84/2 den 42MHz TIM2 olarak kullanýrsýn.
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period= 1999; // sayma islemi period a verdigin sayýya kadar gider,max deger 32 bit,neden tim2 ve tim5 32bit,tim3,tim4,tim9,tim10,11,12,13,14 ise 16 bit genel amaclý timer,
	init.TIM_Prescaler=41999 ;
	init.TIM_RepetitionCounter=0;// 1999 oldugunda bir taþma olur ve baþtan tekrar devam eder saymaya ,burdada repetion'ýn olayý kaç taþma da bir tekrarlasýn ,0 dedik her taþmada tekrarlasýn dedik
	//sayma islemi ile ilgili bir iþ yapar,temel mantýk : kaç defada bir tekrarlanacagýný söyler,mesela period 10 verdin, her sayma sayýsý 10 oldugunda tekrar edicek timer'i

	TIM_TimeBaseInit(TIM2,&init);
	TIM_Cmd(TIM2,ENABLE);

}
uint32_t timer_function(){
	return TIM_GetCounter(TIM2);
}

int main(void){

	gpio_config();
	timer_config();

  while (1){

	  count=timer_function();
	  if(count==1000){
		  GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	  }else if(count==1999){
		  GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	  }
  }


}

/*

 1 sn de bir timer'ýn tekrar etmesini istersen nasýl yaparsýn:

 formul :

 saniye = ((period+1)*(prescaler+1))/clock speed; dir,
 1sn = ((1999+1)*(41999+1))/84*10^6;
 1sn = ((3999+1)*(41999+1))/84*10^6;

 clock speed apb1 hattý 84MHzdir. 84*10^6  , diagramda 142MHz yazýyor ama clok config photosunu actýgýnda apb1 hattýnda apb1 timer clock 84MHz olarak görürsün
 period : 0 dan baþlar period a verdigin sayýya kadar sayar.
 prescaler ise bir ön bölücüdür,aslýnda temel hedef frekansý bulmak ,frekans=1/time formul bu
 o yüzden clock speed'in bölücüdür prescaler, önce prescaler clock speed'i böler,birde period ile böler yukarda formulde
 sonra frekansýn tersi saniye buluruz

prescaler'a verebilecegin degerler :  from 0 to 2^16 dýr


 */



















