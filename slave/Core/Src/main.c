/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Slave
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"
#include "tim.h"
#include "pwm.h"

/* Private variables ---------------------------------------------------------*/
//receive transmit
uint8_t RxBuffer[4];
uint8_t i;

//input value
float REF1;
float REF2;
uint8_t spcase=0; //cac TH dieu khien dong co

//encoder motor value
uint32_t pre_pos1=0,pos1=0; //motor1
uint32_t pre_pos2=0,pos2=0; //motor2
float w1=0,w2=0; //van toc goc hien tai cua motor 1 và 2

//PID value
float tpid=0.01; //thoi gian lay mau
//motor 1 (right)
float Kp1=0.655, Ki1=21.858,kd1=0;
float error1=0,pre_e1=0; //sai so so voi gia tri mong muon
float ipart1=0; //khau tich phan
float pidval1=0;
//motor2 (left)
float Kp2=0.6176, Ki2=20.8714 , kd2=0;
float error2=0,pre_e2=0;//sai so so voi gia tri mong muon
float ipart2=0; //khau tich phan
float pidval2=0;

//output value
float pwm1=0,pwm2=0; //xung can cap cho 2 dong co

//reading value
float result1[2000],result2[2000];
int8_t result3[2000];
int8_t pos_val;
uint16_t j=0; //bien dem luu du lieu

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  TimerInit();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {}
  /* USER CODE END 3 */
}
/**
  * @brief Receive value using SPI interface
  * @unit RPM
  * @retval uint8_t
  */
void receivevalue(void)
{
	HAL_SPI_Receive_IT(&hspi1,RxBuffer,4);		
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance==hspi1.Instance)
	{
		REF1 = RxBuffer[1];
		REF2 = RxBuffer[0];
		if (RxBuffer[2]>=100) pos_val=RxBuffer[2]-100;
		else pos_val=-RxBuffer[2];
		spcase=RxBuffer[3];
	}
}

/**
  * @brief Reading encoder motor
  * @retval uint16_t
  */
void encoder(void)
{
	pre_pos1=pos1;
	pre_pos2=pos2;
  pos1=__HAL_TIM_GetCounter(&htim1);
	pos2=__HAL_TIM_GetCounter(&htim4);
}

/**
  * @brief Reading angular speed
  * @param previous position and current position of encoder
  * @unit RPM
  * @retval float
  */
float tocdoquay(uint32_t pre_pos,uint32_t pos)
{	
	float w;		
	if (pos>pre_pos) 
	{
		if (pos-pre_pos>30000) w=(65535-pos+pre_pos)*60/(1570*tpid);
		else w=(pos-pre_pos)*60/(1570*tpid);
	}
	else if (pre_pos==pos) w=0;
	else 
	{
		if (pre_pos-pos>30000) w=(65535-pre_pos+pos)*60/(1570*tpid);
		else w=(pre_pos-pos)*60/(1570*tpid);
	}
  return w;
}

/**
  * @brief Calculate PID speed value to control 2 motor
  * @unit Speed percentage
  * @retval uint16_t 
  */
void PIDmotor(float rotate1,float rotate2)
{
	//PID motor1
	pre_e1=error1;
	error1=rotate1-w1;	
	ipart1=ipart1+error1*tpid;
	pidval1 = Kp1*error1+Ki1*ipart1+kd1*(error1-pre_e1)/tpid;
	pwm1=pidval1*10;
	if (pwm1>900) pwm1=900;
	//PID motor2
	pre_e2=error2;
	error2=rotate2-w2;
	ipart2=ipart2+error2*tpid;
	pidval2 = Kp2*error1+Ki2*ipart2+kd2*(error2-pre_e2)/tpid;
	pwm2=pidval2*10;
	if (pwm2>900) pwm2=900;
}

/**
  * @brief Timer Interrupt after 0.01s
  * @retval None
  */
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance==TIM3)
	{	
		HAL_GPIO_TogglePin(PC13_GPIO_Port,PC13_Pin);
		receivevalue();
		encoder();					
		w1= tocdoquay(pre_pos1,pos1);
		w2= tocdoquay(pre_pos2,pos2);
		if (spcase==2)
		{
			PIDmotor(30,30);
			RotateCC(pwm1,pwm2);
		}
		else if (spcase==1)
		{
			PIDmotor(30,30);
			Rotate(pwm1,pwm2);
		}
		else if (spcase==3)
		{
			PIDmotor(0,0);
			Forward(pwm1,pwm2);
		}
		else
		{			
			
			//read motor speed
			if (j<2000)
			{
				j++;
				result1[j]=w1;
				result2[j]=w2;
				result3[j]=pos_val;
			}
			PIDmotor(REF1,REF2);												
			//control motor					
			Forward(pwm1,pwm2);
		}			
	}
}
/* USER CODE END 4 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/* USER CODE BEGIN 4 */

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
