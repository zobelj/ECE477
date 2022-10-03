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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stm32f4xx_hal.h"
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
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

int counter;
int currentStateCLK;
int lastStateCLK;
int currentStateDT;
int debounceLock;
int state;
int lastState;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  // SCREEN 1
  ILI9341_Init();

   // Simple Text writing (Text, Font, X, Y, Color, BackColor)
   // Available Fonts are FONT1, FONT2, FONT3 and FONT4
   ILI9341_FillScreen(WHITE);
   ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
   ILI9341_DrawText("HELLO WORLD", FONT4, 90, 110, BLACK, WHITE);
   HAL_Delay(1000);

   //Writing numbers
   ILI9341_FillScreen(WHITE);
   static char BufferText[30];
   for(uint8_t i = 0; i <= 5; i++)
   {
     sprintf(BufferText, "COUNT : %d", i);
     ILI9341_DrawText(BufferText, FONT3, 10, 10, BLACK, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 30, BLUE, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 50, RED, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 70, GREEN, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 90, YELLOW, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 110, PURPLE, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 130, ORANGE, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 150, MAROON, WHITE);
     ILI9341_DrawText(BufferText, FONT3, 10, 170, WHITE, BLACK);
     ILI9341_DrawText(BufferText, FONT3, 10, 190, BLUE, BLACK);
   }

   // Horizontal Line (X, Y, Length, Color)
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawHLine(50, 120, 200, NAVY);
   HAL_Delay(1000);

   // Vertical Line (X, Y, Length, Color)
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawVLine(160, 40, 150, DARKGREEN);
   HAL_Delay(1000);

   // Hollow Circle (Centre X, Centre Y, Radius, Color)
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawHollowCircle(160, 120, 80, PINK);
   HAL_Delay(1000);

   // Filled Circle (Centre X, Centre Y, Radius, Color)
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawFilledCircle(160, 120, 50, CYAN);
   HAL_Delay(1000);

   // Filled Rectangle (Start X, Start Y, Length X, Length Y)
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawRectangle(50, 50, 220, 140, GREENYELLOW);
   HAL_Delay(1000);

   // Hollow Rectangle (Start X, Start Y, End X, End Y)
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawHollowRectangleCoord(50, 50, 270, 190, DARKCYAN);
   HAL_Delay(1000);

   // Simple Pixel Only (X, Y, Color)
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawPixel(100, 100, BLACK);
   HAL_Delay(1000);

   // Simple Text writing (Text, Font, X, Y, Color, BackColor)
   // Available Fonts are FONT1, FONT2, FONT3 and FONT4
   ILI9341_FillScreen(WHITE);
   ILI9341_DrawText("HELLO WORLD", FONT4, 90, 110, BLACK, WHITE);
//   HAL_Delay(10000);

   LCD_CS_PORT = LCD2_CS_PORT;
   LCD_CS_PIN = LCD2_CS_PIN;
   LCD_DC_PORT = LCD2_DC_PORT;
   LCD_DC_PIN = LCD2_DC_PIN;
   LCD_RST_PORT = LCD2_RST_PORT;
   LCD_RST_PIN = LCD2_RST_PIN;
   ILI9341_Init();
   ILI9341_FillScreen(WHITE);
   ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
   ILI9341_DrawText("HELLO WORLD", FONT4, 90, 110, BLACK, WHITE);

  // start the timer
  HAL_TIM_Base_Start_IT(&htim1);

  counter = 0;
  debounceLock = 0;
  state = 1;
  lastState = 1;

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 2-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PA4_Pin|PA5_Pin|PA6_Pin|PA7_Pin
                          |LCD2_CS_Pin|LCD2_RESET_Pin|LCD1_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, PE7_Pin|PE11_Pin|PE12_Pin|PE13_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD1_DC_Pin|LCD1_CS_Pin|LCD2_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0_Pin PC1_Pin PC2_Pin */
  GPIO_InitStruct.Pin = PC0_Pin|PC1_Pin|PC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0_Pin PA1_Pin PA2_Pin PA3_Pin */
  GPIO_InitStruct.Pin = PA0_Pin|PA1_Pin|PA2_Pin|PA3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4_Pin PA5_Pin PA6_Pin PA7_Pin
                           LCD2_CS_Pin LCD2_RESET_Pin LCD1_RESET_Pin */
  GPIO_InitStruct.Pin = PA4_Pin|PA5_Pin|PA6_Pin|PA7_Pin
                          |LCD2_CS_Pin|LCD2_RESET_Pin|LCD1_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7_Pin PE11_Pin PE12_Pin PE13_Pin */
  GPIO_InitStruct.Pin = PE7_Pin|PE11_Pin|PE12_Pin|PE13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD1_DC_Pin LCD1_CS_Pin LCD2_DC_Pin */
  GPIO_InitStruct.Pin = LCD1_DC_Pin|LCD1_CS_Pin|LCD2_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
