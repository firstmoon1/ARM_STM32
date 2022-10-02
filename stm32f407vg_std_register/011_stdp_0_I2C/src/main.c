

#include "stm32f4xx.h"

uint8_t i=0 , m_address = 0x27;

void gpio_conf(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

	init.GPIO_Mode=GPIO_Mode_IN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_0;
	init.GPIO_PuPd=GPIO_PuPd_DOWN;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&init);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);

	init.GPIO_Mode=GPIO_Mode_AF;
	init.GPIO_OType=GPIO_OType_OD;
	init.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7; // Pin6 SCL, Pin7 SDA
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&init);




}

void I2C_conf(){

	I2C_InitTypeDef init;

	init.I2C_Ack=I2C_Ack_Enable;
	init.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	init.I2C_ClockSpeed=100000; // 400KHz den kücük ne frequecny verirsen ver.
	init.I2C_DutyCycle=I2C_DutyCycle_2;
	init.I2C_Mode=I2C_Mode_I2C;
	init.I2C_OwnAddress1=0x00; // master slave iliskisi, master memory konumu 0x00 olsun.

	I2C_Init(I2C1,&init);
	I2C_Cmd(I2C1,ENABLE);

}

void I2C_Write_function_ceylan(uint8_t address,uint8_t data){


	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)); //i2c bayragý uygun olana kadar bekle
	I2C_GenerateSTART(I2C1,ENABLE);

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,address,I2C_Direction_Transmitter);

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1,data);

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1,ENABLE);

}

void delay(uint32_t tt){
	while(tt--);
}

int main(void){

	gpio_conf();
	I2C_conf();

  while (1){

	  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
		  i++;
		  delay(16800000);
	  }

	  switch(i)
	  {
	  case 0:
		  I2C_Write_function_ceylan(m_address, 0x00);
		  break;
	  case 1:
		  I2C_Write_function_ceylan(m_address, 0x01);
		  break;
	  case 2:
		  I2C_Write_function_ceylan(m_address, 0x02);
		  break;
	  case 3:
		  I2C_Write_function_ceylan(m_address, 0x04);
		  break;
	  case 4:
		  I2C_Write_function_ceylan(m_address, 0x08);
		  break;
	  case 5:
		  I2C_Write_function_ceylan(m_address, 0x10);
		  break;
	  case 6:
		  I2C_Write_function_ceylan(m_address, 0x20);
		  break;
	  case 7:
		  I2C_Write_function_ceylan(m_address, 0x40);
		  break;
	  case 8:
		  I2C_Write_function_ceylan(m_address, 0x80);
		  break;
	  default:
		  i=0;
		  break;
	  }


  }
}



/*
 ders videosu 96dýr. I2C de.,

 led yakýyon yani  butona bastýkca switch case deki adreslerin baglý oldugu ledler set olur.


 */









