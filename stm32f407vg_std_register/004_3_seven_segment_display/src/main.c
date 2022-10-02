
#include "stm32f4xx.h"
int cnt=0;

void gpio_config(){
	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	//buton
	init.GPIO_Pin=GPIO_Pin_1;
	init.GPIO_Mode=GPIO_Mode_IN;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_PuPd=GPIO_PuPd_DOWN;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&init);

	//led
	init.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	init.GPIO_Mode=GPIO_Mode_OUT;
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_PuPd=GPIO_PuPd_NOPULL;
	init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&init);

}
void delay(uint32_t tt){
	while(tt--);
}

int main(void)
{
	gpio_config();

  while (1)
  {

	  if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)){
		  while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1));
		  delay(1680000); // 200 ms
		  cnt++;
	  }
		  switch(cnt){

		  case 0:
			  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
			  GPIO_SetBits(GPIOD, GPIO_Pin_6);
			  break;
		  case 1:
			  GPIO_ResetBits(GPIOD, GPIO_Pin_1 | GPIO_Pin_2);
			  GPIO_SetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
			  break;
		  case 2:
			  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4  | GPIO_Pin_6);
			  GPIO_SetBits(GPIOD, GPIO_Pin_2 | GPIO_Pin_5 );
			  break;
		  case 3:
			  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6);
			  GPIO_SetBits(GPIOD, GPIO_Pin_4 | GPIO_Pin_5 );
			  break;
		  case 4:
			  GPIO_ResetBits(GPIOD, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6);
			  GPIO_SetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 );
			  break;
		  case 5:
			  GPIO_ResetBits(GPIOD,  GPIO_Pin_0  | GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_5 | GPIO_Pin_5);
			  GPIO_SetBits(GPIOD, GPIO_Pin_1 | GPIO_Pin_4);
			  break;
		  case 6:
			  GPIO_ResetBits(GPIOD,GPIO_Pin_0  | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
			  GPIO_SetBits(GPIOD, GPIO_Pin_1);
			  break;
		  case 7:
			  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );
			  GPIO_SetBits(GPIOD,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
			  break;
		  case 8:
			  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
			  break;
		  case 9:
			  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_5 | GPIO_Pin_6);
			  GPIO_SetBits(GPIOD, GPIO_Pin_4 );
			  break;

		  default :
			  cnt=0;
		  }





  }
}
