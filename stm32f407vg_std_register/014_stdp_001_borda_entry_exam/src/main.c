#include "stm32f4xx.h"

//uint32_t cnt_timer = 0; // for timer interrupt get counter.
uint8_t address1, data1;
uint8_t address2, data2;
uint8_t address3, data3;

void rcc_config() {

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // i2c1 rcc enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // tim2 interrput rcc enable max 32 bit
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  // i2c1 rcc enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  // i2c2 rcc enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);  // i2c3 rcc enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);  // sc_card rcc enable
}

void gpio_config() {

	GPIO_InitTypeDef init;
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6 | GPIO_PinSource7, GPIO_AF_I2C1);
	init.GPIO_Mode = GPIO_Mode_AF;
	init.GPIO_OType = GPIO_OType_OD;
	init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11; //i2c1 PB6=SCL,i2c1 PB7=SDA
	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	init.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &init);
}

void i2c_config() {

	I2C_InitTypeDef init;
	init.I2C_Ack = I2C_Ack_Enable;
	init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	init.I2C_ClockSpeed = 390000; // max 400KHz according to the referance manuel
	init.I2C_DutyCycle = I2C_DutyCycle_2;
	init.I2C_Mode = I2C_Mode_I2C;
	init.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &init);
	I2C_Cmd(I2C1, ENABLE);

}


void i2c_read() {

	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
	I2C_Send7bitAddress(I2C1, address, I2C_Direction_Receiver);
	while (!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
	I2C_ReceiveData(I2C1, data1);
	while (!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));
	I2C_GenerateSTOP(I2C1, ENABLE);

}



// sd_card eksik
void sd_card_config() {

	SDIO_InitTypeDef init;
	SDIO_CmdInitTypeDef init1;

	init.SDIO_BusWide=SDIO_BusWide_8b;
	init.SDIO_ClockBypass ; // bu boþ kalanlara bak youtube dan
	init.SDIO_ClockDiv ;
	init.SDIO_ClockEdge= SDIO_ClockEdge_Rising;
	init.SDIO_ClockPowerSave ;
	init.SDIO_HardwareFlowControl=SDIO_HardwareFlowControl_Disable;
	SDIO_Init(&init);

	//init1. // bu fonksiyonlarda var.
	SDIO_CmdStructInit(&init1);

}

void sd_card_write(uint8_t data){

	while(SDIO_GetFlagStatus());
	SDIO_WriteData((uint32_t)data);
	//while();

}



void timer_config() {

	TIM_TimeBaseInitTypeDef init;
	init.TIM_ClockDivision = TIM_CKD_DIV1;
	init.TIM_CounterMode = TIM_CounterMode_Up;
	init.TIM_Period = 1999; // for 1 sn , ((period+1)*(prescaler+1))/(clock line) ,max 32 bit tim2
	init.TIM_Prescaler = 41999; 	//  ((1999+1)*(41999+1))/(84*10^6)
	init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &init);
	TIM_Cmd(TIM2, ENABLE);
}

void timer_nvic_config() {

	NVIC_InitTypeDef init;
	init.NVIC_IRQChannel = TIM2_IRQn;
	init.NVIC_IRQChannelCmd = ENABLE;
	init.NVIC_IRQChannelPreemptionPriority = 0;
	init.NVIC_IRQChannelSubPriority = 0;
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_Init(&init);
}




void TIM2_IRQHandler() {

	// ......
	i2c_read(address1, data1);
	i2c_read(address2, data2);
	i2c_read(address3, data3);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

}

int main(void) {

	rcc_config();
	gpio_config();
	i2c_config();
	sd_card_config();
	timer_config();
	timer_nvic_config();

	while (1) {
		//cnt_timer = TIM_GetCounter(TIM2);
	}
}

