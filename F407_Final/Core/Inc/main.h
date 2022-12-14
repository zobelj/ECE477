/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define COL1_Pin GPIO_PIN_8
#define COL1_GPIO_Port GPIOD
#define COL2_Pin GPIO_PIN_9
#define COL2_GPIO_Port GPIOD
#define COL3_Pin GPIO_PIN_10
#define COL3_GPIO_Port GPIOD
#define COL4_Pin GPIO_PIN_11
#define COL4_GPIO_Port GPIOD
#define COL5_Pin GPIO_PIN_12
#define COL5_GPIO_Port GPIOD
#define ENC_A_Pin GPIO_PIN_13
#define ENC_A_GPIO_Port GPIOD
#define ENC_B_Pin GPIO_PIN_14
#define ENC_B_GPIO_Port GPIOD
#define ENC_SW_Pin GPIO_PIN_15
#define ENC_SW_GPIO_Port GPIOD
#define LCD1_DC_Pin GPIO_PIN_7
#define LCD1_DC_GPIO_Port GPIOC
#define LCD1_CS_Pin GPIO_PIN_8
#define LCD1_CS_GPIO_Port GPIOC
#define LCD2_DC_Pin GPIO_PIN_9
#define LCD2_DC_GPIO_Port GPIOC
#define LCD2_CS_Pin GPIO_PIN_8
#define LCD2_CS_GPIO_Port GPIOA
#define LCD2_RST_Pin GPIO_PIN_9
#define LCD2_RST_GPIO_Port GPIOA
#define LCD1_RST_Pin GPIO_PIN_10
#define LCD1_RST_GPIO_Port GPIOA
#define ROW0_Pin GPIO_PIN_0
#define ROW0_GPIO_Port GPIOD
#define ROW1_Pin GPIO_PIN_1
#define ROW1_GPIO_Port GPIOD
#define ROW2_Pin GPIO_PIN_2
#define ROW2_GPIO_Port GPIOD
#define ROW3_Pin GPIO_PIN_3
#define ROW3_GPIO_Port GPIOD
#define ROW4_Pin GPIO_PIN_4
#define ROW4_GPIO_Port GPIOD
#define ROW5_Pin GPIO_PIN_5
#define ROW5_GPIO_Port GPIOD
#define ROW6_Pin GPIO_PIN_6
#define ROW6_GPIO_Port GPIOD
#define COL0_Pin GPIO_PIN_7
#define COL0_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
