#include "stm32f4xx.h"


int count = 0;

void GPIO_Config_160MHz_hocanin()
{
	RCC->CR |= 0x00030000;	// HSEON and HSEONRDY enable
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
	RCC->CIR |= 0x00800000;		// CSS Flag clear

	RCC->AHB1ENR = 0x00000009;	// GPIOA ve GPIOD aktif

	GPIOD->MODER = 0x55000000;	// 12,13,14,15. pins digital output ,page 281 , burda 0x55000000 olayý þu ,01 general purpose output, pinleri 0101 0101 0000 0000 0000 0000 0000 0000 yaptýk
	GPIOD->OTYPER = 0x00000000;	// 12,13,14,15. pins Push pull ,page 281
	GPIOD->OSPEEDR = 0xFF000000; // 12,13,14,15. pins 100MHz ,page 282 , burda 0xFF000000 olayý þu ,PD12,PD13,PD14,PD15 pinleri 1111 1111 0000 0000 0000 0000 0000 0000 yaptýk yaslýnda ,very high speed 100MHz dir
	GPIOD->PUPDR = 0x00000000;	// 12,13,14,15. pins no pull ,page 282

	// BUTON ÝÇÝN CONF YAPMADI CUNKU HEPSI DEFAULT RESET OLARAK AYARLANMIS PULL PUSH , PUSH DOWN LOW SPEED, DEFAULT INPUT
}

/*
void GPIO_Config_168MHz_kendimce(){ // dogru yanlýs bilmiyorum kendimce denedim ilk 001 register clock daki gibi gördüm PLLN,PLLM,PPP yi
	RCC->CR |= 0x00030000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR = 0x00000000; // default olarak hala 1 ler vardý bende 0 yaptým
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4    aslýnda 20       8
	RCC->PLLCFGR |= 0x00002A00;	// Pll N = 168  aslýnda  488     320 // yalan a.q 168 degil 90 bu ,aþagýda anlattým en altta
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2   aslýnda   2 debug da baktýgýmda gördügüm   2



	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;		// CSS Flag clear

	RCC->AHB1ENR = 0x00000009;	// GPIOA ve GPIOD aktif

	GPIOD->MODER = 0x55000000;	// 12,13,14,15. pins digital output
	GPIOD->OTYPER = 0x00000000;	// 12,13,14,15. pins Push pull
	GPIOD->OSPEEDR = 0xFF000000; // 12,13,14,15. pins 100MHz
	GPIOD->PUPDR = 0x00000000;	// 12,13,14,15. pins no pull
}
*/


void delay(uint32_t time)
{
	while(time--);
}

int main(void)
{
	//RCC_DeInit(); // bunu yapmayýnca registerlarda hiç bir þey degiþmiyor ,bunu // yapýnca 160 MHz görüyoruz HSE olarak PLL ile , default mudur auto mudur degiþmiyor hiç bir þey
	GPIO_Config_160MHz_hocanin();
	//GPIO_Config_168MHz_kendimce();
  while (1)
  {
	  if(GPIOA->IDR & 0x00000001)  // GPIOA port is set
	  {
		  while(GPIOA->IDR & 0x00000001); // 0 inci pin için 1 mi  & ledik,PA0   wait during GPIOA port is set,page 283 ,8.4.5 partýnda ,sadece r yazar : read anlamý var. 0 to 15 pin leri kullanabilirsin 16 to 31 e kadar hep reserve
		  delay(1680000);                 // wait

		  count++;
	  }

	  if(count % 2 == 0)
		  GPIOD->ODR = 0x00000000;        // GPIOD port all pins are reset
	  else
		  GPIOD->ODR = 0x0000F000;        // GPIOD port 12,13,14 and 15 pins are set
  }
}






/*
 RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168
  yalan a.q 168 degil 90 bu ,8 bitlik yerde 5A : 0 0101 1010 gelir,  168 ise A8 : 0 1010 1000 ,hatta (168<<6)  yapýnca  A8 gelir hesap makinesinde,
  ama olmadý kodda, bunu 0x0000A800 yapýncada saçma bir þey alýyorum , n7,n5,n3 1 olursa digerleri 0, bize 168 verir ama olmadý,
  bende 0x00002A00 yaptým n7,n5,n3 1 geldi digerleri 0 ,aslýnda 0x00002A00 bize PLL N veriyor
 */



/*

 // öyle deneme yapmýstým 000_denemede ordakileri buraya aldým

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





