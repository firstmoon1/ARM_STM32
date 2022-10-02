#include "mbed.h"

DigitalOut blue_led(p5);
DigitalOut red_led(p6);
InterruptIn button1(p7);

int count1=0;
int count2=0;

void buffer1(){
    count1=count1+1;
    count2=count2+1;
    printf("blue %d red %d \n",count1,count2);
    
    if(count1==4){

        blue_led=1;
        wait_ms(500);
        
        count1=0;
         }   
         
     if(count2==6){
          red_led=1;
          wait_ms(500);
          count2=0; 
          
            }
    blue_led=0;
    red_led=0;
            
    }

int main(){
 
    button1.fall(callback(&buffer1));

    wait_ms(osWaitForever);
    
}

