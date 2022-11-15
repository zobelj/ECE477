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
#include "layout.h"
#include "ILI9341_GFX.h"
#include "ILI9341_STM32_Driver.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEYCODE_LIMIT 6
#define GPIOEX_ADDR (0x20 << 1)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */

// WPM Counters
unsigned int charCount = 0;
unsigned int charsInCycle = 0;
unsigned int numCycles = 0;
unsigned int dryCycles = 0;
float wpm = 0;
int writeScreen = 1;

// row-column scanning
int keycodeNum = 1;
int row = 0;
int local_cols = 0;
int expander_cols = 0;
int expander_rot = 0; // 3 bits: switch, A, B
int exLastCLK = 0;
int exRotLock = 0;

// rotary direction
int currentStateCLK = 0;
int lastStateCLK = 0;
int currentStateDT = 0;
int rotLock = 0;

// USBD struct
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
static void MX_TIM7_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
// local keypad scanning
void scan_keypad();
void set_rows();
void get_cols();

// rotary encoder scanning
void scan_rotary();

// usb functions
void add_keypress(uint16_t key);
void record_keys();

// LCD functions
void switch_lcd();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Local Keypad Scanning */
void scan_keypad() {
  set_rows();

  // update the global variables for col values
  get_cols();

  // scan local cols
  for(int i = 0; i < nCols1; i++) {
    if(local_cols & (1 << i)) {
    	if (keypresses1[row][i] != 1) {
    		keypresses1[row][i] = 1;
    	    // WPM timer not enabled
//    	    if (!(TIM1->CR1 && TIM_CR1_CEN)) {
//    	    	HAL_TIM_Base_Start_IT(&htim7);
//    	    }
//    	    charCount++;
//    	    charsInCycle++;
    	}
    }
    else {
      keypresses1[row][i] = 0;
    }
  }

  // scan expander cols
  for(int i = 0; i < nCols2; i++) {
    if(expander_cols & (1 << i)) {
      if (keypresses2[row][i] != 1) {
    	  keypresses2[row][i] = 1;
    	  // WPM timer not enabled
    	  if (!(TIM1->CR1 && TIM_CR1_CEN)) {
    		  HAL_TIM_Base_Start_IT(&htim7);
    	  }
    	  charCount++;
    	  charsInCycle++;
      }
    }
    else {
      keypresses2[row][i] = 0;
    }

  }

}

void set_rows() {
  // update row value
  row = (row + 1) % nRows1;
  HAL_GPIO_WritePin(GPIOD, ROW0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW5_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, ROW6_Pin, GPIO_PIN_SET);

  // use global variable row to set the correct row to low and last row back to high
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
    case 4:
      HAL_GPIO_WritePin(GPIOD, ROW4_Pin, GPIO_PIN_RESET);
      break;
    case 5:
	  HAL_GPIO_WritePin(GPIOD, ROW5_Pin, GPIO_PIN_RESET);
	  break;
    case 6:
	  HAL_GPIO_WritePin(GPIOD, ROW6_Pin, GPIO_PIN_RESET);
	  break;
  }

  // ! GPIOB2 - GPIOB7 maps to row0 - row5, GPIOA7 maps to row6
  if (row != 6){
	  uint8_t data[3] = {0x14, ~( 0x04 << row ), ~0}; // addr 0x14 for IOCON.BANK = 0, 0x0A for IOCON.BANK = 1
	  HAL_I2C_Master_Transmit(&hi2c2, GPIOEX_ADDR, data, 2, 1000);
  } else {
	  uint8_t data[3] = {0x14, ~0, ~0x80}; // addr 0x14 for IOCON.BANK = 0, 0x0A for IOCON.BANK = 1
	  HAL_I2C_Master_Transmit(&hi2c2, GPIOEX_ADDR, data, 2, 1000);
  }
}

void get_cols() {
  // get the value of all columns as one int from the GPIO register
  // PD7-4
  local_cols = ~(GPIOD->IDR >> 7) & 0x3F;

  // read the GPIO expander columns
  // ! GPIOA0 - GPIOA5 maps to col0 - col5
  uint8_t data[2] = {0x12, 0x00}; // addr 0x12 for IOCON.BANK = 0, 0x09 for IOCON.BANK = 1
  HAL_I2C_Master_Transmit(&hi2c2, GPIOEX_ADDR, data, 1, 1000);
  HAL_I2C_Master_Receive(&hi2c2, GPIOEX_ADDR, data, 2, 1000);

  expander_cols = data[0] & 0xC0;

  expander_rot = (data[0] & 0x20 << 2) | (data[1] & 0x03);
}

