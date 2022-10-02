#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x.h"                  // Device header
uint32_t arrsin[]={2048,2248,2447,2642,2831,3013,3185,3346,
3495,3630,3750,3853,3939,4007,4056,4085,
4095,4085,4056,4007,3939,3853,3750,3630,
3495,3346,3185,3013,2831,2642,2447,2248,
2048,1847,1648,1453,1264,1082,910,749,
600,465,345,242,156,88,39,10,
0,10,39,88,156,242,345,465,
600,749,910,1082,1264,1453,1648,1847,};

uint32_t arrtri[]={128,256,384,512,640,768,896,1024,
1152,1280,1408,1536,1664,1792,1920,2048,
2175,2303,2431,2559,2687,2815,2943,3071,
3199,3327,3455,3583,3711,3839,3967,4095,
3967,3839,3711,3583,3455,3327,3199,3071,
2943,2815,2687,2559,2431,2303,2175,2048,
1920,1792,1664,1536,1408,1280,1152,1024,
896,768,640,512,384,256,128,0};

void gpio_config(){
	GPIO_InitTypeDef init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	init.GPIO_Mode=GPIO_Mode_AIN;
	init.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOA,&init);
	
}

void dac_config(){
	DAC_InitTypeDef init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	init.DAC_LFSRUnmask_TriangleAmplitude=DISABLE;
	init.DAC_OutputBuffer=ENABLE;
	init.DAC_Trigger=DAC_Trigger_None;
	init.DAC_WaveGeneration=DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_1,&init);
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
}

int main(){
	gpio_config();
	dac_config();
	int i=0;
	while(1){
		/*for(i=0;i<64;i++){	
		DAC_SetChannel1Data(DAC_Align_12b_R,arrsin[i]);
		}*/
		for(i=0;i<64;i++){	
		DAC_SetChannel1Data(DAC_Align_12b_R,arrtri[i]);
		}
		
	}
}

/*
osilaskop olsaydi gösterirdi sinus ile triangular dalgalari 

*/


