#include "stm32f4xx.h"


int count = 0;

void GPIO_Config_160MHz_hocanin()
{
	RCC->CR |= 0x00030000;	// HSEON and HSEONRDY enable
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
	RCC->CIR |= 0x00800000;		// CSS Flag clear

	RCC->AHB1ENR = 0x00000009;	// GPIOA ve GPIOD aktif

	GPIOD->MODER = 0x55000000;	// 12,13,14,15. pins digital output ,page 281 , burda 0x55000000 olay� �u ,01 general purpose output, pinleri 0101 0101 0000 0000 0000 0000 0000 0000 yapt�k
	GPIOD->OTYPER = 0x00000000;	// 12,13,14,15. pins Push pull ,page 281
	GPIOD->OSPEEDR = 0xFF000000; // 12,13,14,15. pins 100MHz ,page 282 , burda 0xFF000000 olay� �u ,PD12,PD13,PD14,PD15 pinleri 1111 1111 0000 0000 0000 0000 0000 0000 yapt�k yasl�nda ,very high speed 100MHz dir
	GPIOD->PUPDR = 0x00000000;	// 12,13,14,15. pins no pull ,page 282

	// BUTON ���N CONF YAPMADI CUNKU HEPSI DEFAULT RESET OLARAK AYARLANMIS PULL PUSH , PUSH DOWN LOW SPEED, DEFAULT INPUT
}

/*
void GPIO_Config_168MHz_kendimce(){ // dogru yanl�s bilmiyorum kendimce denedim ilk 001 register clock daki gibi g�rd�m PLLN,PLLM,PPP yi
	RCC->CR |= 0x00030000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR = 0x00000000; // default olarak hala 1 ler vard� bende 0 yapt�m
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE se�tik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4    asl�nda 20       8
	RCC->PLLCFGR |= 0x00002A00;	// Pll N = 168  asl�nda  488     320 // yalan a.q 168 degil 90 bu ,a�ag�da anlatt�m en altta
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2   asl�nda   2 debug da bakt�g�mda g�rd�g�m   2



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
	//RCC_DeInit(); // bunu yapmay�nca registerlarda hi� bir �ey degi�miyor ,bunu // yap�nca 160 MHz g�r�yoruz HSE olarak PLL ile , default mudur auto mudur degi�miyor hi� bir �ey
	GPIO_Config_160MHz_hocanin();
	//GPIO_Config_168MHz_kendimce();
  while (1)
  {
	  if(GPIOA->IDR & 0x00000001)  // GPIOA port is set
	  {
		  while(GPIOA->IDR & 0x00000001); // 0 inci pin i�in 1 mi  & ledik,PA0   wait during GPIOA port is set,page 283 ,8.4.5 part�nda ,sadece r yazar : read anlam� var. 0 to 15 pin leri kullanabilirsin 16 to 31 e kadar hep reserve
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
  yalan a.q 168 degil 90 bu ,8 bitlik yerde 5A : 0 0101 1010 gelir,  168 ise A8 : 0 1010 1000 ,hatta (168<<6)  yap�nca  A8 gelir hesap makinesinde,
  ama olmad� kodda, bunu 0x0000A800 yap�ncada sa�ma bir �ey al�yorum , n7,n5,n3 1 olursa digerleri 0, bize 168 verir ama olmad�,
  bende 0x00002A00 yapt�m n7,n5,n3 1 geldi digerleri 0 ,asl�nda 0x00002A00 bize PLL N veriyor
 */



/*

 // �yle deneme yapm�st�m 000_denemede ordakileri buraya ald�m

#include "stm32f4xx.h"

int cnt=0;

void rcc_config(){

	RCC->CR =  0x00000000; // 0000 0000 0000 0000 0000 0000 0000 0000,
	RCC->CR |= 0x010B0000; // 0000 0001 0000 1011 0000 0000 0000 0000,

	RCC->PLLCFGR = 0x00000000; // DUZENLE cal��mad� bu RCC_DeInit(); yapmay�nca cal��m�yor
	//RCC->PLLCFGR &= ~( 4095 << 0 ); // i�e yaramad� bu ilk 16 bit 0 yapt�k
	RCC->PLLCFGR |=0x00402A04;// 0000 0000 0100 0000 0000 1010 1000 0100, PLLN:168,PLLM:4,PLLP:2

	RCC->CFGR =  0x00000000; //  CFGR de olan toplam bu 0000 0000 0000 1000 1001 0100 0000 0010 budur yani , AHB Prescaler = 1 , 0xxx: system clock not divided demek 1 asl�nda
	RCC->CFGR |= 0x00029402; // HSE/2 , APB1 Prescaler = 4 binary 101, APB2 Prescaler = 2  binary 100   ,kitapta sayfa 166

	RCC->CIR = 0x00800000;  //DUZENLE  CSS Flag clear
	//RCC->CIR |= ( 1 << 23 ); // yukardaki ile ayn� ama cal��mad� yukardaki
}
void gpio_config(){

	RCC->AHB1ENR =   0x00000009; // GPIOA ve GPIOD portlar� active ettik PAGE 180
	// LED
	GPIOD->MODER  =   0x55000000;   // PD12,PD13,PD14,PD15 pinleri active ettik, LED , page 281
	GPIOD->OTYPER |=  0x00000000;  // RESET STATE ,0: Output push-pull (reset state)
	GPIOD->OSPEEDR |= 0x55000000;  // MEDIUM SPEED
	GPIOD->PUPDR |=   0x00000000; // 00: No pull-up, pull-down page 283

	// BUTON ���N CONF YAPMAYA GEREK YOK ASLINDA CUNKU HEPSI DEFAULT RESET OLARAK AYARLANMIS PULL PUSH , PUSH DOWN LOW SPEED, DEFAULT INPUT
	//GPIOA->MODER  =   0x00000000;   // 00 : �NPUT RESET STATE, PA0 BUTTON page 281  bu yorumu silince hata alyorum  anlamad�m MODER deki olay�
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
	gpio_config(); // bunda bokluk var d�zelt

  while (1){

	  if(GPIOA->IDR & 0x00000001){  // GPIOA port is set
		  while(GPIOA->IDR & 0x00000001);
		  delay(1680000);
		  cnt++;
	  }
	  if(cnt%2==0){
		  GPIOD->ODR = 0x0000F000; // LED YANSIN  0000 F000   ilk 4 bit 0000 olan  reserve sayfa 283 kalan 4 bit  ile ilgileniyoruz yaln�z hex bunlar
	  }else{
		  GPIOD->ODR = 0x00000000; // LED SONSUN
	  }

  }
}


 */





