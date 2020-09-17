/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GLCD_L2.h"
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
/* USER CODE BEGIN Variables */
GLCD_L0_TypeDef tmp_glcd;

// Temporary Variables for Debugging
char tmp_str[] = "12345678901234567890";
uint8_t tmp_cntr = 0;
/* USER CODE END Variables */
/* Definitions for LCD_Test_Task */
osThreadId_t LCD_Test_TaskHandle;
const osThreadAttr_t LCD_Test_Task_attributes = {
  .name = "LCD_Test_Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for BlinkingLED_Tas */
osThreadId_t BlinkingLED_TasHandle;
const osThreadAttr_t BlinkingLED_Tas_attributes = {
  .name = "BlinkingLED_Tas",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_LCD_Test_Task(void *argument);
void Start_BlinkingLED_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
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
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LCD_Test_Task */
  LCD_Test_TaskHandle = osThreadNew(Start_LCD_Test_Task, NULL, &LCD_Test_Task_attributes);

  /* creation of BlinkingLED_Tas */
  BlinkingLED_TasHandle = osThreadNew(Start_BlinkingLED_Task, NULL, &BlinkingLED_Tas_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_LCD_Test_Task */
/**
  * @brief  Function implementing the LCD_Test_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_LCD_Test_Task */
void Start_LCD_Test_Task(void *argument)
{
  /* USER CODE BEGIN Start_LCD_Test_Task */
  /* Infinite loop */
  // Perform a Reset procedure
  GLCD_L2_FullInit(&tmp_glcd, GLCD_L2_DispColor_Black);

  // Turning on and off and read the Status Reg
  GLCD_L1_Disp_OnOff(&tmp_glcd, GLCD_L0_HALF_Right, GLCD_L1_Disp_On); // LCD On
  GLCD_L1_Disp_OnOff(&tmp_glcd, GLCD_L0_HALF_Left, GLCD_L1_Disp_Off); // LCD Off
  // tmp_check_status();
  GLCD_L1_Disp_OnOff(&tmp_glcd, GLCD_L0_HALF_Left, GLCD_L1_Disp_On); // LCD On
  osDelay(1000);

  // tmp_check_status();

  // Ser Address Regs
  GLCD_L2_GotoXYZ(&tmp_glcd, GLCD_L2_HALF_Both, 0, 0, 0);

  for (size_t j = 0; j < 8; j++)
  {
    for (size_t i = 0; i < 64; i++)
    {
      GLCD_L2_WriteByteXYZ(&tmp_glcd, GLCD_L2_HALF_Both, (uint8_t)(0x55), j, i, i>>1);
      osDelay(45);
    }
  }
  osDelay(2000);
  GLCD_L2_ClearDisplay(&tmp_glcd, GLCD_L2_HALF_Both);
  osDelay(2000);

  for(;;)
  {
    tmp_cntr++;
    GLCD_L2_GotoZ(&tmp_glcd, GLCD_L2_HALF_Both, tmp_cntr%64);
    // Show image on LCD
    for (size_t j = 0; j < 4; j++)
    {  
      GLCD_L2_GotoX(&tmp_glcd, GLCD_L2_HALF_Both, j);
      for (size_t i = 0; i < 64; i++)
      {
        GLCD_L2_WriteByte(&tmp_glcd, GLCD_L2_HALF_Left, (uint8_t)(i+ tmp_cntr));
        GLCD_L2_WriteByte(&tmp_glcd, GLCD_L2_HALF_Right, (uint8_t)(i+ tmp_cntr+ 64));        
      }
    }
    osDelay(1);
  }
  /* USER CODE END Start_LCD_Test_Task */
}

/* USER CODE BEGIN Header_Start_BlinkingLED_Task */
/**
* @brief Function implementing the BlinkingLED_Tas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_BlinkingLED_Task */
void Start_BlinkingLED_Task(void *argument)
{
  /* USER CODE BEGIN Start_BlinkingLED_Task */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    osDelay(50);
  }
  osThreadTerminate(BlinkingLED_TasHandle);
  /* USER CODE END Start_BlinkingLED_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
