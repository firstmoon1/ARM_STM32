#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI ,interrupt için
#include "stm32f10x.h"                  // Device header  genel kütüphane c programdaki stdio.h gibi
               

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
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	// no speed for button
	GPIO_Init(GPIOC,&GPIOInitStructure);
	
}

// interrupt config
void interruptconfig(){
	 
	EXTI_InitTypeDef extistructure; // external interrupt class objesi
	NVIC_InitTypeDef nvicstructure; // nvic class objesi,bu nvic bütün interruptlari kontrol eder,öncelik siralarini belirler
	
	//external interrupt part
	// clock lari acalim
	// interruptlarin ayri bir clock hatti yoktur ve alternative function olarak acilir AFIO olarak,mesela
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	//interrupt in hangi line de oldugunu bildiricez simdi
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6); // port bilgisi,pin bilgisi,interrupt'a ait line bilgisini verdik
	// simdi ise structure larin yani objelerin degerlerini girelim
	extistructure.EXTI_Line=EXTI_Line6; // hangi line de oldugunu belirttik
	extistructure.EXTI_LineCmd=ENABLE; // enable or disable degerleri alir,line'in aktif ve inaktif olup olmadigini belirtir
	extistructure.EXTI_Mode=EXTI_Mode_Interrupt; // interrupt olarak kullanacagimizi belirttik çesitleri event ,interrupt ve exit mode 
	// trigger da alcalan,yükselen ve ikisi bir arada modu var,butona bastin ve hala basili tutuyorsun calisti bu rising dir,
	//extistructure.EXTI_Trigger=EXTI_Trigger_Rising;  butona bastigin an calisir fonksiyon
	//extistructure.EXTI_Trigger=EXTI_Trigger_Falling; butona basip sonra  butondan elini cektiginde calisir fonksiyon
	//extistructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling; // 2 defa sayar hem bastiginda hemde cektiginde count olarak
	extistructure.EXTI_Trigger=EXTI_Trigger_Rising;// trigger da cem'in gösterdigi buton un sinyal olarak yükselen mi alcalan olarak mi aktif olacagini belirtiyor
	EXTI_Init(&extistructure);
	
	
	// biz yukarda buton tanitip onu external interrupta cevirdik ama bu interrupt'i kontrol eden nvic dir ,bu sebeple 2 foto attim stm32 klasor una
	// buton pin'ini göre bu exti1 or 9_5 ,sonra fonksiyon ismi degisir nvic de ayarlarini yapmaliyiz
	//NVIC part
	// interrupt gerceklestiginde hangi fonksiyonun cagrilmasi gerektigini belirler
	// bu fonksiyon isimlerini kafamiza göre veremeyiz belli isimleri vardir
	// mesela Irg=EXTI0_IRQn ise  Handler=EXTI0_IRQHandler olmali yani fonksiyon ismi
	// niye 9-5 olani kullandik dersen pin numarasi 6 (PC6) oldugu için eger pin numarasi 0 (PC0) olani kullansaydik EXTI0_IRQn kullanirdik
	nvicstructure.NVIC_IRQChannel=EXTI9_5_IRQn; // yazacagimiz fonksiyonun channel ini belirttik biz butonu PC6 yani 6inci channelda oldugu için 9-5arasi bunu kullandik
	//fonksiyona gitmesi gerektigini söyledik enable yaparak
	nvicstructure.NVIC_IRQChannelCmd=ENABLE;
	//priority degerlerini verelim,1 tane interrupt oldugu için 1 verdik ,eger 1 den fazla interrupt
	// olsaydi bu verilen rakamlar degisirdi,mesela reset butonu priority -3 dür en büyük interruptdir reset,
	// sayi pozitif e gittikce priority degeri azalir	
	nvicstructure.NVIC_IRQChannelPreemptionPriority=1;
	nvicstructure.NVIC_IRQChannelSubPriority=1; 
	// initiliaze edelim.
	NVIC_Init(&nvicstructure);
	
}

void delay(uint16_t tt){
	while(tt--);
}

// bu özel bir isimdir ,ebru felan yapamazdik fonksiyon ismini
//EXTI0_IRQHandler gibi ama biz EXTI9_5_IRQn kullandik
void EXTI9_5_IRQHandler(){
	// bu getitstatus : kullanilan line'i check ediyor eger bu RESET yani kapali degilse kod calissin
	// EXTI_GetITStatus : Checks whether the specified EXTI line is asserted or not.
	// RESET : kapali demek feature gibibir yere tanimlamadik 	
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){
		
		for(int i=0;i<4;i++){
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
			delay(360000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			delay(360000);
		}
	}
	// tekrardan interrupt larin calisabilmesi için bunu yapmaliyiz alttakini
	//  Clears the EXTI's line pending bits.
  //  EXTI_Line: specifies the EXTI lines to clear.
	EXTI_ClearITPendingBit(EXTI_Line6);
	
}


int main(){	
	gpioConfig();	
	interruptconfig();
	int arr[3]={GPIO_Pin_0 , GPIO_Pin_1 , GPIO_Pin_2},i=0;
	while(1){
		for(int i=0;i<3;i++){
			GPIO_SetBits(GPIOB,arr[i]);
			delay(3600000);
			GPIO_ResetBits(GPIOB,arr[i]);
			delay(3600000);
		}
		for(int i=1;i<2;i++){
			GPIO_SetBits(GPIOB,arr[i]);
			delay(3600000);
			GPIO_ResetBits(GPIOB,arr[i]);
			delay(3600000);
		}
	}	
}



















