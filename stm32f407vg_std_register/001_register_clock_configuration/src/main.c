

// ibne burda farkl� yap�yor ama 001_register_led_blink de rcc config i farkl� yap�yor amc�k burda bu kod tam dogru degil

#include "stm32f4xx.h"

extern uint32_t SystemCoreClock ;  // frekans osilat�r�, HSE varya 168 MHz o i�te  ,system_stm32f4xx.c de line 182 deydi
uint32_t system_clock;

void rcc_config_first_yaptigi(){

	// RCC->CR nedir, CR : clock register demek,full referance manuelde page 161 de
	//  &+ veya += veya /= veya |= bunlar� biliyon += cok kullan�rd�k , &= ise and e�ittir, 1 & 1 ise 1 dir gibi d���n ,yada 1|=0 ise 1 dir gibi

	RCC->CR &= 0x00000083;// 8 bit ve full referance manuelde page 161 de, rcc resetledik ,register seviyesi kodlama ile
	//  |= ile biz sisteme ilave yapar�z,�nceki ayarlar� degi�tirmeyiz


	RCC->CR |= ~(1<<0);  // HSI OFF ,  asl�nda gerekli degil kapatmak ,sistemsel olarak kendi kapat�yor HSE aktif ettiginde  HSI'y� inactive yapt�k HSE kullancaz diye. sayfa 161 de 0 �nc� bitte
	RCC->CR |= (1<<16);  // HSE ON,  1 degerini 16 bit sola kayd�rd�k ,referans manuelde page 161 de tabloda HSE ON var ya high speed external on anlam�na gelir aktif ettik

	// flag 1 oluncaya kadar yani HSE aktif olana kadar while dong�s�nde kal�r
	while( !(RCC->CR &(1<<17)) ); // Wait HSE active , neden kullan�yoruz bunu c�nk� HSE an�nda ac�lm�yor 6 HSE oscillator clock cycles gecmesi gerek,age 162 de HSERDY bu asl�nda 17 bitte yeri 1 yapt�k bu flag d�r, HSEON kontrol eder high m� low mu diye
	// ~ :  altgr + � ile yap�l�yor da nedense atollic de olmad�

	RCC->CR |= (1<<19); // g�venlik biti bir hata c�karsa sistemi off yap�yor sayfa 162'de


	// PLL M  config edelim
	// ilk PLLM de 5 bit var initial olarak hepsini 0 yapal�m  baz�lar� auto 1 oldu hocan�n denemesinde
	RCC->PLLCFGR = 0x00000000; // PLLCFGR Reset , 8 bit var hexadecimal de her bit 4 basamak tutar 8x4 den 32 bit initial olarak hepsini 0 yapt�k alttakilere gerek kalmad�
	RCC->PLLCFGR = (1<<22); // PLL Source HSE ,  0 ise HSI ,1 ise HSE kullan�caz demektir ,   1 degerini 22 bit sola kayd�rd�k , bitler 0 dan baslar 31 bit te son bulur

	//RCC->CFGR  &= ~(31<<0);  // 31 anlam� 4 bit de 11111 dir  asl�nda most left significant + veya - i�in unutma  5 bit var ama 4 say
	  // uzun yol
	//RCC->PLLCFGR &= ~(1<<0);  // PLLM0 0  div -> 4
	//RCC->PLLCFGR &= ~(1<<1);  // PLLM1 0  div -> 4
	//RCC->PLLCFGR |= ~(1<<2);  // PLLM2 1  div -> 4
	//RCC->PLLCFGR &= ~(1<<3);  // PLLM3 0  div -> 4
	//RCC->PLLCFGR &= ~(1<<4);  // PLLM4 0  div -> 4
	//RCC->PLLCFGR &= ~(1<<5);  // PLLM5 0  div -> 4


	// k�sa yol
	RCC->PLLCFGR |=(4<<0); // PLL M 4  ,  4 degeri 0100 d�r binary de ve 0 �nc� bitten baslayarak kayd�r�yor
	// PLL N config edelim
	RCC->PLLCFGR |= (168<<6); // PLL N 168 ,  0 1010 1000 asl�nda 6 �nc� bitten sonrasi kendi auto decimal to hexa ,binary ceviriyor
	//RCC->PLLCFGR &= ~(1<<16); // a  // 16 �nc� biti 0 yapt�k tersleyen ile
	//RCC->PLLCFGR &= ~(1<<17); // b    //  a & b degerleri olarak  M = 2 olur
	RCC->CR |= (1<<24);  // PLL ON

	//  page 167 SW var  binary 10 yapt�k c�nk� PLL selected as system clock
	RCC->CFGR &= ~(1<<0);
	RCC->CFGR &= ~(1<<1); // system clock is PLL ,yanl�s a.q bu hoca gibi ,altta dogru olana bak

	while(!(RCC->CFGR & (1 << 1))); //select system clock is pll clock , PLL ise devam  ,degil ise while d�ng�s�nde kal�r


}

