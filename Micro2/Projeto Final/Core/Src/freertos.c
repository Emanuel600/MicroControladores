/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NOKIA5110_fb.h"
#include "figuras.h"
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

extern uint32_t ADC_Buffer[2];
/**
 * @brief           Valor do "joystick"
 *
 * @param 1:        Eixo x
 * @param 2:        Eixo y
 */
static int32_t Axis[2];
osThreadId MoveTaskHandle;
osThreadId RefreshTaskHandle;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/**
 * @brief               Moves Figure by (x,y)
 *
 * @param pvParameters  struct containing destination and figure
 */
void Task_Move_Figure(void* pvParameters);
void Move_Rect(void* pvParamaters);
/**
 * @brief               Refreshes screen periodically (17ms or ~59 Hz)
 *
 * @param pvParameters  NULL
 */
void Refresh_Screen(void* pvParameters);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const* argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer,
        uint32_t* pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer,
        uint32_t* pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */
    static struct pontos_t q = {
        .x1 = 40,
        .y1 = 20,
        .x2 = 45,
        .y2 = 25
    };
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
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    osThreadDef(Move_Thread, Move_Rect, osPriorityNormal, 0, 128);
    MoveTaskHandle = osThreadCreate(osThread(Move_Thread), &q);

    osThreadDef(Refresh_Thread, Refresh_Screen, osPriorityNormal, 0, 128);
    RefreshTaskHandle = osThreadCreate(osThread(Refresh_Thread), NULL);
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const* argument)
{
    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for(;;) {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Move_Rect(void* param)
{
    struct pontos_t p = {
        .x1 = 10,
        .y1 = 10
    };

    while(1) {
        Apaga_Figura(&p, &Cursor);
        // Eixo 'x'
        if(Axis[0] < -200) {
            if(p.x1 > 0) {
                p.x1--;
            }
        } else if(Axis[0] > 200) { // 50 = Delta de tolerancia no deslocamento do eixo
            if(p.x1 < 75) {
                p.x1++;
            }
        }
        // Eixo 'y'
        if(Axis[1] < -200) {
            if(p.y1 < 40) {
                p.y1++;
            }
        } else if(Axis[1] > 200) {
            if(p.y1 > 0) {
                p.y1--;
            }
        }
        // Atualiza Buffer da LCD
        desenha_fig(&p, &Cursor);
        osDelay(150);
    }
}

void Refresh_Screen(void* pvParam)
{
    portTickType Last_Wake = xTaskGetTickCount();
    while(1) {
        imprime_LCD();
        osDelayUntil(&Last_Wake, 100 * portTICK_PERIOD_MS);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) {
        Axis[1] = 2048 - ADC_Buffer[0];
        Axis[0] = 2048 - ADC_Buffer[1];
    }
}

/* USER CODE END Application */
