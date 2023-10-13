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
#define SPI_SS_Pin GPIO_PIN_15
#define SPI_SS_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

void read_ADC(void);
void read_position(void);
void calculate_error(void);
void position(void);
void transmit_value(void);
void transmit_position(void);
/* USER CODE END Private defines */
#define spi_enable HAL_GPIO_WritePin(SPI_SS_GPIO_Port,SPI_SS_Pin,GPIO_PIN_RESET);
#define spi_disable HAL_GPIO_WritePin(SPI_SS_GPIO_Port,SPI_SS_Pin,GPIO_PIN_SET);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