/* Rotary Encoder Scanning */
void scan_rotary() {
  currentStateCLK = HAL_GPIO_ReadPin(GPIOD, ENC_A_Pin);

  // if CLK pin has changed, then the rotary encoder has turned
  if (currentStateCLK != lastStateCLK && rotLock == 0 ) {// && rotLock == 0) {
    // if the DT state is different, then the encoder is rotating counter-clockwise
    currentStateDT = HAL_GPIO_ReadPin(GPIOD, ENC_B_Pin);

    if (currentStateDT != currentStateCLK) {
      // Volume Down
      rotary_keypresses1[1] = 1;
    }

    // otherwise, it is turning clockwise
    else if (currentStateDT == currentStateCLK) {
      // Volume Up
      rotary_keypresses1[2] = 1;
    }
    rotLock++;

  }
  else if (rotLock == 0) {
	  rotary_keypresses1[1] = 0;
	  rotary_keypresses1[2] = 0;
  }

  lastStateCLK = currentStateCLK;

  // rotLock allows the rotary encoder's inputs to settle over a few extra clock cycles
  if(rotLock != 0) {
    rotLock = (rotLock + 1) % 150;
  }

  // if the state is low (default is high), turn toggle the LED
  if (HAL_GPIO_ReadPin(GPIOD, ENC_SW_Pin) == 0) {
    // Volume Mute Toggle
	  rotary_keypresses1[0] = 1;
  }
  else {
	  rotary_keypresses1[0] = 0;
  }

  // Expander rotary encoder
  int exCLK = (expander_rot & 0x02) >> 1;
  int exDT  = (expander_rot & 0x01);
  int exSW  = (expander_rot & 0x04) >> 2;

  // if CLK pin has changed, then the rotary encoder has turned
  if (exCLK != exLastCLK && exRotLock == 0 ) {// && rotLock == 0) {
	  // if the DT state is different, then the encoder is rotating counter-clockwise

	  if (exDT != exCLK) {
		// Volume Down
		rotary_keypresses2[1] = 1;
	  }

	  // otherwise, it is turning clockwise
	  else if (exDT == exCLK) {
		// Volume Up
		rotary_keypresses2[2] = 1;
	  }
	  exRotLock++;

  }
  else if (exRotLock == 0) {
	  rotary_keypresses2[1] = 0;
	  rotary_keypresses2[2] = 0;
  }

  exLastCLK = exCLK;

  if(exRotLock != 0) {
      exRotLock = (exRotLock + 1) % 150;
  }

  if(exSW)
	  rotary_keypresses2[0] = 1;
  else
	  rotary_keypresses2[0] = 0;


}
/* END Rotary Encoder Scanning */

/* USB Functions */
void record_keys() {
  // reset keyboardhid to 0
  memset(&keyboardhid, 0, sizeof(keyboardHID));
  keycodeNum = 1;

  // Add local keypresses
  for(int i = 0; i < nRows1; i++) {
	  for(int j = 0; j < nCols1; j++) {
		  if(keypresses1[i][j] == 1) {
			  add_keypress(layout1[i][j]);
	    }
	  }
  }

  // Add GPIO Expander keypresses
  for(int i = 0; i < nRows2; i++) {
	  for(int j = 0; j < nCols2; j++) {
		  if(keypresses2[i][j] == 1) {
			  add_keypress(layout2[i][j]);
		  }
	  }
  }

  // Add Rotary Encoder keypresses
  for(int i = 0; i < 4; i++) {
	  if(rotary_keypresses1[i] == 1)
		  add_keypress(rotary_keys1[i]);
  }

  // Add Expander Rotary Encoder
  for(int i = 0; i < 4; i++) {
	  if(rotary_keypresses2[i] == 1)
		  add_keypress(rotary_keys2[i]);
   }

}

void add_keypress(uint16_t key) {

	if( (key & 0xFF00) == 0xFF00) {
		int shift = key & 0xFF;
		keyboardhid.MODIFIER |= (1<<shift);

		return;
	}

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
		default:
			break;
	}
	keycodeNum++;

}
/* END USB Functions */

