/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : HC_SR04.h
  * @brief          : Header for HC_SR04.c file.
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

#ifndef INC_HC_SR04_H_
#define INC_HC_SR04_H_

#include "stm32f1xx_hal.h"
#define TRIG_PIN GPIO_PIN_9
#define TRIG_PORT GPIOA


void delay(uint16_t time);
uint16_t HCSR04_GetDis (void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
uint16_t adcValue(uint16_t a);
#endif /* INC_HC_SR04_H_ */