void rcc_config_second_yaptigi(){  // refereans manuelde sayfa 161  ,hoca bir sonrakinde register led blink te b�yle yapt� ,yukardaki yanl�s

	// 8 000 000 -> 168 000 000

	RCC->CR &= ~(1<<0); // HSI OFF
	RCC->CR |= (1<<16); // HSE ON
	while( !(RCC->CR & (1<<17)) ); // HSE FLAG SECURITY
	RCC->CR |= (1<<19);  // SECURITY
	RCC->PLLCFGR = 0x00000000; // 32 BIT ALL ZEROS NOW
	RCC->PLLCFGR |= (1<<22);  //  1 : HSE oscillator clock selected as PLL and PLLI2S clock entry
	RCC->PLLCFGR |=(4<<0); // PLL M  4
	RCC->PLLCFGR |= (168<<6); // PLL N 168

	RCC->CR |= ( 1 << 24 );  // PLL ON
	while(!(RCC->CR & ( 1 << 25 ))); // wait PLL active

	RCC->CFGR &= ~(1 << 0); // PLL selected as system clock ,page 167
	RCC->CFGR |=  (1 << 1); // PLL selected as system clock
	while(!(RCC->CFGR & (1 << 1)));  //select system clock is pll clock ,

}

// video 26.
void rcc_config_third_yaptigi(){  // refereans manuelde sayfa 161  ,hoca bir sonrakinde register led blink te b�yle yapt� ,yukardaki yanl�s
	RCC->CR |= 0x00010000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE se�tik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4    asl�nda 20       8
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168  asl�nda  488     320 // yalan a.q 168 degil 90 bu ,a�ag�da anlatt�m en altta
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2   asl�nda   2 debug da bakt�g�mda g�rd�g�m   2

	// kendimce asl�nda tek sat�rdada yapabiliriz gibime geliyor mesela  RCC->PLLCFGR |= 0x00402A04; // tabi denemedim PLLCFGR i�in bu
	// hocan�nkine g�re asl�nda tek sat�rdada yapabiliriz gibime geliyor mesela  RCC->PLLCFGR |= 0x00405A04; // tabi denemedim PLLCFGR i�in bu

	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2 ,bence bu HSE/8 yapt� bunu temsil ediyor 0000 0000 0000 1000 0000 0000 0000 0000 budur yani
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4 ,APB2 Prescaler = not divided oldu APB2 bos asl�nda ,bence bura yanl�s yapt� hoca , olmas� gereken RCC->CFGR |= 0x00009400;  olmal� ki APB1 Prescaler = 4 binary 101, APB2 Prescaler = 2  binary 100   ,kitapta sayfa 166
	RCC->CIR  |= 0x00080000;		// HSERDY Flag clear
	RCC->CIR  |= 0x00800000;		// CSS Flag clear

	RCC->AHB1ENR = 0x00000008;	// GPIOA ve GPIOD aktif

	GPIOD->MODER = 0x55000000;	// 12,13,14,15. pins digital output ,page 281 , burda 0x55000000 olay� �u ,01 general purpose output, pinleri 0101 0101 0000 0000 0000 0000 0000 0000 yapt�k
	GPIOD->OTYPER = 0x00000000;	// 12,13,14,15. pins Push pull ,page 281
	GPIOD->OSPEEDR = 0xFF000000; // 12,13,14,15. pins 100MHz ,page 282 , burda 0xFF000000 olay� �u ,PD12,PD13,PD14,PD15 pinleri 1111 1111 0000 0000 0000 0000 0000 0000 yapt�k yasl�nda ,very high speed 100MHz dir
	GPIOD->PUPDR = 0x00000000;	// 12,13,14,15. pins no pull ,page 282


}

