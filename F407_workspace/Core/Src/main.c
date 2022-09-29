/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usbd_hid.h"
#include "keycodes.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEYCODE_LIMIT 6

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */

// rotary direction
int currentStateCLK;
int lastStateCLK;
int currentStateDT;
int rotLock;

int keycodeNum;

int row;


/* keyccodes for:
   7 8 9 [space]
   4 5 6 w
   1 2 3 .
   < ^ v > 
*/
const uint8_t keys[5][4] =  {{KEY_7, KEY_8, KEY_9, KEY_SPACE},
                             {KEY_4, KEY_5, KEY_6, KEY_W},
                             {KEY_1, KEY_2, KEY_3, KEY_PERIOD},
                             {KEY_LEFT, KEY_UP, KEY_DOWN, KEY_RIGHT},
                             {KEY_MUTE, KEY_VOLDOWN, KEY_VOLUP, 0}};

char keypresses[5][4] = {{0, 0, 0, 0},
                         {0, 0, 0, 0},
                         {0, 0, 0, 0},
                         {0, 0, 0, 0},
                         {0, 0, 0, 0}};

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef struct
{
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
} keyboardHID;

keyboardHID keyboardhid = {0,0,0,0,0,0,0,0};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void set_rows();
int get_cols();
void add_keypress(char key);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
 // Write character to ITM ch.0
 ITM_SendChar(ch);
 return(ch);
}
void set_rows() {
  // update row value
  row = (row + 1) % 4;
  // set all rows to high
  HAL_GPIO_WritePin(GPIOD, ROW0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW3_Pin, GPIO_PIN_SET);

  // use global variable row to set the correct row to low
  switch(row) {
    case 0:
      HAL_GPIO_WritePin(GPIOD, ROW0_Pin, GPIO_PIN_RESET);
      break;
    case 1:
      HAL_GPIO_WritePin(GPIOD, ROW1_Pin, GPIO_PIN_RESET);
      break;
    case 2:
      HAL_GPIO_WritePin(GPIOD, ROW2_Pin, GPIO_PIN_RESET);
      break;
    case 3:
      HAL_GPIO_WritePin(GPIOD, ROW3_Pin, GPIO_PIN_RESET);
      break;
  }

}

int get_cols() {
  // get the value of all columns as one int from the GPIO register
  // PD7-4
  return ~(GPIOD->IDR >> 4) & 0xF;
}

void add_keypress(char key) {
	if(keycodeNum <= KEYCODE_LIMIT) {
	  switch(keycodeNum) {
		case 1:
		  keyboardhid.KEYCODE1 = key;
		  break;
		case 2:
		  keyboardhid.KEYCODE2 = key;
		  break;
		case 3:
		  keyboardhid.KEYCODE3 = key;
		  break;
		case 4:
		  keyboardhid.KEYCODE4 = key;
		  break;
		case 5:
		  keyboardhid.KEYCODE5 = key;
		  break;
		case 6:
		  keyboardhid.KEYCODE6 = key;
		  break;
	  }
	  keycodeNum++;
	}

}

void record_keys() {
  // reset keyboardhid to 0
  memset(&keyboardhid, 0, sizeof(keyboardHID));
  keycodeNum = 1;

  // add currently pressed keys to the keyboardhid keypresses
  for(int i = 0; i < 5; i++) {
    for(int j = 0; j < 4; j++) {
      if(keypresses[i][j] == 1) {
    	  add_keypress(keys[i][j]);
      }
    }
  }

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  // start the timer
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start_IT(&htim6);

  // init counter variables
  row = 0;
  keycodeNum = 1;
  lastStateCLK = 0;
  rotLock = 0;

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
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 160-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 100-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1600-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 10-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ROW0_Pin|ROW1_Pin|ROW2_Pin|ROW3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : COL4_Pin COL0_Pin COL1_Pin COL2_Pin
                           COL3_Pin */
  GPIO_InitStruct.Pin = COL4_Pin|COL0_Pin|COL1_Pin|COL2_Pin
                          |COL3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : ROT_SW_Pin ROT_DT_Pin ROT_CLCK_Pin */
  GPIO_InitStruct.Pin = ROT_SW_Pin|ROT_DT_Pin|ROT_CLCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW0_Pin ROW1_Pin ROW2_Pin ROW3_Pin */
  GPIO_InitStruct.Pin = ROW0_Pin|ROW1_Pin|ROW2_Pin|ROW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// Callback: timer has reset
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	// Send USB data
	if (htim == &htim4) {
	    // send HID report
		record_keys();
		USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboardhid, sizeof(keyboardhid));

	}

	// Keypad scanning
	else if(htim == &htim6) {

		/* Row-Col Scanning */
		// update the rows
		set_rows();

		// read the columns
		int cols = get_cols();
		// if a key was pressed, print the key
		for(int i = 0; i < 4; i++) {
			if(cols & (1 << i)) {
				keypresses[row][i] = 1;
			}
			else {
				keypresses[row][i] = 0;
			}

		}
		/* END Row-Col Scanning */


		/* Rotary Encoder */
		currentStateCLK = HAL_GPIO_ReadPin(GPIOC, ROT_CLCK_Pin);

		// if CLK pin has changed, then the rotary encoder has turned
		if (currentStateCLK != lastStateCLK && rotLock == 0) {
			// if the DT state is different, then the encoder is rotating counter-clockwise
			currentStateDT = HAL_GPIO_ReadPin(GPIOC, ROT_DT_Pin);

			if (currentStateDT != currentStateCLK) {
				// Volume Down
				keypresses[4][1] = 1;
			}

			// otherwise, it is turning clockwise
			else if (currentStateDT == currentStateCLK) {
				// Volume Up
				keypresses[4][2] = 1;
			}
			rotLock++;

		}
		else if (rotLock == 0) {
			keypresses[4][1] = 0;
			keypresses[4][2] = 0;
		}

		lastStateCLK = currentStateCLK;

		// rotLock allows the rotary encoder's inputs to settle after a change is detected over a few extra clock cycles
		if(rotLock != 0) {
			rotLock = (rotLock + 1) % 200;
		}

		// if the state is low (default is high), turn toggle the LED
		if (HAL_GPIO_ReadPin(GPIOC, ROT_SW_Pin) == 0) {// && lastState != state) {
			// Volume Mute Toggle
			keypresses[4][0] = 1;
		}
		else {
			keypresses[4][0] = 0;
		}

//		currentStateCLK = HAL_GPIO_ReadPin(GPIOC, ROT_CLCK_Pin);
//
//		// if CLK pin has changed, then the rotary encoder has turned
//		if (currentStateCLK != lastStateCLK) {// && currentStateCLK == 1) {
//			// if the DT state is different, then the encoder is rotating clockwise
//			currentStateDT = HAL_GPIO_ReadPin(GPIOC, ROT_DT_Pin);
//
//
//			if (currentStateDT != currentStateCLK) {
//				// Volume Up
//				add_keypress(keys[4][1]);
//				//keypresses[4][1] = 1;
//				//keypresses[4][2] = 0;
//
//			}
//
//			// otherwise, it is turning counter-clockwise
//			else {
//				// Volume Down
//				add_keypress(keys[4][2]);
//				//keypresses[4][1] = 0;
//				//keypresses[4][2] = 1;
//			}
//
//		}
//		else {
//			keypresses[4][1] = 0;
//			keypresses[4][2] = 0;
//		}
//
//		lastStateCLK = currentStateCLK;
		/* END Rotary Encoder */
	 }
}

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
