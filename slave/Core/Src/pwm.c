/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : pwm.c
  * @brief          : Control motor
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
	*note: motor
	*motor 1:
	PA0: xung
	PA6: IN1 -> HIGH: tien
	PA7: IN2 -> HIGH: lùi
	encoder: tim1
	*motor 2:
	PB11: xung
	PB1: IN4 -> HIGH: tien
	PB10: IN3 -> HIGH: lùi
	encoder: tim4
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/	
#include "pwm.h"

extern TIM_HandleTypeDef htim2;

/**
  * @brief  Set speed for motor1.
  * @param  Speed percentage
  * @retval None
  */

void Speed1_Forward(uint16_t speed)
{	
	htim2.Instance->CCR1 = speed;
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
}

void Speed1_Backward(uint16_t speed)
{
	htim2.Instance->CCR1 = speed;
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
}

void Speed1_Stop(void)
{
	htim2.Instance->CCR1 = 0;
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
}

/**
  * @brief  Set speed for motor2.
  * @param  Speed percentage
  * @retval None
  */

void Speed2_Forward(uint16_t speed)
{
	htim2.Instance->CCR4 = speed;
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
}
void Speed2_Backward(uint16_t speed)
{
	htim2.Instance->CCR4 = speed;
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
}

void Speed2_Stop(void)
{
	htim2.Instance->CCR4 = 0;
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
}

/**
  * @brief  Set speed to control motor.
  * @param  Speed percentage
  * @retval None
  */
void Stop(void) //dung
{
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_RESET);
}
void Forward(uint16_t speed1,uint16_t speed2) //tien
{
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	Speed1_Forward(speed1);
	Speed2_Forward(speed2);
}

void RotateCC(uint16_t speed1,uint16_t speed2) //xoay tai cho nguoc chieu
{
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	Speed2_Forward(speed2);
	Speed1_Backward(speed1);
}
void Rotate(uint16_t speed1,uint16_t speed2) //xoay tai cho cung chieu
{
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	Speed1_Forward(speed1);
	Speed2_Backward(speed2);
}



