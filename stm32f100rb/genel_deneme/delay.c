/**
  ******************************************************************************
  * @file      delay.c
  * @author    Arif Mandal
  * @date      12/05/2020
  ******************************************************************************
  */
   
#include "delay.h"

// ticks counter
static __IO uint32_t counter;

void SysTick_Handler()  // her bir 1ms de bir kesme olur
{
   if(counter > 0)
      counter--;   // 1ms de bir bu islem yapilir
}

void delay_ms(uint32_t time) 
{
   counter = time;

   while(counter);
}

void DelayInit()
{
   SystemCoreClockUpdate();
   SysTick_Config(SystemCoreClock / 1000); // saat'imi 1ms olacak sekilde boldum,1000ms = 1sn dir ,her 1 ms de bir interrupt'a gidecek
	 //SysTick_Config(SystemCoreClock / 1000000); 1us oldu 3 sifir daha ekleyince
}


/********************************* END OF FILE ********************************/
/******************************************************************************/