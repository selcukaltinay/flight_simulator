/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "BMP180.h"
#include "MPU6050.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MPU6050_SLAVE_ADDRESS 0xD0
#define MPU6050_WHO_AM_I 0x75
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart4;

osThreadId task1_manyetoHandle;
osThreadId task2_bmpHandle;
osThreadId task4_tempHandle;
osThreadId task5_mpuHandle;
osThreadId task6_joyHandle;
osMutexId mutexUARTHandle;
/* USER CODE BEGIN PV */
uint16_t rxControl[2];
char uartMsg[50];
float temperature, tempFrac;
int tempInt1, tempInt2, presInt;
RawData_Def myAccelRaw, myGyroRaw;
ScaledData_Def myAccelScaled, myGyroScaled;
MPU_ConfigTypeDef mpu6050_t;
float orientation;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_UART4_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
void taskCOMPASS(void const * argument);
void taskALTITUDE(void const * argument);
void taskTEMPERATURE(void const * argument);
void taskGYRO(void const * argument);
void taskCONTROL(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
Gyro_TypeDef gyro;
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
  //static float fTemperature;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_UART4_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Create the mutex(es) */
  /* definition and creation of mutexUART */
  osMutexDef(mutexUART);
  mutexUARTHandle = osMutexCreate(osMutex(mutexUART));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of task1_manyeto */
  osThreadDef(task1_manyeto, taskCOMPASS, osPriorityNormal, 0, 128);
  task1_manyetoHandle = osThreadCreate(osThread(task1_manyeto), NULL);

  /* definition and creation of task2_bmp */
  osThreadDef(task2_bmp, taskALTITUDE, osPriorityNormal, 0, 128);
  task2_bmpHandle = osThreadCreate(osThread(task2_bmp), NULL);

  /* definition and creation of task4_temp */
  osThreadDef(task4_temp, taskTEMPERATURE, osPriorityNormal, 0, 128);
  task4_tempHandle = osThreadCreate(osThread(task4_temp), NULL);

  /* definition and creation of task5_mpu */
  osThreadDef(task5_mpu, taskGYRO, osPriorityNormal, 0, 256);
  task5_mpuHandle = osThreadCreate(osThread(task5_mpu), NULL);

  /* definition and creation of task6_joy */
  osThreadDef(task6_joy, taskCONTROL, osPriorityNormal, 0, 128);
  task6_joyHandle = osThreadCreate(osThread(task6_joy), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 2;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PG13 PG14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_taskCOMPASS */
/**
  * @brief  Function implementing the task1_manyeto thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_taskCOMPASS */
void taskCOMPASS(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_taskALTITUDE */
/**
* @brief Function implementing the task2_bmp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_taskALTITUDE */
void taskALTITUDE(void const * argument)
{
  /* USER CODE BEGIN taskALTITUDE */
  /* Infinite loop */
  BMP180_Init();

  for(;;)
  {
	  BMP180_GetTemperature();
	  BMP180_GetPressure();
	  osMutexWait(mutexUARTHandle, osWaitForever);
	  presInt=10000*comPressure;
	  sprintf(uartMsg, "basinc:0.%d\r\n",presInt);
	  HAL_UART_Transmit(&huart4,uartMsg,strlen(uartMsg),HAL_MAX_DELAY);
	 osMutexRelease(mutexUARTHandle);
	  osDelay(10);
  }
  /* USER CODE END taskALTITUDE */
}

/* USER CODE BEGIN Header_taskTEMPERATURE */
/**
* @brief Function implementing the task4_temp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_taskTEMPERATURE */
void taskTEMPERATURE(void const * argument)
{
  /* USER CODE BEGIN taskTEMPERATURE */
  /* Infinite loop */

  for(;;)
  {
	  osMutexWait(mutexUARTHandle, osWaitForever);
		  temperature=comTemp*0.1;
		  tempInt1 = temperature;
		  tempFrac = temperature-tempInt1;
		  tempInt2 = tempFrac*10000;
		  sprintf(uartMsg, "sicaklik:%d.%d\r\n",tempInt1,tempInt2);
		  HAL_UART_Transmit(&huart4,uartMsg,strlen(uartMsg),HAL_MAX_DELAY);
	  osMutexRelease(mutexUARTHandle);
    osDelay(50);
  }
  /* USER CODE END taskTEMPERATURE */
}

/* USER CODE BEGIN Header_taskGYRO */
/**
* @brief Function implementing the task5_mpu thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_taskGYRO */
void taskGYRO(void const * argument)
{
  /* USER CODE BEGIN taskGYRO */
	MPU6050_Init(&hi2c1);
	mpu6050_t.Accel_Full_Scale = AFS_SEL_4g;
	mpu6050_t.ClockSource = Internal_8MHz;
	mpu6050_t.CONFIG_DLPF = DLPF_184A_188G_Hz;
	mpu6050_t.Gyro_Full_Scale = FS_SEL_500;
	mpu6050_t.Sleep_Mode_Bit = 0;  //1: sleep mode, 0: normal mode
	MPU6050_Config(&mpu6050_t);
	gyro.x=0;
	gyro.y=0;
	gyro.z=0;

  /* Infinite loop */
  for(;;)
  {
	/*if(HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_SLAVE_ADDRESS, 3, HAL_MAX_DELAY) == HAL_OK)
	{


		/*while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MPU6050_SLAVE_ADDRESS, MPU6050_WHO_AM_I, 1, 1000) != HAL_OK){

		    if (HAL_I2C_GetError(&amp;hi2c1) != HAL_I2C_ERROR_AF)
		    {
		      Error_Handler();
		    }
		}*/

		MPU6050_Get_Accel_RawData(&myAccelRaw);
		HAL_Delay(10);
		MPU6050_Get_Gyro_RawData(&myGyroRaw);
		HAL_Delay(10);
		MPU6050_Get_Accel_Scale(&myAccelScaled);
		HAL_Delay(10);
		MPU6050_Get_Gyro_Scale(&myGyroScaled);
		HAL_Delay(10);
		osMutexWait(mutexUARTHandle, osWaitForever);
			sprintf(uartMsg, "ivmex:%d ivmey:%d ivmez:%d\r\n",(int16_t)myAccelScaled.x,(int16_t)myAccelScaled.y, (int16_t)myAccelScaled.z);
			HAL_UART_Transmit(&huart4,uartMsg,strlen(uartMsg),HAL_MAX_DELAY);
			sprintf(uartMsg, "gyrox:%d gyroy:%d gyroz:%d\r\n",(int16_t)myGyroScaled.x,(int16_t)myGyroScaled.y, (int16_t)myAccelScaled.z);
			HAL_UART_Transmit(&huart4,uartMsg,strlen(uartMsg),HAL_MAX_DELAY);
		osMutexRelease(mutexUARTHandle);
		//sprintf(uartMsg, "rx%hu\r\n", rxControl);
		//HAL_UART_Transmit(&huart4,myAccelScaled.x,9,HAL_MAX_DELAY);
		//osMutexRelease(mutexUARTHandle);
		//}
    osDelay(50);
  }
  /* USER CODE END taskGYRO */
}

/* USER CODE BEGIN Header_taskCONTROL */
/**
* @brief Function implementing the task6_joy thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_taskCONTROL */
void taskCONTROL(void const * argument)
{
  /* USER CODE BEGIN taskCONTROL */
  /* Infinite loop */
  for(;;)
  {

	  	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)rxControl, 2);
	/*if(HAL_ADC_PollForConversion(&hadc1, 100)== HAL_OK)
	{
		rxControl = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 100);
		ryControl = HAL_ADC_GetValue(&hadc1);

		osMutexWait(mutexUARTHandle, osWaitForever);
		sprintf(uartMsg, "rx%hu\r\n", rxControl);
		HAL_UART_Transmit(&huart4,uartMsg,8,HAL_MAX_DELAY);
		sprintf(uartMsg, "ry%hu\r\n", ryControl);
		HAL_UART_Transmit(&huart4,uartMsg,8,HAL_MAX_DELAY);
		osMutexRelease(mutexUARTHandle);

	}
	HAL_ADC_Stop(&hadc1);*/
	  	osMutexWait(mutexUARTHandle, osWaitForever);
	  	sprintf(uartMsg, "rx%d\r\n", rxControl[0]);
	  	HAL_UART_Transmit(&huart4,uartMsg,8,HAL_MAX_DELAY);
	  	sprintf(uartMsg, "ry%d\r\n", rxControl[1]);
	  	HAL_UART_Transmit(&huart4,uartMsg,8,HAL_MAX_DELAY);
	  	osMutexRelease(mutexUARTHandle);
	  	HAL_ADC_Stop_DMA(&hadc1);

	osDelay(50);
	//HAL_UART_Transmit(&huart4, "Selcuk\n", 7, HAL_MAX_DELAY);
  }
  /* USER CODE END taskCONTROL */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
