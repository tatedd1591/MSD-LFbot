/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : pwm.h
  * @brief          : Header for pwm.c file.
  *                   This file contains the common defines of the application.
	* @author         : Duong Le Tuan Anh
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Speed1_Forward(uint16_t speed);
void Speed2_Forward(uint16_t speed);
void Speed1_Backward(uint16_t speed);
void Speed2_Backward(uint16_t speed);
void Speed1_Stop(void);
void Speed2_Stop(void);
void Forward(uint16_t speed1,uint16_t speed2);
void Rotate(uint16_t speed1,uint16_t speed2);
void RotateCC(uint16_t speed1,uint16_t speed2);
void Stop(void);
void TimerInit(void);
void TimerStop(void);
uint16_t wtopwm(float w);

