/**
 * @file Traffic_Light.c
 * @author your name (you@domain.com)
 * @brief Definitions for Traffic_Light.h
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Traffic_Light.h"
#ifdef _DEBUG
#include <stdio.h>
#endif

/// @brief                  Calls forth the state setter function, can return an error
/// @param Turn_Green_On:   Switches to GREEN state
/// @param Turn_Yellow_On:  Switches to YELLOW state
/// @param Turn_Red_On:     Switches to RED state
/// @param Traffic_Error:   Switches to GREEN state
/// @param Traffic_STANDBY: Switches to STANDBY state
StateSetters_fp State_Setters[] = {
    Turn_Green_On,
    Turn_Yellow_On,
    Turn_Red_On,
    Traffic_Error,
    Traffic_STANDBY
};

traffic_light_s Init_Traffic_Light(GPIO_TypeDef *PORT, uint16_t green, uint16_t yellow, uint16_t red)
{
    traffic_light_s light;
    /**
     * @brief Check that port and pins are valid addresses, sets RED light
     *
     * @note USART is only connected when debugging, so it only prints
     * if _DEBUG is defined (on Project_Root/env_vars.mk)
     */
    if(!IS_GPIO_ALL_INSTANCE(PORT)) {
#ifdef _DEBUG
        fprintf(stderr, "[ERROR] setting port: Invalid Address");
#endif
        State_Setters[TR_ERROR](&light);
        return light;
    } else if(!IS_GPIO_PIN(red)) {
#ifdef _DEBUG
        fprintf(stderr, "[ERROR] setting the red light's pin: Invalid Address");
#endif
        State_Setters[TR_ERROR](&light);
        return light;
    } else if(!IS_GPIO_PIN(yellow)) {
#ifdef _DEBUG
        fprintf(stderr, "[ERROR] setting the yellow light's pin: Invalid Address");
#endif
        State_Setters[TR_ERROR](&light);
        return light;
    } else if(!IS_GPIO_PIN(green)) {
#ifdef _DEBUG
        fprintf(stderr, "[ERROR] setting the green light's pin: Invalid Address");
#endif
        State_Setters[TR_ERROR](&light);
        return light;
    }
    // All addresses are valid, so we may continue
    light.LIGHT_PORT    = PORT;
    light.RED_LIGHT     = red;
    light.YELLOW_LIGHT  = yellow;
    light.GREEN_LIGHT   = green;
#ifdef _DEBUG
    fprintf(stderr, "[OK!] - Light Initialized Properly");
#endif
    // Checks all lights - ~1 second delay
    State_Setters[TR_STANDBY](&light);
    // Sets Red light
    State_Setters[TR_RED](&light);

    return light;
}

bool Turn_Red_On(traffic_light_s *light)
{
    if(light == NULL) {
#ifdef _DEBUG
        fprintf(stderr, "Invalid pointer to traffic light");
#endif
        return false;
    }
    // Make sure there are no logic conflicts
    if((light->Current_Status != TR_YELLOW) || (light->Current_Status != TR_STANDBY)) {
        State_Setters[TR_ERROR](light);
        return false;
    }
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->GREEN_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->YELLOW_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->RED_LIGHT, GPIO_PIN_SET);
    return true;
}

bool Turn_Green_On(traffic_light_s *light)
{
    if(light == NULL) {
#ifdef _DEBUG
        fprintf(stderr, "Invalid pointer to traffic light");
#endif
        return false;
    }
    // Make sure there are no logic conflicts
    if((light->Current_Status != TR_RED)) {
        State_Setters[TR_ERROR](light);
        return false;
    }
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->GREEN_LIGHT, GPIO_PIN_SET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->YELLOW_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->RED_LIGHT, GPIO_PIN_RESET);
    return true;
}

bool Turn_Yellow_On(traffic_light_s *light)
{
    if(light == NULL) {
#ifdef _DEBUG
        fprintf(stderr, "Invalid pointer to traffic light");
#endif
        return false;
    }
    // Make sure there are no logic conflicts
    if((light->Current_Status != TR_GREEN)) {
        State_Setters[TR_ERROR](light);
        return false;
    }
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->GREEN_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->YELLOW_LIGHT, GPIO_PIN_SET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->RED_LIGHT, GPIO_PIN_RESET);
    return true;
}

bool Traffic_Error(traffic_light_s *light)
{
    if(light == NULL) {
#ifdef _DEBUG
        fprintf(stderr, "Invalid pointer to traffic light");
#endif
        return false;
    }
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->GREEN_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->RED_LIGHT, GPIO_PIN_RESET);
    do {
        HAL_GPIO_TogglePin(light->LIGHT_PORT, light->YELLOW_LIGHT);
        HAL_Delay(250);
        // Check_For_Errors(&light);
    } while(light->Current_Status == TR_ERROR);
    return false;
}

bool Traffic_STANDBY(traffic_light_s *light)
{
    if(light == NULL) {
#ifdef _DEBUG
        fprintf(stderr, "Invalid pointer to traffic light");
#endif
        return false;
    }

    /* Test all lights */
    // Green
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->GREEN_LIGHT, GPIO_PIN_SET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->YELLOW_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->RED_LIGHT, GPIO_PIN_RESET);
    /// Could check green light here
    HAL_Delay(333);
    // Yellow
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->GREEN_LIGHT, GPIO_PIN_SET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->YELLOW_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->RED_LIGHT, GPIO_PIN_RESET);
    /// Could check yellow light here
    HAL_Delay(333);
    // Red
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->GREEN_LIGHT, GPIO_PIN_SET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->YELLOW_LIGHT, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(light->LIGHT_PORT, light->RED_LIGHT, GPIO_PIN_RESET);
    /// Could check red light here
    HAL_Delay(333);

    return true;
}
