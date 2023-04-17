
#include "stm32f4xx.h"


int i = 0;
uint8_t m_address = 0x4E, TCS_W_ADDRESS = 0xD0;

void RCC_Config()
{
	RCC->CR |= 0x00010000;					// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));			// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;					// CSS Enable
	RCC->CR |= 0x01000000;					// PLL ON
	RCC->PLLCFGR |= 0x00400000;				// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;				// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;				// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;				// PLL p = 2
	RCC->CFGR |= 0x00000000;				// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;				// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;				// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;					// CSS Flag clear
}

// I2C, pb10 =SCL ,pb11 = SDA

void GPIO_Config(){

RCC->AHB1ENR  |= 3<<0;// GPIOA,GPIOB aktif ettik.

GPIOB->AFR[1] |=(4 << 8) | (4 << 12); // PB10 and PB11 AF4 (I2C)
GPIOB->MODER |= (2<<20) | (2<<22) ; //PB10 and PB11 AF = alternate function
GPIOB->OTYPER |= (1<<11) | (1<<12); // OPEN DRAIN yaptýk pb10 pb11

}

void I2C_Config(){

RCC->APB1ENR |= 1<<22; // I2C clock enabled.
I2C2->CR2 |= 0x0008; // I2C clock 8MHz
I2C2->CCR |= 0x0028; // I2C 100 kHz
I2C2->TRISE |= 0x09;
I2C2->CR1 |= 0x0001; // I2C ENABLED

}

void I2C_Write(uint8_t address,uint8_t data){

I2C2->CR1 |= 1<<8; //send start bit
while(!(I2C2->SR1 & 0x0001)); // WAIT FOR START
I2C2->DR =0x4E; // slave address
while(!(I2C2->SR1 & 0x0002)); // wait send address
while(!(I2C2->SR2 & 0x0001)); // wait MASTER
//I2C2->DR =address;  // Cipin adresi
while(!(I2C2->SR1 & 0x0080));  // TxE Data Register'in bosalmasini bekle(Txe)
I2C2->DR =data; //Veriyi Yaz
while(!(I2C2->SR1 & 0x0080));  // TxE Data Register'in bosalmasini bekle(Txe)
while(!(I2C2->SR1 & 0x0004)); // wait BTF successful,Verinin gönderimini bekle (BTF)
I2C2->CR1 |=1<<9; // send stop bit

}


uint8_t I2C_Read(uint8_t address)
{
    I2C2 -> CR1 |= 0x0100;                 // Start bit gönderilir
    while (!(I2C2 -> SR1 & 0x0001));  	   // Start bitinin gönderilmesini bekle (SB = 1)
    I2C2 -> DR = 0xd0;                     // Slave adresini gönder. (LSB = 1)
    while (!(I2C2 -> SR1 & 0x0002));       // Adresin gönderilmesini bekle (ADDR = 1)
    int Status2 = I2C2 -> SR2;             // Flag temizlemek için SR2 register’ý oku
    I2C2 -> DR = address;                      // chip adresini  gönder
    while (!(I2C2 -> SR1 & 0x0080));	   // DR register’ýn boþalmasýný bekle
    while (!(I2C2 -> SR1 & 0x0004));   	   // Byte’ýn gönderilmesini bekle

    I2C2 -> CR1 |= 0x0100;                 // Start bit gönderilir
    while (!(I2C2 -> SR1 & 0x0001));	   // Start bitinin gönderilmesini bekle (SB = 1)
    I2C2 -> DR = 0xd1;                     // Slave adresini gönder. (LSB = 0)
    while (!(I2C2->SR1 & 0x0002));		   // Adresin gönderilmesini bekle (ADDR = 1)
    int Status4 = I2C2 -> SR2;             // Flag temizlemek için SR2 register’ý oku
    while (!(I2C2 -> SR1 & 0x0040)); 	   // byte alýnana kadar bekle
    I2C2 -> CR1 |= 0x0200;                 // Stop biti gönder

     return ((char)I2C2 -> DR);            // okunan byte ile geri dön
}


void delay(uint32_t time)
{
	while(time--);
}


int main(void){

	RCC_Config();
	GPIO_Config();
	I2C_Config();

	while (1){

	  if(GPIOA->IDR & 0x00000001)
	  {
		  i++;
		  delay(6300000);
	  }

	  switch(i)
	  {
	  case 0:
		  I2C_Write(m_address, 0x00);
		  break;
	  case 1:
		  I2C_Write(m_address, 0x01);
		  break;
	  case 2:
		  I2C_Write(m_address, 0x02);
		  break;
	  case 3:
		  I2C_Write(m_address, 0x04);
		  break;
	  case 4:
		  I2C_Write(m_address, 0x08);
		  break;
	  case 5:
		  I2C_Write(m_address, 0x10);
		  break;
	  case 6:
		  I2C_Write(m_address, 0x20);
		  break;
	  case 7:
		  I2C_Write(m_address, 0x40);
		  break;
	  case 8:
		  I2C_Write(m_address, 0x80);
		  break;
	  default:
		  i=0;
		  break;
	  }
  }
}


