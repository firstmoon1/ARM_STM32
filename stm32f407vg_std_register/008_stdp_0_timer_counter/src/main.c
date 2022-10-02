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

	init.TIM_ClockDivision=TIM_CKD_DIV1; // timer clock division b�lme, biz b�lmeyelim b�lme say�s� 1 olarak kals�n,system freq 168MHz ise apb1 hatt� 42MHz dir ,
	// stm32cubeide den bakarsanda apb1 timer clocks 84MHz dir,DIV1 yaparsan 84MHz olarak oldugu gibi kullan�rs�n,DIV2 yaparsan 84/2 den 42MHz TIM2 olarak kullan�rs�n.
	init.TIM_CounterMode=TIM_CounterMode_Up;
	init.TIM_Period= 1999; // sayma islemi period a verdigin say�ya kadar gider,max deger 32 bit,neden tim2 ve tim5 32bit,tim3,tim4,tim9,tim10,11,12,13,14 ise 16 bit genel amacl� timer,
	init.TIM_Prescaler=41999 ;
	init.TIM_RepetitionCounter=0;// 1999 oldugunda bir ta�ma olur ve ba�tan tekrar devam eder saymaya ,burdada repetion'�n olay� ka� ta�ma da bir tekrarlas�n ,0 dedik her ta�mada tekrarlas�n dedik
	//sayma islemi ile ilgili bir i� yapar,temel mant�k : ka� defada bir tekrarlanacag�n� s�yler,mesela period 10 verdin, her sayma say�s� 10 oldugunda tekrar edicek timer'i

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

 1 sn de bir timer'�n tekrar etmesini istersen nas�l yapars�n:

 formul :

 saniye = ((period+1)*(prescaler+1))/clock speed; dir,
 1sn = ((1999+1)*(41999+1))/84*10^6;
 1sn = ((3999+1)*(41999+1))/84*10^6;

 clock speed apb1 hatt� 84MHzdir. 84*10^6  , diagramda 142MHz yaz�yor ama clok config photosunu act�g�nda apb1 hatt�nda apb1 timer clock 84MHz olarak g�r�rs�n
 period : 0 dan ba�lar period a verdigin say�ya kadar sayar.
 prescaler ise bir �n b�l�c�d�r,asl�nda temel hedef frekans� bulmak ,frekans=1/time formul bu
 o y�zden clock speed'in b�l�c�d�r prescaler, �nce prescaler clock speed'i b�ler,birde period ile b�ler yukarda formulde
 sonra frekans�n tersi saniye buluruz

prescaler'a verebilecegin degerler :  from 0 to 2^16 d�r


 */



















