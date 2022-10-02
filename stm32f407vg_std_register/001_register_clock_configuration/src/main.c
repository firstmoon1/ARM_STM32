

// ibne burda farklý yapýyor ama 001_register_led_blink de rcc config i farklý yapýyor amcýk burda bu kod tam dogru degil

#include "stm32f4xx.h"

extern uint32_t SystemCoreClock ;  // frekans osilatörü, HSE varya 168 MHz o iþte  ,system_stm32f4xx.c de line 182 deydi
uint32_t system_clock;

void rcc_config_first_yaptigi(){

	// RCC->CR nedir, CR : clock register demek,full referance manuelde page 161 de
	//  &+ veya += veya /= veya |= bunlarý biliyon += cok kullanýrdýk , &= ise and eþittir, 1 & 1 ise 1 dir gibi düþün ,yada 1|=0 ise 1 dir gibi

	RCC->CR &= 0x00000083;// 8 bit ve full referance manuelde page 161 de, rcc resetledik ,register seviyesi kodlama ile
	//  |= ile biz sisteme ilave yaparýz,önceki ayarlarý degiþtirmeyiz


	RCC->CR |= ~(1<<0);  // HSI OFF ,  aslýnda gerekli degil kapatmak ,sistemsel olarak kendi kapatýyor HSE aktif ettiginde  HSI'yý inactive yaptýk HSE kullancaz diye. sayfa 161 de 0 ýncý bitte
	RCC->CR |= (1<<16);  // HSE ON,  1 degerini 16 bit sola kaydýrdýk ,referans manuelde page 161 de tabloda HSE ON var ya high speed external on anlamýna gelir aktif ettik

	// flag 1 oluncaya kadar yani HSE aktif olana kadar while dongüsünde kalýr
	while( !(RCC->CR &(1<<17)) ); // Wait HSE active , neden kullanýyoruz bunu cünkü HSE anýnda acýlmýyor 6 HSE oscillator clock cycles gecmesi gerek,age 162 de HSERDY bu aslýnda 17 bitte yeri 1 yaptýk bu flag dýr, HSEON kontrol eder high mý low mu diye
	// ~ :  altgr + Ü ile yapýlýyor da nedense atollic de olmadý

	RCC->CR |= (1<<19); // güvenlik biti bir hata cýkarsa sistemi off yapýyor sayfa 162'de


	// PLL M  config edelim
	// ilk PLLM de 5 bit var initial olarak hepsini 0 yapalým  bazýlarý auto 1 oldu hocanýn denemesinde
	RCC->PLLCFGR = 0x00000000; // PLLCFGR Reset , 8 bit var hexadecimal de her bit 4 basamak tutar 8x4 den 32 bit initial olarak hepsini 0 yaptýk alttakilere gerek kalmadý
	RCC->PLLCFGR = (1<<22); // PLL Source HSE ,  0 ise HSI ,1 ise HSE kullanýcaz demektir ,   1 degerini 22 bit sola kaydýrdýk , bitler 0 dan baslar 31 bit te son bulur

	//RCC->CFGR  &= ~(31<<0);  // 31 anlamý 4 bit de 11111 dir  aslýnda most left significant + veya - için unutma  5 bit var ama 4 say
	  // uzun yol
	//RCC->PLLCFGR &= ~(1<<0);  // PLLM0 0  div -> 4
	//RCC->PLLCFGR &= ~(1<<1);  // PLLM1 0  div -> 4
	//RCC->PLLCFGR |= ~(1<<2);  // PLLM2 1  div -> 4
	//RCC->PLLCFGR &= ~(1<<3);  // PLLM3 0  div -> 4
	//RCC->PLLCFGR &= ~(1<<4);  // PLLM4 0  div -> 4
	//RCC->PLLCFGR &= ~(1<<5);  // PLLM5 0  div -> 4


	// kýsa yol
	RCC->PLLCFGR |=(4<<0); // PLL M 4  ,  4 degeri 0100 dýr binary de ve 0 ýncý bitten baslayarak kaydýrýyor
	// PLL N config edelim
	RCC->PLLCFGR |= (168<<6); // PLL N 168 ,  0 1010 1000 aslýnda 6 ýncý bitten sonrasi kendi auto decimal to hexa ,binary ceviriyor
	//RCC->PLLCFGR &= ~(1<<16); // a  // 16 ýncý biti 0 yaptýk tersleyen ile
	//RCC->PLLCFGR &= ~(1<<17); // b    //  a & b degerleri olarak  M = 2 olur
	RCC->CR |= (1<<24);  // PLL ON

	//  page 167 SW var  binary 10 yaptýk cünkü PLL selected as system clock
	RCC->CFGR &= ~(1<<0);
	RCC->CFGR &= ~(1<<1); // system clock is PLL ,yanlýs a.q bu hoca gibi ,altta dogru olana bak

	while(!(RCC->CFGR & (1 << 1))); //select system clock is pll clock , PLL ise devam  ,degil ise while döngüsünde kalýr


}

