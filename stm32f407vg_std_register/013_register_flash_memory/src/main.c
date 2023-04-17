
#include "stm32f4xx.h"


uint32_t data; // flash dan cekilecek veri data ya aktar�lacak.

void RCC_Config(){

	RCC->CR |= 0x00030000;										// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));								// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;										// CSS Enable
	RCC->CR |= 0x01000000;										// PLL ON
	RCC->PLLCFGR |= 0x00400000;									// PLL e HSE se�tik
	RCC->PLLCFGR |= 0x00000004;									// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;									// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;									// PLL p = 2
	RCC->CFGR |= 0x00000000;									// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;									// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;									// APB1 Prescaler = 4
	RCC->CIR |= 0x00080000;										// HSERDY Flag clear
	RCC->CIR |= 0x00800000;										// CSS Flag clear
}


void FLASH_UnLocker(){

	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}


void FLASH_Erase(){

	while((FLASH->SR & 0x00010000) != 0); // binary :0x0001000 hex : 0x10
	FLASH->CR != 1<< 1; // SER set,1 yapt�k
	FLASH->CR != 11 << 3; // sector 11
	FLASH->CR != 1 << 16;

	while((FLASH->SR & 0x00010000) != 0);

	if((FLASH->SR & 0x00000001) != 0){ // end of operaion writing/erasing i�lemi yap�ld�g�nda 1 olur
		FLASH->SR !=0x00000001; // end of operation bitini temizleme i�lemi o biti 1 yaparak gercekle�tirilir.
	}else{
		//silme islemi gercekle�medi,oled ekrana felan belki bir �ey yazd�r�rs�n burdan
	}
	FLASH->CR &= ~1 <<1; // SER reset,0 yapt�k

}


void FLASH_Write(uint32_t address,uint32_t data){

	while((FLASH->SR & 0x00010000) != 0); // busy mi degilmi diye kontrol ediyor
	FLASH->CR != 1 << 0; // PG set,1 yapt�k,flash program activated
	FLASH->CR != 2 << 8; // Psize 32bit yapt�k, 8bit sola �tele ve 2 yaz dedik ,binary:0010 demek 4 bitlik yerde
	while((FLASH->SR & 0x00010000) != 0);


	*(__IO uint32_t*)address=data; //  (__IO uint32_t*) adress i 32bit e cevirdik,istersen yazma dedi,


while((FLASH->SR & 0x00010000) != 0);

	if((FLASH->SR & 0x00000001) != 0){ // end of operaion writing/erasing i�lemi yap�ld�g�nda 1 olur
		FLASH->SR !=0x00000001; // end of operation bitini temizleme i�lemi o biti 1 yaparak gercekle�tirilir.
	}else{
		//silme islemi gercekle�medi,oled ekrana felan belki bir �ey yazd�r�rs�n burdan
	}
	FLASH->CR &= (~1 << 0); // PG reset,0 yapt�k

}


uint32_t FLASH_Read(uint32_t address){

	uint32_t myFlashData;
	myFlashData = *(uint32_t*)address; // yada direk *address yaz ,garanti olsun diye b�yle yapt�
	return myFlashData;;
}


void FLASH_Locker(){
	while((FLASH->SR & 0x00010000) != 0);
	FLASH->CR != 1 <<31; // locklad�k flash l�
}

