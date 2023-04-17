
#include "stm32f4xx.h"
#include "i2c-lcd.h"

GPIO_InitTypeDef init_gpio;
I2C_InitTypeDef init_i2c;

uint8_t i=0,m_address=0x4E;

void clock_line_config(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

}

void gpio_config(){

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1); // i2c scl pin
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1); // i2c sda pin

	init_gpio.GPIO_Mode=GPIO_Mode_AF;
	init_gpio.GPIO_OType=GPIO_OType_OD;
	init_gpio.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;  // Pin6 SCL, Pin7 SDA
	init_gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init_gpio.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&init_gpio);

}

void i2c_config(){

	I2C_InitTypeDef init_i2c;

	init_i2c.I2C_Ack=I2C_Ack_Enable;
	init_i2c.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	init_i2c.I2C_ClockSpeed=100000;  // 400KHz den kücük ne frequecny verirsen ver. max 400000 verebilirsin.
	init_i2c.I2C_DutyCycle=I2C_DutyCycle_2;
	init_i2c.I2C_Mode=I2C_Mode_I2C;
	init_i2c.I2C_OwnAddress1=0x00; // master slave iliskisi, master memory konumu 0x00 olsun.

	I2C_Init(I2C1,&init_i2c);
	I2C_Cmd(I2C1,ENABLE);
}



void i2c_write(uint8_t address,uint8_t data){

	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1,ENABLE);

	while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))); // master -> slave
	I2C_Send7bitAddress(I2C1,address,I2C_Direction_Transmitter); // veri göndericez from master to slave

	while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))); // check ettik transmit mode selected or not,
	I2C_SendData(I2C1,data);

	while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))); // bu i2c de  iþlemler gercekleþmesi için zaman harcanýyor bu yüzden check etmemiz gerek.
	I2C_GenerateSTOP(I2C1,ENABLE);

}

/*
 bu delay alttaki 8 cycle sürer, 168mhz/8 yaparsak 21milyon olur
 2100000 demek 100milisaniye demek.
 */
void delay(uint32_t tt){
	while(tt--);
}


int main(void){

	clock_line_config();
	gpio_config();
	i2c_config();
	lcd_init();
	lcd_send_cmd(0x01); // clear display command.


  while (1){

	  lcd_send_cmd(0x80); // go to 1 . 1  lcd deki yer 2x16 lýk yerde
	  lcd_sende_string("ilkay");
	  delay(6300000);
	  lcd_send_cmd(0xC0); // go to 2 . 1  // 0xC3 dersen de 2 . 4 den baslar
	  lcd_sende_string("koc");
	  delay(6300000);
	  lcd_send_cmd(0x01);
	  delay(2100000);

  }
}
