/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @version        : 0.2.1
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
#include <stdio.h>
#include <unistd.h>
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "atraso.h"
#include "Traffic_Light.h"
#include "Global_Defines.h"

/// @defgroup Traffic Light Pins and Port
/// @{
/// Traffic Light #1
#define LIGHT1_PORT         GPIOB
#define GREEN1_PIN          GPIO_PIN_0
#define YEL1_PIN            GPIO_PIN_1
#define RED1_PIN            GPIO_PIN_2
/// Traffic Light #2
#define LIGHT2_PORT         GPIOB
#define GREEN2_PIN          GPIO_PIN_5
#define YEL2_PIN            GPIO_PIN_6
#define RED2_PIN            GPIO_PIN_7
/// Traffic Light #3
#define LIGHT3_PORT         GPIOA
#define GREEN3_PIN          GPIO_PIN_5
#define YEL3_PIN            GPIO_PIN_6
#define RED3_PIN            GPIO_PIN_7
/// Traffic Light #4
#define LIGHT4_PORT         GPIOA
#define GREEN4_PIN          GPIO_PIN_9
#define YEL4_PIN            GPIO_PIN_10
#define RED4_PIN            GPIO_PIN_11
/// @}
#define GREEN_DELAY         3U
#define YELLOW_DELAY        1U
#define RED_DELAY           2U

/// @brief          Rerouting stderr to USART2
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
extern StateSetters_fp Set_Traffic_State[];
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

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
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    // Only enable USART when debugging
    DEBUG_BLOCK(
            MX_USART2_UART_Init();
    )
    pdebug("[OK]    DEBUG info redirected to USART\r\n");

    traffic_light_s light1 = Init_Traffic_Light(LIGHT1_PORT, GREEN1_PIN, YEL1_PIN, RED1_PIN);
    traffic_light_s light2 = Init_Traffic_Light(LIGHT2_PORT, GREEN2_PIN, YEL2_PIN, RED2_PIN);
    traffic_light_s light3 = Init_Traffic_Light(LIGHT3_PORT, GREEN3_PIN, YEL3_PIN, RED3_PIN);
    traffic_light_s light4 = Init_Traffic_Light(LIGHT4_PORT, GREEN4_PIN, YEL4_PIN, RED4_PIN);

    while(1) {
        // Operate Light 1
        Set_Traffic_State[TR_GREEN](&light1);
        delay_s(GREEN_DELAY);
        Set_Traffic_State[TR_YELLOW](&light1);
        delay_s(YELLOW_DELAY);
        Set_Traffic_State[TR_RED](&light1);
        delay_s(RED_DELAY);
        // Operate Light 2
        Set_Traffic_State[TR_GREEN](&light2);
        delay_s(GREEN_DELAY);
        Set_Traffic_State[TR_YELLOW](&light2);
        delay_s(YELLOW_DELAY);
        Set_Traffic_State[TR_RED](&light2);
        delay_s(RED_DELAY);
        // Operate Light 3
        Set_Traffic_State[TR_GREEN](&light3);
        delay_s(GREEN_DELAY);
        Set_Traffic_State[TR_YELLOW](&light3);
        delay_s(YELLOW_DELAY);
        Set_Traffic_State[TR_RED](&light3);
        delay_s(RED_DELAY);
        // Operate Light 4
        Set_Traffic_State[TR_GREEN](&light4);
        delay_s(GREEN_DELAY);
        Set_Traffic_State[TR_YELLOW](&light4);
        delay_s(YELLOW_DELAY);
        Set_Traffic_State[TR_RED](&light4);
        delay_s(RED_DELAY);
    }
    /* USER CODE END 3 */
    return -1;
}

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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9
            | GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_5
            | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pins : PA5 PA6 PA7 PA9
                             PA10 PA11 */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9
            | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PB0 PB1 PB2 PB5
                             PB6 PB7 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_5
            | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
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
    while(1) {
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

/**
 * @brief Re-routes STDs to USART
 *
 * @param fd:       File descriptor
 * @param ptr:      String to write
 * @param len:      Bytes to write $(strlen(ptr))
 * @return int:     bytes written $(len)
 */
int _write(int fd, char *ptr, int len)
{
    if(fd == STDERR_FILENO) {
        HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    }
    return len;
}