int main(){

	RCC_Config();
	FLASH_UnLocker();
	FLASH_Erase();
	FLASH_Write(0x080E0000,0xCC);
	FLASH_Locker();
	FLASH_UnLocker();
	data=FLASH_Read(0x080E0000);
	FLASH_Locker();

	while(1){

	}
}





 /*  //hocan�n kodu


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

uint32_t m_FlashData;

void RCC_Config()
{
	RCC->CR |= 0x00030000;										// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));								// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;										// CSS Enable
	RCC->CR |= 0x01000000;										// PLL ON
	RCC->PLLCFGR |= 0x00400000;									// PLL e HSE se�tik
	RCC->PLLCFGR |= 0x00000004;									// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;									// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;									// PLL p = 2
	RCC->CFGR |= 0x00000000;									// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;									// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;									// APB1 Prescaler = 4
	RCC->CIR |= 0x00080000;										// HSERDY Flag clear
	RCC->CIR |= 0x00800000;										// CSS Flag clear
}

void FLASH_UnLock(void)
{

	// FLASH_CR kayd�n�n kilidini a�mak ve programlama/silme i�lemlerini sa�lamak i�in FLASH->KEYR
	// kaydedicisine KEY1 ve KEY2 de�erleri s�ras�yla yaz�lmal�d�r.

	while ((FLASH->SR & 0x00010000) != 0 );	// Flash belle�in me�gul olmamas�n� bekle

	if(FLASH->CR & 0x80000000)				// Fla�'�n kilidinin a��k olup olmad���n� kontrol et
	{
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}
}

void FLASH_Locker(void)
{

	 // FLASH_CR kayd�n�n kilitlemek ve programlama/silme i�lemlerini durdurmak i�in FLASH->CR
	 // kaydedicisinin LOCK bitine 1 yaz�lmal�d�r.

	FLASH->CR |= 0x80000000;
}

void FLASH_Erase(void)
{

	//  Silmek demek Fla�'� 0x0000 yapmak demek de�il 0xFFFF yapmak demektir.
	// Silme i�lemi i�in:
	//  	1- Status registerindaki busy(BSY) bitini kontrol ederek Flash belle�i i�lemlerinin devam etmedi�ini do�rula.
	// 	2- CR kaydedicisindeki SER bitini ayarla ve silmek istedi�in sectoru ana bellek blo�undaki SNB den belirle
	//  	3- CR kaydedicisindeki STRT bitini ayarla
	//  	4- Status registerindaki busy(BSY) bitinin temizlenmesini bekle

	// Bayraklar� temizle
	FLASH->SR |= 0x00000001;				// End of Operation flag clear
	FLASH->SR |= 0x00000040;				// Programming Parallelism error flag clear
	FLASH->SR |= 0x00000010;				// Write protected error flag clear

	while((FLASH->SR & 0x00010000) != 0);	// Me�guliyet bitene kadar bekle
	FLASH->CR |= 0x00000002;				// sekt�r silme bitini ayarla (SER)
	FLASH->CR |= 11 << 3;					// Sekt�r 11 silinecek sekt�r olarak se�ildi
	FLASH->CR |= 1 << 16;					// STRT biti 1 olarak ayarland�
	while(FLASH->SR & 0x00010000);			// ��lemler bitene kadar bekle

	if((FLASH->SR & 0x00000001) != 0)		// ��lem sonu biti ayarland� ise
	{
	    FLASH->SR |= 0x00000001;			// ��lem ba�ar�l� oldu, i�lem sonu ba�ra�� temizle
	}
	FLASH->CR &= ~0x00000002; 				// CR register'� PER biti ba�lang��taki duruma ayarla
}

void FLASH_Write(uint32_t address, uint32_t data)
{

	//  Yazma i�lemi i�in:
	//  	1- Status register'�ndaki busy(BSY) bitini kontrol ederek Flash belle�i i�lmelerinin devam etmedi�ini do�rula
	//  	2- CR kaydedicisindeki Programming Enable(PG) bitini ayarla
	// 	3- Veri yazma i�lemini istenilen adrese yap (ana haf�za blo�u veya OTP i�erisinde)
	// 		- x8 paralelli�i durumunda bayt eri�imi
	//		- x16 paralellik durumunda yar�m kelimelik eri�im
	// 		- x32 paralellik durumunda kelime eri�imi
	//		- x64 paralellik durumunda �ift kelime eri�imi
	//  4- Status register'�ndaki busy(BSY) bitinin temizlenmesini bekle

	while(FLASH->SR & 0x00010000);		// Me�guliyet bitene kadar bekle
	FLASH->CR |= 0x00000001;			// PG biti ayarland�, Programmin modda
	FLASH->CR |= 2 << 8;				// Program size 32
	*(__IO uint32_t*)address = data;	// �stenilen adrese istenilen veri yaz�l�yor.
	while(FLASH->SR & 0x00010000);		// ��lemler bitene kadar bekle

	if((FLASH->SR & 0x00000001) != 0)	// ��lem sonu biti ayarland� ise
	{
	     FLASH->SR |= 0x00000001;		// ��lem ba�ar�l� oldu, i�lem sonu ba�ra�� temizle
	}

	FLASH->CR &= ~0x00000001;			// CR register'� PG biti ba�lang��taki duruma ayarla
}

uint32_t FLASH_Read(uint32_t address)
{
	uint32_t myFlashData;

	myFlashData = (*(uint32_t*)address);

	return myFlashData;
}

int main(void)
{
	RCC_Config();

	FLASH_UnLock();
	FLASH_Erase();
	FLASH_Write(0x080E0000, 0xDB);
	FLASH_Locker();
	FLASH_UnLock();
	m_FlashData = FLASH_Read(0x080E0000);
	FLASH_Locker();
  while (1)
  {

  }
}

*/


