#include "mbed.h"

DigitalOut red_led(p5); // 4kere bast�ktan yand�ktan sonra  -> 2 kez bas�nca s�n 
DigitalOut blue_led(p6);//5 kere bast�ktan sonra yan--> 3 kez bas�nca s�n

DigitalIn button_red(p7);
DigitalIn button_blue(p8);



int main(){
   red_led=1;
   blue_led=1;
  int count1=0;
  int count2=0;
  
      
      while(1){
      
      if(button_red==1){
      
          count1=count1+1;
              printf("Button red : %d\n",count1);
          
          if(count1==4){
              red_led=!red_led;
          }
          if(count1==6){
              red_led=!red_led;
              count1=0;
          }
      }
      
       if(button_blue==1){
          
          count2=count2+1;
           printf("Button blue : %d\n",count2);
          if(count2==5){
              blue_led=!blue_led;
          }
          if(count2==8){
              blue_led=!blue_led;
              count2=0;
          }
      }
      
      wait_ms(500);
  }
  }
