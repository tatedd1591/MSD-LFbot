/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : master
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
#include "HC_SR04.h"
#include "stdio.h"
#include <math.h>

/* USER CODE BEGIN Includes */

//sensor value
//tcrt5000 value
uint16_t u16_ADCScanVal[5],ADCValue[5],calib[5];
uint8_t digital[5],pre_digital[5]; 
uint8_t i; //bien dem vi tri sensor
uint16_t sum=0; //tong gia tri adc
uint16_t threshold=3000; //nguong doc line den
uint8_t limit_pos=34; //gioi han sai so vi tri
uint8_t limit_left=0,limit_right=0; //gioi han cam bien do line
//HC-SR04 value
uint8_t distance=255; //bien luu gia tri khoang cach
uint8_t limit_obj=100; //gioi han cam bien phat hien vat

//position value
float weight; //trong so
float slope=0.3959; //do doc gia tri
float intercept=-0.7642; //giao diem truc hoanh
float pos=0; //gia tri vi tri tinh toan

//input value
float vR=400; //van toc dai mong muon (mm/s)
float wR; //van toc goc mong muon (rad/s)
float tsamp=0.01; //thoi gian lay mau (s)
//PID
float kp=0.0504;
float ki=0.1;
float kd=0.000229; 
float b=195; //khoang cach tam 2 banh (mm)
float D=85; //duong kinh banh xe (mm)
float L=180; //khoang cach tu truc dong co den mat cam bien
uint16_t ymax=3822;//gia tri lon nhat cam bien tren line den
uint16_t ymin=1209; //gia tri nho nhat cam bien tren nen trang

//calculate value
float pre_e2=0,e2=0; //sai so e2
float inte2=0; //khau tich phan
float vleft=0,vright=0,wleft=0,wright=0; //van toc banh trai va banh phai
float v=0,w=0; //van toc dai va van toc goc

//save value
uint8_t point=0; //bien luu gia tri cac case
uint8_t rotate=0; //phat hien da xoay tai cho (phat hien nga 3)
uint16_t count=0; //bien dem
uint8_t obj=0; //bien phat hien vat
uint8_t busy=0; //trang thai ban
uint8_t dist[100]; //luu du lieu khoang cach
uint16_t n=0; //bien dem

//transmit value
uint8_t txBuffer[4]; //mang truyen du lieu
uint8_t dem=0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief Timer interrupt after 0.01s
  * @retval None
  */
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim2.Instance)
	{
		read_ADC();						
		position();   				
		transmit_value();				
	}
}
/* USER CODE END 0 */

/**
  * @brief Transmit value using SPI interface
  * @retval None
  */
void transmit_value(void)
{
	if (wleft >255) wleft=255;
	else if (wleft<0) wleft=0;
	if (wright > 255) wright=255;
	else if (wright<0) wright=0;
	txBuffer[0] = wleft ;
	txBuffer[1] = wright;
	if (pos<0)	txBuffer[2] = -pos;
	else	txBuffer[2]=pos+100;
	spi_enable;			
	HAL_SPI_Transmit_IT(&hspi1,txBuffer,4);	
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance==hspi1.Instance)
	{
		spi_disable;
	}
}

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
  MX_ADC1_Init();
	TimerInit();
  MX_SPI1_Init();

  /* Infinite loop */
  while (1) {}
  /* USER CODE END 3 */
}

/**
  * @brief Reading TCRT5000 value
  * @retval uint16_t
  */
void read_ADC(void)
{
	//trai qua phai: 0->4
	sum=0;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)u16_ADCScanVal,5);	
	for (i=0;i<5;i++)
	{
		pre_digital[i]=digital[i];
		ADCValue[i]=u16_ADCScanVal[i];
		if (ADCValue[i] > threshold) digital[i]=1;
		else digital[i]=0;
	}		
	calib[0] = ymin + 0.9332*(ADCValue[0]-1008) ;
	calib[1] = ymin + 1.1840*(ADCValue[1]-1663) ;
	calib[2] = ymin + 0.9977*(ADCValue[2]-1178) ;
	calib[3] = ymin + 0.9266*(ADCValue[3]-990) ;
	calib[4] = ymin + 0.9969*(ADCValue[4]-1206) ;	
}

