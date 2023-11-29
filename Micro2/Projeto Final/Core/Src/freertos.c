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
#include "logic_handler.h"
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
osThreadId ShooterTaskHandle;
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
void Move_Shooter(void* pvParameters);
void Move_Char(void* pvParamaters);
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
    pontos_t q1 = {
        .x1 = 0,
        .y1 = 0
    };
    desenha_fig(&q1, &Background_Top);
    q1.y1 += 8;
    desenha_fig(&q1, &Background_Left);
    q1.y1 += 32;
    desenha_fig(&q1, &Background_Bottom);
    q1.y1 = 8;
    q1.x1 = 77;
    desenha_fig(&q1, &Background_Right);
    /* USER CODE END Init */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    osThreadDef(Move_Thread, Move_Char, osPriorityNormal, 0, 128);
    MoveTaskHandle = osThreadCreate(osThread(Move_Thread), NULL);

    osThreadDef(Shooter_Thread, Move_Shooter, osPriorityNormal, 0, 128);
    ShooterTaskHandle = osThreadCreate(osThread(Shooter_Thread), NULL);

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
    UNUSED(argument);
    /* Infinite loop */
    for(;;) {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// Move Personagem
void Move_Char(void* param)
{
    UNUSED(param);
    struct pontos_t p = {
        .x1 = 38,
        .y1 = 10,
        .x2 = 00,
        .y2 = 00
    };
    struct pontos_t hitbox = p;
    hitbox.x2 = hitbox.x1 + Char_fig.largura;
    hitbox.y2 = hitbox.y1 + Char_fig.altura;

    while(1) {
        Apaga_Figura(&p, &Char_fig);
        // Eixo 'x'
        if(Axis[0] < -200) {
            if(hitbox.x1 > 0) {
                hitbox.x1--;
                hitbox.x2--;
            }
        } else if(Axis[0] > 200) { // 50 = Delta de tolerancia no deslocamento do eixo
            if(hitbox.x1 < 75) {
                hitbox.x1++;
                hitbox.x2++;
            }
        }
        // Eixo 'y'
        if(Axis[1] < -200) {
            if(hitbox.y1 < 40) {
                hitbox.y1++;
                hitbox.y2++;
            }
        } else if(Axis[1] > 200) {
            if(hitbox.y1 > 0) {
                hitbox.y1--;
                hitbox.y2--;
            }
        }
        // Verifica se a nova localização colide com algo
        Collision_e coll = Check_Collision(&hitbox);
        if(!coll) { // Não colidiu
            p.x1 = hitbox.x1;
            p.y1 = hitbox.y1;
        } else { // Colidiu
            // Em X e Y
            if(coll == XY_COLLISION) {
                hitbox = p;
            } else if(coll == Y_COLLISION) { // Apenas em Y
                p.x1 = hitbox.x1;
                hitbox.y1 = p.y1;
            } else if(coll == X_COLLISION) { // Apenas em X
                p.y1 = hitbox.y1;
                hitbox.x1 = p.x1;
            }
            hitbox.x2 = hitbox.x1 + Char_fig.largura;
            hitbox.y2 = hitbox.y1 + Char_fig.altura;
        }
        // Atualiza Buffer da LCD
        desenha_fig(&hitbox, &Char_fig);
        osDelay(100);
    }
}
// Move "Shooter"
void Move_Shooter(void* parm)
{
    struct pontos_t p = {
        .x1 = 20,
        .y1 = 15
    };
    struct pontos_t hitbox = p;
    hitbox.x2 = hitbox.x1 + Shooter.largura;
    hitbox.y2 = hitbox.y1 + Shooter.altura;
    int32_t direction = 1;

    while(1) {
        Apaga_Figura(&p, &Shooter);
        hitbox.y1 += direction;
        hitbox.y2 += direction;
        Collision_e coll = Check_Collision(&hitbox);
        if(coll == NO_COLLISION) {
            p.y1 = hitbox.y1;
        } else if(coll == Y_COLLISION) {
            hitbox.y1 = p.y1;
            hitbox.y2 -= direction;
            direction *= -1;
        }
        desenha_fig(&p, &Shooter);
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
        Axis[0] = 2048 - ADC_Buffer[0];
        Axis[1] = 2048 - ADC_Buffer[1];
    }
}

/* USER CODE END Application */
