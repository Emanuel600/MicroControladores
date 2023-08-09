/**
 * @file Traffic_Light.h
 * @author Emanuel Staub
 * @brief Declarations and Data Structures for Traffic_Light.c
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TRAFFIC_LIGHT_H
// TRAFFIC_LIGHT_H = Major_Release_Version*100 + Minor_Version*10 + Patch_Version
#define TRAFFIC_LIGHT_H         10
#include "stm32f1xx_hal.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @addtogroup Traffic_Light Traffic_Light
 * @{
 */

/**
 * @brief Status of the traffic light
 *
 * @enum    traffic_status_e
 *
 * @param TR_ERROR:    Some kind of error has been detected,
 *  error will be comunicated through `stderr`, TR_YELLOW light
 *  turned **ON** and blinking
 *
 * @param TR_STANDBY:  Currently waiting for status checks,
 *  turns TR_RED light **ON** and blinking
 */
typedef enum Traffic_Status {
    TR_GREEN,
    TR_YELLOW,
    TR_RED,
    TR_ERROR,
    TR_STANDBY
} traffic_status_e;

/**
 * @brief Contains the necessary data for operating the traffic light
 *
 * @struct traffic_light_s
 * @param LIGHT_PORT:       Address to the GPIO port that controls the lights
 * @param RED_LIGHT:        Pin used to turn the red light ON/OFF
 * @param YELLOW_LIGHT:     Pin used to turn the yellow light ON/OFF
 * @param GREEN_LIGHT:      Pin used to turn the green light ON/OFF
 * @param Current_Status:   Current operating status of the traffic light (as described in traffic_status_e)
 */
typedef struct Traffic_Light {
    GPIO_TypeDef                *LIGHT_PORT;
    uint16_t                    RED_LIGHT;
    uint16_t                    YELLOW_LIGHT;
    uint16_t                    GREEN_LIGHT;
    volatile traffic_status_e   Current_Status;
} traffic_light_s;

/**
 * @brief Initiates the traffic light
 *
 * @param PORT:     Controll port
 * @param red:      Red light pin
 * @param yellow:   Yellow light pin
 * @param green:    Green light pin
 * @return traffic_light_s
 *
 * @note:           Sets status to either `[TR_ERROR]` or `[TR_STANDBY]`
 */
traffic_light_s Init_Traffic_Light(GPIO_TypeDef *PORT, uint16_t green, uint16_t yellow, uint16_t red);

/// @defgroup       Traffic_State_Setters State_Setters
/// @{

/// @brief          Turns the red light on
/// @param light:   Pointer to the traffic light
/// @warning:       Turns to error mode on sequence error (Green->Red)
bool Turn_Red_On(traffic_light_s *light);

/// @brief          Turns the green light on
/// @param light:   Pointer to the traffic light
/// @warning:       Turns to error mode on sequence error (Yellow->Green)
bool Turn_Green_On(traffic_light_s *light);

/// @brief          Turns the yellow light on
/// @param light:   Pointer to the traffic light
/// @warning:       Turns to error mode on sequence error (Red->Yellow)
bool Turn_Yellow_On(traffic_light_s *light);

/// @brief          Sets the error sign on the traffic light
/// @param light:   Pointer to the traffic light
bool Traffic_Error(traffic_light_s *light);

/// @brief          Sets the light to standby
/// @param light:   Pointer to the traffic light
bool Traffic_STANDBY(traffic_light_s *light);
/** @} */

/// @brief          Array of function pointers to set a state
typedef bool (*StateSetters_fp)();

/// @}

#ifdef __cplusplus
}
#endif

#endif
