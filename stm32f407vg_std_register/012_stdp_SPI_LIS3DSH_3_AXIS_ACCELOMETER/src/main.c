
#include "stm32f4xx.h"

GPIO_InitTypeDef init1;
SPI_InitTypeDef init2;

uint8_t x=0,y=0,z=0;
uint8_t x_address=0x29,y_address=0x2B,z_address=0x2D; // datasheet den baktý,video 111,dakika 12:39 da gösteriyor sayfa bilgisini
uint8_t m_data = 0x00; // tekrar tekrar veri almak için

void gpio_config(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	init1.GPIO_Pin =GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; // leds
	init1.GPIO_Mode=GPIO_Mode_OUT;
	init1.GPIO_OType=GPIO_OType_PP;
	init1.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init1.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOD,&init1);


	init1.GPIO_Mode=GPIO_Mode_OUT; // sensor (LIS302DL) ile ilgili PE3 CS_I2C / SPI'da SS1 ,SS2 ,SS3 felan onlar, active or nonacitve etmek için kullanýlacak.
	init1.GPIO_Pin=GPIO_Pin_3;
	init1.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOE,&init1);

	/* SPI SCK-MOSI-MISO pin Ayarlari */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);	// SCK pin
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);	// MOSI pin
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);	// MISO pin
	// SCK pin  :PA5  ,SPI1
	// MOSI pin :PA6
	// MISO pin :PA7
	init1.GPIO_Mode=GPIO_Mode_AF;
	init1.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	init1.GPIO_Speed=GPIO_Speed_100MHz;
	init1.GPIO_OType = GPIO_OType_PP;
	init1.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&init1);
}

void spi_config(){

	init2.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2; // APB2 HATTI  84 MHZ de calýþýr ,2 ye böldük. 42 mhz de calýþþýn dedik.
	init2.SPI_CPHA=SPI_CPHA_2Edge; // clock daki 2.inci edge i sectik ,kare dalga düþün orda düþen kenarý referans aldýk.
	init2.SPI_CPOL=SPI_CPOL_High; // polarity high or low
	init2.SPI_DataSize=SPI_DataSize_8b;
	init2.SPI_Direction=SPI_Direction_2Lines_FullDuplex; // hem okuma hem yazma için 2 yönlü olsun dedik.
	init2.SPI_FirstBit=SPI_FirstBit_MSB;
	init2.SPI_Mode=SPI_Mode_Master;
	init2.SPI_NSS=SPI_NSS_Soft | SPI_NSSInternalSoft_Set;

	SPI_Init(SPI1,&init2);
	SPI_Cmd(SPI1,ENABLE);

	GPIO_SetBits(GPIOE,GPIO_Pin_3); // CS pini lojik 1 e çekildi

}

void SPI_Write(uint8_t address,uint8_t data){
	// önce adres gönderip sonra datayý göndericez

	GPIO_ResetBits(GPIOE,GPIO_Pin_3); // CS pini lojik 0 a çekildi,artýk ben detaylý bir þekilde kullanabilirim.

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)); // txe interrrupt'i enable olana kadar bekle ,enable oldu sonra ! ile terslendi ve while dan cýktýk
	SPI_I2S_SendData(SPI1,address);

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,data);

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);

	GPIO_SetBits(GPIOE,GPIO_Pin_3);
}


uint8_t SPI_Read(uint8_t address,uint8_t data){

	GPIO_ResetBits(GPIOE,GPIO_Pin_3); // CS pini lojik 0 a çekildi,artýk ben detaylý bir þekilde kullanabilirim.

	address=address | 0x80; // okuma islemi yapýlacak

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)); // txe interrrupt'i enable olana kadar bekle ,enable oldu sonra ! ile terslendi ve while dan cýktýk
	SPI_I2S_SendData(SPI1,address); // adres bilgisini gönderdik.

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1); // adres data bilgisi  geliyor mu diye kontrol ettik.

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,data); // data bilgisini gönderdik

	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1); // data bilgisini geri aldýk

	GPIO_SetBits(GPIOE,GPIO_Pin_3);  //CS pini lojik 1 e çekildi

	return SPI_I2S_ReceiveData(SPI1);
}

void delay(uint32_t tt){
	while(tt>0){
		tt--;
	}
}

int main(void){

	gpio_config();
	spi_config();

	SPI_Write(0x20,0x67);// CTRL_REG1 100Hz, +-8g
	GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	delay(5000000);
	GPIO_SetBits(GPIOD, GPIO_Pin_12 );
	delay(5000000);
	GPIO_SetBits(GPIOD, GPIO_Pin_13 );
	delay(5000000);
	GPIO_SetBits(GPIOD, GPIO_Pin_14);
	delay(5000000);
	GPIO_SetBits(GPIOD, GPIO_Pin_15);
	delay(5000000);
	GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	delay(5000000);
  while (1){
	  x=SPI_Read(x_address,m_data);
	  y=SPI_Read(y_address,m_data);
	  z=SPI_Read(z_address,m_data);

	  delay(100000);
	  if(x < 132){
		  GPIO_ResetBits(GPIOD,GPIO_Pin_13 | GPIO_Pin_15);
	  }else if(x > 201){
		  GPIO_SetBits(GPIOD, GPIO_Pin_13); //
		  GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	  }else if(x >= 133 && x <= 136){
		  GPIO_SetBits(GPIOD, GPIO_Pin_15);
		  GPIO_ResetBits(GPIOD,GPIO_Pin_13);
	  }
	  if(y < 132){
	  	  GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_14);
	  }else if(y > 201 ){
		  GPIO_SetBits(GPIOD, GPIO_Pin_12 ); //
		  GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	  }else if(y >= 133 && y <= 200){
		  GPIO_SetBits(GPIOD, GPIO_Pin_14);
		  GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	  }
	  /*if(z > 139 ){
		  GPIO_SetBits(GPIOD, GPIO_Pin_ );  //
	  }else{
		  GPIO_ResetBits(GPIOD, GPIO_Pin_);
	  }*/


  }
}


// AMAC:
// bu projede kart üzerindeki motion sensor,accelerometer kullanýlacak SPI1 ile.

// alternate function (AF) : SPI,TIMER PWM,USART,I2C