void rcc_deneme(){  // refereans manuelde sayfa 161  ,deneme yapt�m �yle  register larla oynad�m

	// 8 000 000 -> 168 000 000


	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000001;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000002;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000003;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000004;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 1<<3;



	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000010;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000020;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000030;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x000000A0; // page 168'de HPRE: AHB prescaler, 1010: system clock divided by 8 ,asl�nda olan sey 0000 0000 0000 0000 0000 0000 1010 0000 dir asl�nda



	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000B00;  // PPRE1: APB Low speed prescaler (APB1) , 100: AHB clock divided by 2 ,asl�nda olan sey 0000 0000 0000 0000 0000 1000 0000 0000 dir asl�nda

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000C00; // PPRE1: APB Low speed prescaler (APB1) , 110: AHB clock divided by 8 ,asl�nda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir asl�nda

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000D00; // PPRE1: APB Low speed prescaler (APB1), 110: AHB clock divided by 8 ,asl�nda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir asl�nda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000E00; // PPRE1: APB Low speed prescaler (APB1), 110: AHB clock divided by 8 ,asl�nda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir asl�nda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000F00; // PPRE1: APB Low speed prescaler (APB1), 110: AHB clock divided by 8 ,asl�nda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir asl�nda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00001E00; // asl�nda olan sey 0000 0000 0000 0000 0001 1100 0000 0000 dir asl�nda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00002F00; // page 166'dA  PPRE2: APB high-speed prescaler (APB2) ,debug yap buraya kadar gel  stm32f4-RCC->CRGR PPRE2 g�recegin �ey  0000 0000 0000 0000 0010 1100 0000 0000

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 101<<10;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 3<<10; // 0101 asl�nda

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 7<<10; // 0111 asl�nda

}


int main(void){

	//system_clock=SystemCoreClock;
	//RCC_DeInit(); // HSIEN PLL OFF
	//SystemCoreClockUpdate(); // 16 MHz yapar HSI da 16MHz dir frequency ,kartta 8 MHz var  cristal osilator
	//system_clock=SystemCoreClock;
	// direk system clock'u 0 dan 168 yapt�k register seviyesi kodlama ile
	//rcc_config_first_yaptigi();
	//rcc_config_second_yaptigi();
	rcc_config_third_yaptigi();
	//rcc_deneme();

	SystemCoreClockUpdate(); // 168 MHz cevirdik
	system_clock=SystemCoreClock;

  while (1){

  }

}



/*

 0 &= 0  olur sana 0
 0 &= 1  olur sana 0
 1 &= 0  olur sana 0
 1 &= 1  olur sana 1

 0 |= 0  olur sana 0
 0 |= 1  olur sana 1
 1 |= 0  olur sana 1
 1 |= 1  olur sana 1

 asag�da  ~ kullan�l�yor bitwise tersleyen demek  1 ise 0 , 0 ise 1  �ey gibi ! gibi.

 0 &= ~1 olur sana 0  ,asl�nda ~1 �u dur 0 d�r
 0 &= ~1  olur sana 0
 1 &= ~0  olur sana 1 ,asl�nda ~0 �u dur 1 d�r
 1 &= ~1  olur sana 0

 0 |= ~0  olur sana 1
 0 |= ~1  olur sana 0
 1 |= ~0  olur sana 1
 1 |= ~1  olur sana 1

 */




















/*// deneme �yle

#include "stm32f4xx.h"


void rcc_config(){  // refereans manuelde sayfa 161  ,hoca bir sonrakinde register led blink te b�yle yapt� ,yukardaki yanl�s

	// 8 000 000 -> 168 000 000

	RCC->CR &= ~(1<<0); // HSI OFF
	RCC->CR |= (1<<16); // HSE ON
	while( !(RCC->CR & (1<<17)) ); // HSE FLAG SECURITY
	RCC->CR |= (1<<19);  // SECURITY

	//RCC->PLLCFGR = 0x00000000; // 32 BIT ALL ZEROS NOW
	//RCC->PLLCFGR |= (1<<22);  //  1 : HSE oscillator clock selected as PLL and PLLI2S clock entry
	//RCC->PLLCFGR =(4<<0); // PLL M  4
	//RCC->PLLCFGR |= (168<<6); // PLL N 168



  // deneme yapt�m
	//RCC->PLLCFGR = 0x00000000;
	//RCC->PLLCFGR |= (168<<6);

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00005A00;// 8 bitlik yerde  0 0101 1010  olur ve decimalde 90 ,hex 5A olur, ,0 �nc� bit ,1 inci bit , ... 8 inci bit

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00000004;

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00000804;

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x0000A004; // 8 bitlik yerde  0 1010 1000  olur ve decimalde 168 ,hex A8 olur, ,0 �nc� bit ,1 inci bit , ... 8 inci bit

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00002A00; // boyle 168 i al�yon PLL N de,a.q i�ine, hesap makinesinde  bu 42 ,


	RCC->CR |= ( 1 << 24 );  // PLL ON
	while(!(RCC->CR & ( 1 << 25 ))); // wait PLL active

	RCC->CFGR &= ~(1 << 0); // PLL selected as system clock ,page 167
	RCC->CFGR |= ( 1<< 1); // PLL selected as system clock
	while(!(RCC->CFGR & (1 << 1)));  //select system clock is pll clock ,

}




int main(void){

	RCC_DeInit(); // bu olmadan register seviyesinde kodlama imkans�z , 0x00000000 yap�yorsun ama gercekle�tirmiyor ibne
	rcc_config();

  while (1){

  }

}
*/




