void rcc_config_second_yaptigi(){  // refereans manuelde sayfa 161  ,hoca bir sonrakinde register led blink te böyle yaptý ,yukardaki yanlýs

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
void rcc_config_third_yaptigi(){  // refereans manuelde sayfa 161  ,hoca bir sonrakinde register led blink te böyle yaptý ,yukardaki yanlýs
	RCC->CR |= 0x00010000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4    aslýnda 20       8
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168  aslýnda  488     320 // yalan a.q 168 degil 90 bu ,aþagýda anlattým en altta
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2   aslýnda   2 debug da baktýgýmda gördügüm   2

	// kendimce aslýnda tek satýrdada yapabiliriz gibime geliyor mesela  RCC->PLLCFGR |= 0x00402A04; // tabi denemedim PLLCFGR için bu
	// hocanýnkine göre aslýnda tek satýrdada yapabiliriz gibime geliyor mesela  RCC->PLLCFGR |= 0x00405A04; // tabi denemedim PLLCFGR için bu

	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2 ,bence bu HSE/8 yaptý bunu temsil ediyor 0000 0000 0000 1000 0000 0000 0000 0000 budur yani
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4 ,APB2 Prescaler = not divided oldu APB2 bos aslýnda ,bence bura yanlýs yaptý hoca , olmasý gereken RCC->CFGR |= 0x00009400;  olmalý ki APB1 Prescaler = 4 binary 101, APB2 Prescaler = 2  binary 100   ,kitapta sayfa 166
	RCC->CIR  |= 0x00080000;		// HSERDY Flag clear
	RCC->CIR  |= 0x00800000;		// CSS Flag clear

	RCC->AHB1ENR = 0x00000008;	// GPIOA ve GPIOD aktif

	GPIOD->MODER = 0x55000000;	// 12,13,14,15. pins digital output ,page 281 , burda 0x55000000 olayý þu ,01 general purpose output, pinleri 0101 0101 0000 0000 0000 0000 0000 0000 yaptýk
	GPIOD->OTYPER = 0x00000000;	// 12,13,14,15. pins Push pull ,page 281
	GPIOD->OSPEEDR = 0xFF000000; // 12,13,14,15. pins 100MHz ,page 282 , burda 0xFF000000 olayý þu ,PD12,PD13,PD14,PD15 pinleri 1111 1111 0000 0000 0000 0000 0000 0000 yaptýk yaslýnda ,very high speed 100MHz dir
	GPIOD->PUPDR = 0x00000000;	// 12,13,14,15. pins no pull ,page 282


}

void rcc_deneme(){  // refereans manuelde sayfa 161  ,deneme yaptým öyle  register larla oynadým

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
	RCC->CFGR = 0x000000A0; // page 168'de HPRE: AHB prescaler, 1010: system clock divided by 8 ,aslýnda olan sey 0000 0000 0000 0000 0000 0000 1010 0000 dir aslýnda



	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000B00;  // PPRE1: APB Low speed prescaler (APB1) , 100: AHB clock divided by 2 ,aslýnda olan sey 0000 0000 0000 0000 0000 1000 0000 0000 dir aslýnda

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000C00; // PPRE1: APB Low speed prescaler (APB1) , 110: AHB clock divided by 8 ,aslýnda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir aslýnda

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000D00; // PPRE1: APB Low speed prescaler (APB1), 110: AHB clock divided by 8 ,aslýnda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir aslýnda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000E00; // PPRE1: APB Low speed prescaler (APB1), 110: AHB clock divided by 8 ,aslýnda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir aslýnda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00000F00; // PPRE1: APB Low speed prescaler (APB1), 110: AHB clock divided by 8 ,aslýnda olan sey 0000 0000 0000 0000 0000 1100 0000 0000 dir aslýnda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00001E00; // aslýnda olan sey 0000 0000 0000 0000 0001 1100 0000 0000 dir aslýnda ,reserved bitler var ne write nede read yapabilirsin

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 0x00002F00; // page 166'dA  PPRE2: APB high-speed prescaler (APB2) ,debug yap buraya kadar gel  stm32f4-RCC->CRGR PPRE2 görecegin þey  0000 0000 0000 0000 0010 1100 0000 0000

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 101<<10;

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 3<<10; // 0101 aslýnda

	RCC->CFGR = 0x00000000;
	RCC->CFGR = 7<<10; // 0111 aslýnda

}


int main(void){

	//system_clock=SystemCoreClock;
	//RCC_DeInit(); // HSIEN PLL OFF
	//SystemCoreClockUpdate(); // 16 MHz yapar HSI da 16MHz dir frequency ,kartta 8 MHz var  cristal osilator
	//system_clock=SystemCoreClock;
	// direk system clock'u 0 dan 168 yaptýk register seviyesi kodlama ile
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

 asagýda  ~ kullanýlýyor bitwise tersleyen demek  1 ise 0 , 0 ise 1  þey gibi ! gibi.

 0 &= ~1 olur sana 0  ,aslýnda ~1 þu dur 0 dýr
 0 &= ~1  olur sana 0
 1 &= ~0  olur sana 1 ,aslýnda ~0 þu dur 1 dýr
 1 &= ~1  olur sana 0

 0 |= ~0  olur sana 1
 0 |= ~1  olur sana 0
 1 |= ~0  olur sana 1
 1 |= ~1  olur sana 1

 */




















/*// deneme öyle

#include "stm32f4xx.h"


void rcc_config(){  // refereans manuelde sayfa 161  ,hoca bir sonrakinde register led blink te böyle yaptý ,yukardaki yanlýs

	// 8 000 000 -> 168 000 000

	RCC->CR &= ~(1<<0); // HSI OFF
	RCC->CR |= (1<<16); // HSE ON
	while( !(RCC->CR & (1<<17)) ); // HSE FLAG SECURITY
	RCC->CR |= (1<<19);  // SECURITY

	//RCC->PLLCFGR = 0x00000000; // 32 BIT ALL ZEROS NOW
	//RCC->PLLCFGR |= (1<<22);  //  1 : HSE oscillator clock selected as PLL and PLLI2S clock entry
	//RCC->PLLCFGR =(4<<0); // PLL M  4
	//RCC->PLLCFGR |= (168<<6); // PLL N 168



  // deneme yaptým
	//RCC->PLLCFGR = 0x00000000;
	//RCC->PLLCFGR |= (168<<6);

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00005A00;// 8 bitlik yerde  0 0101 1010  olur ve decimalde 90 ,hex 5A olur, ,0 ýncý bit ,1 inci bit , ... 8 inci bit

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00000004;

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00000804;

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x0000A004; // 8 bitlik yerde  0 1010 1000  olur ve decimalde 168 ,hex A8 olur, ,0 ýncý bit ,1 inci bit , ... 8 inci bit

	//RCC->PLLCFGR =  0x00000000;
	//RCC->PLLCFGR |= 0x00002A00; // boyle 168 i alýyon PLL N de,a.q iþine, hesap makinesinde  bu 42 ,


	RCC->CR |= ( 1 << 24 );  // PLL ON
	while(!(RCC->CR & ( 1 << 25 ))); // wait PLL active

	RCC->CFGR &= ~(1 << 0); // PLL selected as system clock ,page 167
	RCC->CFGR |= ( 1<< 1); // PLL selected as system clock
	while(!(RCC->CFGR & (1 << 1)));  //select system clock is pll clock ,

}




int main(void){

	RCC_DeInit(); // bu olmadan register seviyesinde kodlama imkansýz , 0x00000000 yapýyorsun ama gercekleþtirmiyor ibne
	rcc_config();

  while (1){

  }

}
*/




















