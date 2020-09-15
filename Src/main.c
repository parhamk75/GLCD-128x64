/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GLCD_L0.h"
#include "GLCD_L1.h"
#include "GLCD_L2.h"
#include "stdio.h"
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

/* USER CODE BEGIN PV */
GLCD_L0_TypeDef tmp_glcd;


// Temporary Variables for Debugging
char tmp_str[] = "12345678901234567890";
uint8_t tmp_cntr = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void tmp_wait_for_busy(GLCD_L0_HALF_TypeDef hlf_)
{
  while(GLCD_L2_IsDispBusy(&tmp_glcd, hlf_) == GLCD_L2_DispStatBusy_Busy)
  {
    HAL_UART_Transmit_IT(&huart2, (uint8_t*)"Hi\r\n", 4);
  }
}
void tmp_check_status(void)
{
  sprintf(tmp_str, "R%02X\r\n", GLCD_L2_ReadDispStatus(&tmp_glcd, GLCD_L2_HALF_Right));
  HAL_UART_Transmit(&huart2, (uint8_t*)tmp_str, 5, 200);
  sprintf(tmp_str, "L%02X\r\n", GLCD_L2_ReadDispStatus(&tmp_glcd, GLCD_L2_HALF_Left));
  HAL_UART_Transmit(&huart2, (uint8_t*)tmp_str, 5, 200);
}
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  tmp_glcd.DB_Ports[0] = LCD_D0_GPIO_Port;
  tmp_glcd.DB_Ports[1] = LCD_D1_GPIO_Port;
  tmp_glcd.DB_Ports[2] = LCD_D2_GPIO_Port;
  tmp_glcd.DB_Ports[3] = LCD_D3_GPIO_Port;
  tmp_glcd.DB_Ports[4] = LCD_D4_GPIO_Port;
  tmp_glcd.DB_Ports[5] = LCD_D5_GPIO_Port;
  tmp_glcd.DB_Ports[6] = LCD_D6_GPIO_Port;
  tmp_glcd.DB_Ports[7] = LCD_D7_GPIO_Port;

  tmp_glcd.DB_Pins[0] = LCD_D0_Pin;
  tmp_glcd.DB_Pins[1] = LCD_D1_Pin;
  tmp_glcd.DB_Pins[2] = LCD_D2_Pin;
  tmp_glcd.DB_Pins[3] = LCD_D3_Pin;
  tmp_glcd.DB_Pins[4] = LCD_D4_Pin;
  tmp_glcd.DB_Pins[5] = LCD_D5_Pin;
  tmp_glcd.DB_Pins[6] = LCD_D6_Pin;
  tmp_glcd.DB_Pins[7] = LCD_D7_Pin;

  tmp_glcd.CS1_Port = LCD_CS1_GPIO_Port;
  tmp_glcd.CS1_Pin  = LCD_CS1_Pin;
  
  tmp_glcd.CS2_Port = LCD_CS2_GPIO_Port;
  tmp_glcd.CS2_Pin  = LCD_CS2_Pin;
  
  tmp_glcd.RS_Port  = LCD_RS_GPIO_Port;
  tmp_glcd.RS_Pin   = LCD_RS_Pin;
  
  tmp_glcd.RW_Port  = LCD_RW_GPIO_Port;
  tmp_glcd.RW_Pin   = LCD_RW_Pin;
  
  tmp_glcd.EN_Port  = LCD_EN_GPIO_Port;
  tmp_glcd.EN_Pin   = LCD_EN_Pin;
  
  tmp_glcd.RST_Port = LCD_RST_GPIO_Port;
  tmp_glcd.RST_Pin  = LCD_RST_Pin;

  tmp_glcd.Mode     = GLCD_L0_Mode_Write;

  // Some Tests for GLCD_L0
  HAL_UART_Transmit_IT(&huart2, (uint8_t*)"LCD Init Finished\r\n", 19);
  
  // Perform a Reset procedure
  GLCD_L2_FullInit(&tmp_glcd, GLCD_L2_DispColor_Black);

  // Turning on and off and read the Status Reg
  GLCD_L1_Disp_OnOff(&tmp_glcd, GLCD_L0_HALF_Right, GLCD_L1_Disp_On); // LCD On
  GLCD_L1_Disp_OnOff(&tmp_glcd, GLCD_L0_HALF_Left, GLCD_L1_Disp_Off); // LCD Off
  tmp_check_status();
  GLCD_L1_Disp_OnOff(&tmp_glcd, GLCD_L0_HALF_Left, GLCD_L1_Disp_On); // LCD On
  HAL_Delay(1000);

  tmp_check_status();

  // Ser Address Regs
  GLCD_L2_GotoXYZ(&tmp_glcd, GLCD_L2_HALF_Both, 0, 0, 0);

  for (size_t j = 0; j < 8; j++)
  {
    for (size_t i = 0; i < 64; i++)
    {
      GLCD_L2_WriteByteXYZ(&tmp_glcd, GLCD_L2_HALF_Both, (uint8_t)(0x55), j, i, i>>1);
      HAL_Delay(45);
    }
  }
  HAL_Delay(2000);
  GLCD_L2_ClearDisplay(&tmp_glcd, GLCD_L2_HALF_Both);
  HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    tmp_cntr++;
    GLCD_L2_GotoZ(&tmp_glcd, GLCD_L2_HALF_Both, tmp_cntr%64);
    // Show image on LCD
    for (size_t j = 0; j < 4; j++)
    {  
      GLCD_L2_GotoX(&tmp_glcd, GLCD_L2_HALF_Both, j); // x  
      tmp_wait_for_busy(GLCD_L0_HALF_Right);
      for (size_t i = 0; i < 64; i++)
      {
        GLCD_L2_WriteByte(&tmp_glcd, GLCD_L2_HALF_Left, (uint8_t)(i+ tmp_cntr));
        GLCD_L2_WriteByte(&tmp_glcd, GLCD_L2_HALF_Right, (uint8_t)(i+ tmp_cntr+ 64));        
      }
    }

    /*
    // Blink the LED once
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(300);
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(300);
    */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
