
#include "stm32f4xx.h"


extern uint32_t SystemCoreClock ;  // frekans osilatörü, HSE varya 168 MHz o iþte  ,system_stm32f4xx.c de line 182 deydi
uint32_t system_clock;
int i=0;
/*  // benim rcc config ,gene 168MHz gösteriyor ama biz PLLCFGR yi hiç bir þekilde degiþtiremiyoruz
void rcc_conf(){ // hocanýnki ile ayný degil yanlýþ yapmýs 2,3 yerde

	RCC->CR &= !(1<<0); // HSI inactive
	RCC->CR |= 0x0001000; // HSE enable
	while(!(RCC->CR & 0x00020000)); // HSEON ready flag wait
	RCC->CR |= 0x00080000; // CSS enable
	RCC->PLLCFGR =0x004002A4;
	//RCC->PLLCFGR &= !(32767 << 0); // pllm ,plln 0 to 14 bite kadar sýnýrlar dahil , 0 yaptýk hepsini
	RCC->PLLCFGR |= 0x00400000; // PLL'e HSE SECTÝK
	RCC->PLLCFGR |= 0x00000004; // PLL M =4
	RCC->PLLCFGR |= 0x00002A00; // PLL N=168,false 5A=90 ,2A=168
	RCC->PLLCFGR |= 0x00000000; // PLL P=2

	//RCC->CFGR |= 0x00080000;	// clock config photo1 actýgýnda HSE ve LSE vardýr , LSE nin üstündeki HSE /2 var ya ordaki RTC den bahseder apb2 felan degil
	RCC->CFGR |= 0x00009400;	// APB1 Prescaler = 4 ,APB2 Prescaler =2 için ise  buranýn direk 0x00009400
	RCC->CFGR |= 0x00000002; //SW , pll  sectik
	while(!(RCC->CFGR & 0x00000008)); // SW için flag
	RCC->CIR |= 0x00800000;		// CSS Flag clear ,Clock security system interrupt clear
}
*/

// hocanýn rcc config , gene 168MHz gösteriyor ama biz PLLCFGR yi hiç bir þekilde degiþtiremiyoruz
void rcc_conf()
{
	RCC->CR |= 0x00010000;	// HSEON enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2
	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;		// CSS Flag clear
}

void dac_conf(){

	RCC->AHB1ENR |= 0x00000001;  // GPIOA ACTÝVE
	RCC->APB1ENR |= 0x20000000; // APB1 CLOCK LÝNE ACTIVE

	DAC->CR |= 0x00000001; // DAC Channel 1 enable page 445
	DAC->SWTRIGR &= (3<<0);  // DAC Channel 1 software trigger disable, page 448
	DAC->DHR12R1 &= (4095<<0); // DAC Channel 1 12-bit right-aligned data,ilk 11 biti 0 yaptýk , initialda veri olmasýn diye page 448

}
void delay(uint32_t tt){
	while(tt--);
}

int main(void){

	rcc_conf();
	SystemCoreClockUpdate();
	system_clock=SystemCoreClock; //  system clock speed ? ,168 MHz ???

	dac_conf();


	while (1){
		for(i=0;i<4096;i++){
			DAC->DHR12R1 =i; // verileri buna yazarýz yada alýrýz
			delay(10000);
		}

	}
}
