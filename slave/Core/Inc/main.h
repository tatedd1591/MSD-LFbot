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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void receivevalue(void);
void encoder(void);
float tocdoquay(uint32_t pos1,uint32_t pos2);
void PIDmotor(float rotate1,float rotate2);
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
#define PC13_Pin GPIO_PIN_13
#define PC13_GPIO_Port GPIOC
#define PWMA_Pin GPIO_PIN_0
#define PWMA_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_6
#define AIN2_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_7
#define AIN1_GPIO_Port GPIOA
#define STBY_Pin GPIO_PIN_0
#define STBY_GPIO_Port GPIOB
#define BIN1_Pin GPIO_PIN_1
#define BIN1_GPIO_Port GPIOB
#define BIN2_Pin GPIO_PIN_10
#define BIN2_GPIO_Port GPIOB
#define PWMB_Pin GPIO_PIN_11
#define PWMB_GPIO_Port GPIOB
#define ENA1_Pin GPIO_PIN_8
#define ENA1_GPIO_Port GPIOA
#define ENA2_Pin GPIO_PIN_9
#define ENA2_GPIO_Port GPIOA
#define ENB1_Pin GPIO_PIN_6
#define ENB1_GPIO_Port GPIOB
#define ENB2_Pin GPIO_PIN_7
#define ENB2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
