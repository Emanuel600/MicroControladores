/**
 * @file Global_Defines.h
 * @author Emanuel Staub
 * @brief Global defines used in (nearly) all projects
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef GLOBAL_DEFINES
#define GLOBAL_DEFINES  10

//! Change pin status
#define Set_Pin(GPIOx, PINy)            HAL_GPIO_WritePin(GPIOx, PINy, GPIO_PIN_SET)
#define Clr_Pin(GPIOx, PINy)            HAL_GPIO_WritePin(GPIOx, PINy, GPIO_PIN_RESET)
#define Toggle_Pin(GPIOx, PINy)         HAL_GPIO_Toggle_Pin(GPIOx, PINy)
//! Delay utils
#define delay_ms(delay)                 HAL_Delay((uint32_t)delay)
#define delay_s(delay)                  delay_ms(delay*1000U)

#endif
