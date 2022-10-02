
#include "stm32f4xx.h"

void gpio_config(){

	GPIO_InitTypeDef init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	//button
	init.GPIO_Mode=GPIO_Mode_IN; //init.GPIO_Mode=0x00;  , GPIO_Mode_IN   = 0x00 sag týk yap direk buna gider  ha GPIO_Mode_IN yazmýssýn ha 0x00 ayný
	init.GPIO_OType=GPIO_OType_PP;
	init.GPIO_Pin=GPIO_Pin_0;
	init.GPIO_PuPd=GPIO_PuPd_DOWN;
	init.GPIO_Speed=0x03; //init.GPIO_Speed=GPIO_Speed_100MHz; dür aslýnda
	GPIO_Init(GPIOA,&init);

	// led
	init.GPIO_Mode=GPIO_Mode_OUT;
	//init.GPIO_OType=GPIO_OType_OD; // open drain yaptýn ama calýþmadý anlamadým sorna pp ye geçtik
	init.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	init.GPIO_Speed=GPIO_Speed_100MHz;
	init.GPIO_OType=GPIO_OType_PP;  // bu ikisini yazacagýna direk init.GPIO_OType=GPIO_OType_OD; yaz geç
	init.GPIO_PuPd=GPIO_PuPd_NOPULL; // default olarak bunu yapýyor aslýnda yazmasanda olur
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
	  /*if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		  GPIO_SetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 );
	  }else{
		  GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 );
	  }*/
	  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)); // butondan elini kaldýrana kadar while döngüsünde kalýr, butona basýlýyken logic 1 verir analog 3.3v
		  //delay(3360000);
		  GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		  delay(3360000); // 200 ms tekabul eder
	  }

  }
}














