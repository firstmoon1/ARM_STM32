#include "stm32f4xx.h"

void gpio_config(){

	GPIO_InitTypeDef init;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	// led
	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&init);

	/*
	// button
	init.GPIO_Mode=GPIO_Mode_IN;
	init.GPIO_OType=GPIO_OType_PP;  // push pull = PP or open drain = OD
	init.GPIO_Pin=GPIO_Pin_0;
	init.GPIO_PuPd=GPIO_PuPd_DOWN; // puld down or pull up
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&init);
	*/

}


/*
void delay(uint32_t tt){
	while(tt--);
}
*/
int main(void){

	gpio_config();


	while (1){



		/*
		if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) ){

			GPIO_SetBits(GPIOD, GPIO_Pin_12 );
			delay(7200000);
			GPIO_ResetBits(GPIOD, GPIO_Pin_12 );
			delay(7200000);

			GPIO_SetBits(GPIOD, GPIO_Pin_13  );
			delay(7200000);
			GPIO_ResetBits(GPIOD, GPIO_Pin_13  );
			delay(7200000);

			GPIO_SetBits(GPIOD, GPIO_Pin_14  );
			delay(7200000);
			GPIO_ResetBits(GPIOD, GPIO_Pin_14  );
			delay(7200000);

			GPIO_SetBits(GPIOD, GPIO_Pin_15 );
			delay(7200000);
			GPIO_ResetBits(GPIOD, GPIO_Pin_15 );
			delay(7200000);
		}
		*/

		GPIO_SetBits(GPIOD, GPIO_Pin_12 );
		GPIO_SetBits(GPIOD, GPIO_Pin_13 );
		GPIO_SetBits(GPIOD, GPIO_Pin_14 );
		GPIO_SetBits(GPIOD, GPIO_Pin_15 );

	}

}
