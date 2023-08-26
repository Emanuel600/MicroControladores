/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <errno.h>

#include "Global_Defines.h"
#include "HD44780.h"
#include "atraso.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/// @addtogroup Definitions     LCD_Pins
/// @{
/// @brief          GPIO Port used for data transfer
#define LCD_DATA_PORT                   GPIOB
/// @brief          First GPIO Pin to be used (Using sequential pins)
#define LCD_FIRST_DATA_PIN              GPIO_PIN_3
/// @brief          GPIO Port used for Register Select
#define LCD_CONTROL_PORT                GPIOA
/// @brief          GPIO Pin used for Register Select
#define LCD_RS_PIN                      GPIO_PIN_8
/// @brief          Pin used to increase hours
#define HOUR_PIN                        GPIO_PIN_5
/// @brief          Pin used to increase minutes
#define MINUTE_PIN                      GPIO_PIN_6
/// @}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile RTC_TimeTypeDef RTC_Time = {0};
HD44780 lcd;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
/* USER CODE BEGIN PFP */
extern void Reset_Handler();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main (void)
{
    /* USER CODE BEGIN 1 */
    setvbuf (stdout, NULL, _IONBF, 0);
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
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    Set_Control_Port (& (lcd.HD_GPIO), LCD_CONTROL_PORT, LCD_RS_PIN, CONTROL_WITHOUT_RW);
    Set_Data_Port (& (lcd.HD_GPIO), LCD_DATA_PORT, LCD_FIRST_DATA_PIN, LCD_4_BITS);
    HD44780_Init (&lcd);
    HD44780_Begin (&lcd, 16, 2, LCD_5x8DOTS);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        // Testing
        HAL_RTC_GetTime (&hrtc, (RTC_TimeTypeDef*) &RTC_Time, RTC_FORMAT_BCD);
        printf ("%02x:%02x:%02x", RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);
        delay_ms (500);
        HD44780_Clear (&lcd);
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    return -1;
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config (void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
/**
 * @brief EXTI IRQ Handler
 *
 * @param GPIO_Pin  Pin that triggered the interruption
 */
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
    if (GPIO_Pin == HOUR_PIN) {
        RTC_Time.Hours++;
    } else {
        RTC_Time.Minutes++;
    }
    pdebug ("EXTI IRQ called\r\n");
    return;
}

int _write (int fd, char* data, int len ATTRIBUTE (unused))
{
    if (fd == fileno (stdin)) {
        perrorf ("[ERROR] Called '_write' function for input");
        return 0;
    } else if (fd == fileno (stdout)) {
        return HD44780_Print (&lcd, data);
    } else if (fd == fileno (stderr)) {
        return 0;
    } else {
        perrorf ("[ERROR] Output file not supported");
        return 0;
    }
    // Critical error on return
    return -1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler (void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    perrorf ("[Error] Program crashed {Code %d}\r\n", errno);
    perrorf ("      - Attempting restart\r\n");
    Reset_Handler();
    perrorf ("      - Restart Failed, entering sleep mode\r\n");
    // Sleeps forever since IRQ are disabled
    HAL_PWR_EnterSLEEPMode (PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    while (1) {
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
void assert_failed (uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
