/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include "usbd_cdc_if.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"





/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;
SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */






// for sc_card
uint8_t *cdc;
uint8_t *DWSD = "Write sd ok";
char data[]="deneme sd_card with sdio";
FATFS Fatfs;
FIL fil;
FRESULT fr;
UINT br,bw;
FILINFO fno;




// for i2c AM2320
uint8_t data_humidity[2]={0};
uint8_t am2320_address = 0xB8;
uint8_t am2320_write_address=0x10;
uint8_t am2320_read_address=0x03;


// for i2c adxl345
/*
 I2C address of 0x53 (followed by the R/W bit) can be chosen by
grounding the SDO/ALT ADDRESS pin (Pin 12). This translates to
0xA6 for a write and 0xA7 for a read.
 */
#define ADXL345_I2C_SLAVE_ADDRESS 0xA6 // 0x53 << 1 ,due to the 7 bit address and 1 bit for read/write
uint8_t finding_slave_address_cnt=0;
uint8_t myDatas[6];
int16_t x,y,z;
float xG,yG,zG;






/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




/////////////////////////////////////////////////////////////////////////////
// finding the i2c slave address
void findSlaveAddress(){
	for(finding_slave_address_cnt=0;finding_slave_address_cnt<=255;finding_slave_address_cnt++){
		if(HAL_OK == HAL_I2C_IsDeviceReady(&hi2c1, finding_slave_address_cnt, 1, 100)){
			break;
		}
	}
}




/////////////////////////////////////////////////////////////////////////////
void sd_card_start(){

	fr=f_mount(&Fatfs, "", 1);
	if(fr==FR_OK){
		cdc="Ok to mount\n";
		CDC_Transmit_FS(cdc, strlen(cdc));
	}else{
		cdc="Error to Mount\n";
		CDC_Transmit_FS(cdc, strlen(cdc));
	}

	fr=f_open(&fil, "deneme.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(fr==FR_OK){
		cdc="Ok to create or writefile\n";
		CDC_Transmit_FS(cdc, strlen(cdc));
	}else{
		cdc="Error to create or write file\n";
		CDC_Transmit_FS(cdc, strlen(cdc));
	}

	fr=f_stat("deneme.txt",&fno);
	if(fr==FR_OK){
		cdc="File is ready\n";
		CDC_Transmit_FS(cdc, strlen(cdc));
	}else{
		cdc="File is not ready\n";
		CDC_Transmit_FS(cdc, strlen(cdc));
	}
}

void sd_card_stop(){

	f_close(&fil);
	cdc="END of program\n";
	CDC_Transmit_FS(cdc, strlen(cdc));
	f_mount(&Fatfs, "", 0); // unmount yapıyor sd inactive ediyor diyebiliriz.
}

void sd_card_write(uint8_t data[]){
	f_puts((char*)(data),strlen(data));
}

///////////////////////////////////////////////////////////////////
void i2c1_AM2320_sensor(){
	HAL_I2C_Mem_Read(&hi2c1, am2320_address, am2320_read_address,1,data_humidity , 2, 1000);
	sd_card_start();
	sd_card_write(data_humidity);
	sd_card_stop();

}
/////////////////////////////////////////////////////////////////////


void ADXL345_Read(uint8_t rRegister,uint8_t numberOfBytes){

	HAL_I2C_Mem_Read(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS | 0x01, rRegister, 1, myDatas, numberOfBytes , 100); // 100 ms ,1 byte rRegister uzunlugu

}

void ADXL345_Write(uint8_t wRegister,uint8_t value){
	uint8_t data[2]={0};
	data[0]=wRegister;
	data[1]=value;
	HAL_I2C_Master_Transmit(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS, data, 1, 100);
	//HAL_I2C_Mem_Write(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS, wRegister, 1, value, 2, 100); yukardaki ile aynı şey bence
}

void ADXL345_Init(){


	ADXL345_Read(0x00,1); // 0x00 registerindan 0xE5 okuyorsak demekki cihaz dogru calışıyordur
	ADXL345_Write(0x2D, 0); // reset all bits
	// Setup ->
	ADXL345_Write(0x2D,0x08); // measure bit 1,wake up bit 0,autosleep 0,sleep 0 and wakeup frequency 8 hz
	ADXL345_Write(0x31, 0x01); // +- 4g range
}

void i2c1_adxl345_sensor(){

	 ADXL345_Read(0x32, 6);
	  x = (myDatas[1] << 8 ) | myDatas[0];  // x values in myDatas[0] & myDatas[1]
	  y = (myDatas[3] << 8 ) | myDatas[2];  // y values in myDatas[2] & myDatas[3]
	  z = (myDatas[5] << 8 ) | myDatas[4];  // z values in myDatas[4] & myDatas[5]

	  xG =x*0.0087;
	  yG =y*0.0087;
	  zG =z*0.0087;
}



//////////////////////////////////////////////////////////////////////////////////////////////
void i2c1_bmp180_sensor(){

	 BMP180_Get_Temparature();
	 BMP180_Get_Pressure();

}






//////////////////////////////////////////////////////////////////////////////////////////////
// TIM2 : timer interrupt per second.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// ......

	i2c1_AM2320_sensor(); // is done.
	i2c1_adxl345_sensor(); // not yet
	i2c1_bmp180_sensor(); // not yet

}









/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */



  ADXL345_Init();
  BMP180_Init();





  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SDIO_SD_Init();
  MX_TIM2_Init();
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */



  HAL_TIM_Base_Start_IT(&htim2); //TIM2 start init.



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 41999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
