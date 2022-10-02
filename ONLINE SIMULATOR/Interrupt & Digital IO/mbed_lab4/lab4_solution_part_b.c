#include "mbed.h"
DigitalOut red_led(p5);
DigitalOut blue_led(p6);
InterruptIn button(p7);

int button_count=0;

void ISR_callback(){
    extern int button_count;
   button_count=button_count+1;
}

int main() {
   
   button.fall(callback(&ISR_callback));
    red_led=1;
    blue_led=1;
    
    while(1){
       button_count=0; 
        wait_ms(4000);
           
   
    if(button_count==3){
       
         printf("2 times blink is processing.\n");
         
       red_led=!red_led;  
       wait_ms(200);
       red_led=!red_led;
        wait_ms(200);
       
    button_count=0;
   }
    if(button_count==5){
        printf("4 times blink is processing.\n");
       blue_led=!blue_led;
       wait_ms(200);
       blue_led=!blue_led;
       wait_ms(200);
       blue_led=!blue_led;
       wait_ms(200);
       blue_led=!blue_led;
       wait_ms(200);
      
  
   } 
        
        printf("Button count is: %d \n", button_count);
        
    }

  wait_ms(osWaitForever);
}
