#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

int cnt=0;

void gpio_conf(){
	
	GPIO_InitTypeDef init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_Out_PP;
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&init);
}

void timer_conf(){
	// TIM6  temel zamanlayicilar katagorisindedir, dac larla kullanilir genelde.
	TIM_TimeBaseInitTypeDef init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); // clock'u aktif ettik
	
	init.TIM_ClockDivision=1; //1 olarak kalsin timerda bölme islemi yapmayacagini söyledi
	init.TIM_CounterMode=TIM_CounterMode_Up; // yukari dogru saysin 1 to 65535 e kadar , 16 bit in karsiligi 65535 dir
	init.TIM_Period=3999; // her bir saniyede tekrar period degerimiz kadar sayacaktir. 0 to 3999 yani, bu period ile prescaler 16bit'i gecmesin
	init.TIM_Prescaler=5999; // bu sayma iþleminde en fazla 3999 a kadar sayar sonra tekrar 0 to 3999
	init.TIM_RepetitionCounter=0; // advance timer larda kullanilir,bu advance timer olmadigi için 0 yazdik
	
	TIM_TimeBaseInit(TIM6,&init);// initiliaze ettik
	TIM_Cmd(TIM6,ENABLE); //adc,dac de oldugu için cmd ile timer'i aktif ettik
}


int main(){
	timer_conf();
	gpio_conf();	
	while(1){
		cnt=TIM_GetCounter(TIM6);
		// stm32 klasor timer word dosyasi sayfa 5 de yaziyor
		// 1999 ve 3999 geçmemelidir 16 bit 65535  bu 1 den en fazla 65535 e kadar sayar timer,1999 olunca ledi yak ,3999 olunca söndür dedik
		
		if(cnt==1999){
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
		}else if(cnt==3999){
			GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
		}
		
	}
	
	
}


/*
timer lar max 24MHz de calisir bu kart için gecerlidir. system_stm32f10x.c(startup) file inde  line 153 de yazar
birde stm32vldiscovery pdf inde sayfa 8 de apb1 hattinda 24mhz yazar

formula :
uptade event = ((Timerclock)/( (prescaler+1)*(period+1) )) 
biz prescaler ve period'u belirlerken bu kart için 16 bit için 65535 i geçmemeye calisacagiz 
biz uptade event = 1sn olsun dedik yani 1 den 65535 e kadar sayma islemi 1sn de gerceklessin
eger period degerimiz 16 bit ise o degere kadar sayma süresü 1sn dir.
timerclock=24*10^6 dir yani 24MHz dir MHz=megaherz
1Hz = (24*10^6)/((5999+1)*(3999+1)) olur ,1Hz == 1sn dir unutma
circuit 1 deks formul frekans=1/time
1hz=1/time -> time= 1/1hz -> time=1sn  
1hz=1sn
2hz=0.5sn
3hz=0.33333sn

init.TIM_Period=3999;
init.TIM_Prescaler=5999;


Temel zamanlayicilar : TIM6 and TIM7
Genel amaçli zamanlayicilar : TIM2, TIM3, TIM4, TIM5, TIM12, TIM13,TIM114, TIM15, TIM16 and TIM17
gelismis timers : TIM1



*/
