/* LCD Functions */
void switch_lcd() {
  // switch LCD_*_PIN and LCD_*_PORT between LCD1_* and LCD2_*
  if(LCD_DC_PIN == LCD2_DC_PIN) {
	LCD_CS_PORT = LCD1_CS_PORT;
	LCD_CS_PIN = LCD1_CS_PIN;
	LCD_DC_PORT = LCD1_DC_PORT;
	LCD_DC_PIN = LCD1_DC_PIN;
	LCD_RST_PORT = LCD1_RST_PORT;
	LCD_RST_PIN = LCD1_RST_PIN;
  }
  else {
	LCD_CS_PORT = LCD2_CS_PORT;
	LCD_CS_PIN = LCD2_CS_PIN;
	LCD_DC_PORT = LCD2_DC_PORT;
	LCD_DC_PIN = LCD2_DC_PIN;
	LCD_RST_PORT = LCD2_RST_PORT;
	LCD_RST_PIN = LCD2_RST_PIN;
  }
}
/* END LCD Functions */


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
  MX_USB_DEVICE_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  // Initialize the LCDs
  // Left Screen
  char writeBuff[20];
  ILI9341_Init();
  ILI9341_SetRotation(SCREEN_VERTICAL_1);
  ILI9341_FillScreen(BLUE);
  sprintf(writeBuff, "Number of turns: ");
  ILI9341_DrawText(writeBuff, FONT6, 25, 110, BLACK, WHITE);

  // Right Screen
  switch_lcd();
  ILI9341_Init();
  ILI9341_SetRotation(SCREEN_VERTICAL_1);
  ILI9341_FillScreen(BLUE);
  sprintf(writeBuff, "Words");
  ILI9341_DrawText(writeBuff, FONT5, 25, 85, BLACK, WHITE);
  sprintf(writeBuff, "Per");
  ILI9341_DrawText(writeBuff, FONT5, 25, 135, BLACK, WHITE);
  sprintf(writeBuff, "Minute:");
  ILI9341_DrawText(writeBuff, FONT5, 25, 185, BLACK, WHITE);

  // start the timer interrupt
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start_IT(&htim6);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  char buffer1[10];
  char buffer2[10];

  while (1)
  {
	  //draw the counter to the lcd
	  if (writeScreen) {
		  sprintf(buffer1, "%-3d", (int)wpm);
	  	  ILI9341_DrawText(buffer1, FONT5, 	165, 185, BLACK, WHITE);
	  	  writeScreen = 0;
	  }

	  switch_lcd();

	  // draw the counter to the lcd
	  sprintf(buffer2, "%d", 2);
	  ILI9341_DrawText(buffer2, FONT6, 190, 110, BLACK, WHITE);

	  switch_lcd();

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
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  hi2c2.Init.ClockSpeed = 400000;
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
  /* USER CODE BEGIN I2C2_Init 2 */

  // Set Rows to outputs, cols to inputs
  // Reverse polarity of the input pins since we are using active low
  // 1: input/reverse polarity
  // 0: output/regular polarity
  // GPIOA: 0111 1111
  // GPIOB: 0000 0011
  uint8_t data[5] = {0x00, 0x7F, 0x03, 0x7F, 0x03}; // addr 0x00 with data 0x7F
  HAL_I2C_Master_Transmit(&hi2c2, GPIOEX_ADDR, data, 4, 1000);


  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  htim6.Init.Period = 20-1;
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
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 36000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 4000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD1_DC_Pin|LCD1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ROW0_Pin|ROW1_Pin|ROW2_Pin|ROW3_Pin
                          |ROW4_Pin|ROW5_Pin|ROW6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : COL1_Pin COL2_Pin COL3_Pin COL4_Pin
                           COL5_Pin ENC_A_Pin ENC_B_Pin ENC_SW_Pin
                           COL0_Pin */
  GPIO_InitStruct.Pin = COL1_Pin|COL2_Pin|COL3_Pin|COL4_Pin
                          |COL5_Pin|ENC_A_Pin|ENC_B_Pin|ENC_SW_Pin
                          |COL0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD1_DC_Pin LCD1_CS_Pin */
  GPIO_InitStruct.Pin = LCD1_DC_Pin|LCD1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW0_Pin ROW1_Pin ROW2_Pin ROW3_Pin
                           ROW4_Pin ROW5_Pin ROW6_Pin */
  GPIO_InitStruct.Pin = ROW0_Pin|ROW1_Pin|ROW2_Pin|ROW3_Pin
                          |ROW4_Pin|ROW5_Pin|ROW6_Pin;
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
	else if (htim == &htim6) {

		/* Row-Col Scanning */
		scan_keypad();

		/* Rotary Encoder */
		scan_rotary();
	}

	else if (htim == &htim7) {
//		// words per minute
//		// check to see if any keys pressed
		if (charsInCycle == 0) {
			dryCycles++;
			// shut down if 5 cycles with no presses
			if (dryCycles == 5) {
				charCount = 0;
				charsInCycle = 0;
				dryCycles = 0;
				numCycles = 0;
				HAL_TIM_Base_Stop_IT(&htim7);
			}
		}
		// calculate wpm
		numCycles++;
		wpm = (charCount / 5.0f) / ((2.0f * numCycles) / 60.0f);
		charsInCycle = 0;
		writeScreen = 1;

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
