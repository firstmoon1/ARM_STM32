#include "mbed.h"

DigitalOut red_led(p5);
DigitalOut blue_led(p6);
DigitalIn button_1(p7);
DigitalIn button_2(p8);




int main(){
    red_led=0;
    blue_led=0;
    int count_1=0;
    int count_2=0;
    
    while(1){
    if(button_1==1){
        count_1=(count_1)+1;
        printf("blue %d\n",count_1);
    
        if(count_1==4){
        blue_led=!blue_led;
       
    
        count_1=0;
    }
        
        
    }
    else if(button_2==1){
        count_2=(count_2)+1;
         printf("red %d\n",count_2);
         if(count_2==6){
         red_led=!red_led;
        
      
        count_2=0;
    }
    }
    
    
      wait_ms(500);
    
}
}
