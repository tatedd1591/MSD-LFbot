/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : HC_SR04.c
  * @brief          : Ultrasonic Sensor HC-SR04
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "HC_SR04.h"

extern TIM_HandleTypeDef htim1;

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;  // kiem tra da doc ngat chua
uint16_t Distance  = 999;

/**
  * @brief Reading HC-SR04 using Input capture compare interrupt
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // kiem tra interrupt channel1
	{
		if (Is_First_Captured==0) //dieu kien gia tri dau chua vao trang thai capture (chua doc ngat)
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // doc gia tri dau tuong ung ngat suon len
			Is_First_Captured = 1;  // xac nhan co capture (ung voi luc suon len cua echo)
			// thay doi trang thai ngat luc suon xuong
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (Is_First_Captured==1)   // da vao trang thai capture
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // doc trang thai khi ngat suon xuong
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset lai bien dem
			
				Difference = IC_Val2-IC_Val1;
			
			Distance = Difference * 0.034/2; //cong thuc tinh k/c
			Is_First_Captured = 0; // dat lai trang thai chua co ngat
			// chinh lai uu tien ngat suon len
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1); //xoa ngat input timer
		}
	}
}

/**
  * @brief delay
  * @param counter
  * @unit us
  * @retval uint16_t
  */
void delay(uint16_t time){
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while(__HAL_TIM_GET_COUNTER(&htim1) < time);
}

/**
  * @brief Reading distance
  * @unit cm
  * @retval uint16_t
  */
uint16_t HCSR04_GetDis (void)
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // kich chan TRIG len muc HIGH trong 10us
	delay(10);  
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // keo xuong muc LOW
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	return Distance;
}