/**
  * @brief Reading position of linefollow robot
  * @unit mm
  * @retval float
  */
void read_position(void)
{
	limit_left=0;
	limit_right=0;
	sum=calib[0]+calib[1]+calib[2]+calib[3]+calib[4];
	weight=17*(2*(calib[0]-calib[4])+(calib[1]-calib[3]))/sum; //find weight	
	pos = (weight-intercept)/slope; // position calculate
}

/**
  * @brief Calculate error using PID and get angular speed value
  * @unit RPM
  * @retval float
  */
void calculate_error(void)
{
	pre_e2=e2;
	e2=pos;
	inte2=inte2+ (e2+pre_e2)*tsamp;	
  w = kp*e2 + kd*(e2-pre_e2)/tsamp + ki*inte2;
  vleft = 0.5*(2*vR - w*b);
  vright = 0.5*(2*vR + w*b);
	wleft=(vleft*60*2/D)/(2*3.14);
	wright=(vright*60*2/D)/(2*3.14);
}

/**
  * @brief identify of case
  * @retval none
  */
void position(void)
{
	//trang thai ban khi gap nga ba lan dau
	if (busy==0) //xet truong hop o trang thai khong ban 
	{
		if ( ( (digital[0]==1) || (digital[1]==1) ) && ( (digital[3]==1) || (digital[4]==1) ) ) 
		{
			if (rotate==0) point=1;
			else point=2;
		}
		else if ( (digital[0]==0) && (digital[1]==0) && (digital[2]==0) && (digital[3]==0) && (digital[4]==0)) point=3;
		else point=0;
	}
	
	switch (point)
	{
		case 0: //che do do line binh thuong
		{			
			read_position();
			calculate_error();
			txBuffer[3] = 0;				
			break;	
		}
		case 1: //phat hien nga 3 lan dau tien, quet vat can
		{
			busy=1; //xet co trang thai ban
			//cac tien trinh quet vat can
			if (count<=110) count++;								
			if (count<10)	txBuffer[3] = 3;	//dung trong 0.1s
			if (count>=10 && count<70) //xoay nguoc chieu tai cho trong 0.6s, dong thoi doc cam bien sieu am de phat hien v?t
			{
				txBuffer[3] = 1;
				if (count%6==4)	{ distance=HCSR04_GetDis();	dist[n++]=distance;}							
				if (distance<100&& distance >60)	obj=1;	//co phat hien vat set len 1 khi thay vat can, ko thay se bang 0	
			}
			if (count>=70 && count<80)	txBuffer[3] = 3;	//dung trong 0.1s
			if (count>=80) //sau khi quet vat can
			{
				if (obj==1) //thay vat can, di theo nhanh ben phai
				{
					if (digital[0]==1 && (digital[3]==1 || digital[4]==1)) //roi khoi case 1 khi thoa dieu kien, ko thoa xoay tai cho cung chieu cho den khi thoa dieu kien
					{
						rotate=1; 
						busy=0;	
					}
					else  txBuffer[3] = 2;
				}
				else //di theo nhanh ben trai, tuong tu
				{
					if (digital[1]==1)
					{
						rotate=1; 
						busy=0;	
					}
					else txBuffer[3] = 2;
				}												
			}				
			break;
		}		
		case 2: //phat hien nga ba lan tiep theo
		{
			if (obj==1)
			{
				calib[0]=ymin;
				calib[1]=ymin;
			}
			else
			{
				calib[3]=ymin;
				calib[4]=ymin;
			}
			read_position();
			calculate_error();
			txBuffer[3] = 0;
			break;
		}
		case 3: //cam bien roi khoi duong line
		{	
			if ((pre_digital[0]!=1) && (pre_digital[4]!=1) && limit_left==0 && limit_right==0) //neu tin hieu digital truoc do khong doc line den o 2 mat ngoai cung
			{
				txBuffer[3] = 3;
				break;
			}
			else 
			{
				if (pre_digital[0]==1 || limit_left==1) 
				{
					pos=limit_pos;
					limit_left=1;
				}
				else if (pre_digital[4]==1 || limit_right==1) 
				{
					pos=-limit_pos;
					limit_right=1;
				}
				calculate_error();
				txBuffer[3] = 0;
				break;
			}
		}						
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

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
