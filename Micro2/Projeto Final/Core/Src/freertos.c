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
#include "queue.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* USER CODE BEGIN Variables */
extern uint32_t ADC_Buffer[2];
/**
 * @brief           Valor do "joystick"
 *
 * @param 1:        Eixo x
 * @param 2:        Eixo y
 */
static int32_t Axis[2];
/* Task Handles */
osThreadId MoveTaskHandle;
osThreadId ShotFiredHandle;
osThreadId ShooterTaskHandle;
osThreadId RefreshTaskHandle;
// Total number of active projectiles that can still be created
volatile uint32_t active_projectiles = 3;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
// => In main.h
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
    /* USER CODE END Init */
    // Refreshes Screen
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    // Moves MC
    osThreadDef(Move_Thread, Move_Char, osPriorityNormal, 0, 128);
    MoveTaskHandle = osThreadCreate(osThread(Move_Thread), NULL);
    /* Handles Shooter */
    // Movement
    osThreadDef(Shooter_Thread, Move_Shooter, osPriorityNormal, 0, 128);
    ShooterTaskHandle = osThreadCreate(osThread(Shooter_Thread), NULL);
    /* Handles 'Thing' */
    /* Handles Slime */
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
    pontos_t q1 = {
        .x1 = 0,
        .y1 = 0
    };
    // Draws Background
    desenha_fig(&q1, &Background_Top);
    q1.y1 += 8;
    desenha_fig(&q1, &Background_Left);
    q1.y1 += 32;
    desenha_fig(&q1, &Background_Bottom);
    q1.y1 = 8;
    q1.x1 = 77;
    desenha_fig(&q1, &Background_Right);
    portTickType Last_Wake = xTaskGetTickCount();
    /* Infinite loop */
    for(;;) {
        imprime_LCD();
        osDelayUntil(&Last_Wake, 100 * portTICK_PERIOD_MS);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// Move Personagem
void Move_Char(void* param)
{
    UNUSED(param);
    pontos_t p = {
        .x1 = 38,
        .y1 = 10,
        .x2 = 00,
        .y2 = 00
    };
    pontos_t player_hitbox = p;
    player_hitbox.x2 = player_hitbox.x1 + Char_fig.largura;
    player_hitbox.y2 = player_hitbox.y1 + Char_fig.altura;
    set_player_hitbox(&player_hitbox);

    while(1) {
        Apaga_Figura(&p, &Char_fig);
        // Eixo 'x'
        if(Axis[0] < -200) {
            if(player_hitbox.x1 > 0) {
                player_hitbox.x1--;
                player_hitbox.x2--;
            }
        } else if(Axis[0] > 200) { // 50 = Delta de tolerancia no deslocamento do eixo
            if(player_hitbox.x1 < 75) {
                player_hitbox.x1++;
                player_hitbox.x2++;
            }
        }
        // Eixo 'y'
        if(Axis[1] < -200) {
            if(player_hitbox.y1 < 40) {
                player_hitbox.y1++;
                player_hitbox.y2++;
            }
        } else if(Axis[1] > 200) {
            if(player_hitbox.y1 > 0) {
                player_hitbox.y1--;
                player_hitbox.y2--;
            }
        }
        // Verifica se a nova localização colide com algo
        Collision_e coll = Check_Collision(&player_hitbox);
        if(!coll) { // Não colidiu
            p.x1 = player_hitbox.x1;
            p.y1 = player_hitbox.y1;
        } else { // Colidiu
            // Em X e Y
            if(coll == XY_COLLISION) {
                player_hitbox = p;
            } else if(coll == Y_COLLISION) { // Apenas em Y
                p.x1 = player_hitbox.x1;
                player_hitbox.y1 = p.y1;
            } else if(coll == X_COLLISION) { // Apenas em X
                p.y1 = player_hitbox.y1;
                player_hitbox.x1 = p.x1;
            }
            player_hitbox.x2 = player_hitbox.x1 + Char_fig.largura;
            player_hitbox.y2 = player_hitbox.y1 + Char_fig.altura;
        }
        // Atualiza Buffer da LCD
        desenha_fig(&player_hitbox, &Char_fig);
        set_player_hitbox(&player_hitbox);
        osDelay(100);
    }
}
// Move "Shooter"
void Move_Shooter(void* parm)
{
    UNUSED(parm);
    pontos_t p = {
        .x1 = 20,
        .y1 = 15
    };
    pontos_t hitbox = p;
    hitbox.x2 = hitbox.x1 + Shooter.largura;
    hitbox.y2 = hitbox.y1 + Shooter.altura;
    int32_t direction = 1;
    uint32_t cycle = 0;

    osThreadDef(Shot_Thread, Shots_Fired, osPriorityNormal, 0, 128);

    while(1) {
        Apaga_Figura(&p, &Shooter);
        hitbox.y1 += direction;
        hitbox.y2 += direction;
        Collision_e coll = Check_Collision(&hitbox);
        if(coll == NO_COLLISION) {
            p.y1 = hitbox.y1;
        } else {
            hitbox.y1 = p.y1;
            hitbox.y2 -= direction;
            direction *= -1;
        }
        if(!(cycle - 6)) {
            // Shoots
            ShotFiredHandle = osThreadCreate(osThread(Shot_Thread), &(p.y1));
            cycle = 0;
        }
        cycle++;
        desenha_fig(&p, &Shooter);
        osDelay(250);
    }
}

void Refresh_Screen(void* pvParam)
{
    UNUSED(pvParam);
    portTickType Last_Wake = xTaskGetTickCount();
    while(1) {
        imprime_LCD();
        osDelayUntil(&Last_Wake, 100 * portTICK_PERIOD_MS);
    }
}

void Shots_Fired(void* param)
{
    pontos_t hitbox;
    pontos_t p;

    uint32_t y = *((uint32_t*) param);

    if(!active_projectiles) {
        vTaskDelete(NULL);
    }
    while(1) {
        active_projectiles--;
        p.y1 = y + 5;
        p.x1 = 20 + 3;
        hitbox = p;
        hitbox.x2 = p.x1 + 3;
        hitbox.y2 = p.y1;
        desenha_pixel(p.x1, p.y1, 1);
        desenha_pixel(p.x1 + 1, p.y1, 1);
        desenha_pixel(p.x1 + 2, p.y1, 1);
        osDelay(150);
        hitbox.x1++;
        hitbox.x2--;
        Collision_e coll = Check_Collision(&hitbox);
        Collision_e pcoll = Check_For_Player(&hitbox);
        while((coll + pcoll) == NO_COLLISION) {
            desenha_pixel(p.x1, p.y1, 0);
            desenha_pixel(p.x1 + 3, p.y1, 1);
            p.x1 = hitbox.x1;
            osDelay(150);
            hitbox.x1++;
            hitbox.x2++;
            coll = Check_Collision(&hitbox);
            pcoll = Check_For_Player(&hitbox);
        }
        desenha_pixel(p.x1, p.y1, 0);
        desenha_pixel(p.x1 + 1, p.y1, 0);
        desenha_pixel(p.x1 + 2, p.y1, 0);
        active_projectiles++;
        if(pcoll == Player_Collision) {
            ASM("nop");
        }
        vTaskDelete(NULL);
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