int msg_index = 0;
uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };
const char keymap[] = "DCBA#9630852*741";
uint8_t hist[16];
uint8_t col;
char queue[2];
int qin;
int qout;

void drive_column(int c) {
    GPIOC->BSRR = 0xf00000 | (1 << (c + 4));
}

int read_rows() {
    return GPIOC->IDR & 0xf;
}

void push_queue(int n) {
    n = (n & 0xff) | 0x80;
    queue[qin] = n;
    qin ^= 1;
}

uint8_t pop_queue() {
    uint8_t tmp = queue[qout] & 0x7f;
    queue[qout] = 0;
    qout ^= 1;
    return tmp;
}

void update_history(int c, int rows) {
    for(int i = 0; i < 4; i++) {
        hist[4*c+i] = (hist[4*c+i]<<1) + ((rows>>i)&1);
        if (hist[4*c+i] == 1)
          push_queue(4*c+i);
    }
}

void init_tim6() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 48 - 1;
    TIM6->ARR = 1000 - 1;
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;
}

void TIM6_DAC_IRQHandler(void) {
    TIM6->SR &= ~TIM_SR_UIF;
    int rows = read_rows();
    update_history(col, rows);
    col = (col + 1) & 3;
    drive_column(col);
}

char get_keypress() {
    for(;;) {
        asm volatile ("wfi" : :);   // wait for an interrupt
        if (queue[qout] == 0)
            continue;
        return keymap[pop_queue()];
    }
}

void append_display(char val) {
    for (int i = 0; i < 7; i++) {
        msg[i] = (msg[i] & ~0xff) | (msg[i + 1] & 0xff);
    }
    msg[7] = (msg[7] & ~0xff) | val;
}


// Callback: timer has reset
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	// Check which version of the timer triggered this callback
	if (htim == &htim1) {
		// get the state of PC0
		lastState = state;
		state = HAL_GPIO_ReadPin(GPIOC, PC0_Pin);

		// if the state is low (default is high), turn toggle the LED
		// debounce the button press
		if (state == 0 && state != lastState) {
			HAL_GPIO_TogglePin(GPIOE, PE7_Pin);
			debounceLock++;

		    // turn off the rotate-LEDs
		  	HAL_GPIO_WritePin(GPIOE, PE11_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, PE13_Pin, GPIO_PIN_RESET);
		}
		else if (debounceLock >= 250) {
			debounceLock = 0;
		}
		else if (debounceLock) {
			debounceLock++;
		}

		// rotary encoder logic
		currentStateCLK = HAL_GPIO_ReadPin(GPIOC, PC1_Pin);

		// if CLK pin has changed, then the rotary encoder has turned
		if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
			// if the DT state is different, then the encoder is rotating clockwise
			currentStateDT = HAL_GPIO_ReadPin(GPIOC, PC2_Pin);
			if (currentStateDT != currentStateCLK) {
				// turn on the left LED (PE11)
				HAL_GPIO_WritePin(GPIOE, PE11_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, PE12_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE, PE13_Pin, GPIO_PIN_RESET);
				if (counter < 10) {
					counter++;
				}
			}
			// otherwise, it is turning counter-clockwise
			else {
				// turn on the right LED (PE13)
				HAL_GPIO_WritePin(GPIOE, PE11_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE, PE12_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOE, PE13_Pin, GPIO_PIN_SET);
				if (counter > -10) {
					counter--;
				}
			}
		}

		lastStateCLK = currentStateCLK;

		if(counter > 0) {
			HAL_GPIO_WritePin(GPIOE, PE12_Pin, GPIO_PIN_SET);
		}
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
