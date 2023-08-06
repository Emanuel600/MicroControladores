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
#include "Global_Defines.h"

#ifdef _DEBUG
#include <stdio.h>
#endif

/// @brief                  Calls forth the state setter function, can return an error
/// @param Turn_Green_On:   Switches to GREEN state
/// @param Turn_Yellow_On:  Switches to YELLOW state
/// @param Turn_Red_On:     Switches to RED state
/// @param Traffic_Error:   Switches to GREEN state
/// @param Traffic_STANDBY: Switches to STANDBY state
StateSetters_fp Set_Traffic_State[] = {
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
        pdebug("[ERROR] setting port: Invalid Address\r\n");
        Set_Traffic_State[TR_ERROR](&light);
        return light;
    } else if(!IS_GPIO_PIN(red)) {
        pdebug("[ERROR] setting the red light's pin: Invalid Address\r\n");
        Set_Traffic_State[TR_ERROR](&light);
        return light;
    } else if(!IS_GPIO_PIN(yellow)) {
        pdebug("[ERROR] setting the yellow light's pin: Invalid Address\r\n");
        Set_Traffic_State[TR_ERROR](&light);
        return light;
    } else if(!IS_GPIO_PIN(green)) {
        pdebug("[ERROR] setting the green light's pin: Invalid Address\r\n");
        Set_Traffic_State[TR_ERROR](&light);
        return light;
    }
    // All addresses are valid, so we may continue
    light.LIGHT_PORT    = PORT;
    light.RED_LIGHT     = red;
    light.YELLOW_LIGHT  = yellow;
    light.GREEN_LIGHT   = green;

    pdebug("[OK!] - Light Initialized Properly\r\n");
    // Checks all lights - ~1 second delay
    Set_Traffic_State[TR_STANDBY](&light);
    // Sets Red light
    Set_Traffic_State[TR_RED](&light);

    return light;
}

bool Turn_Red_On(traffic_light_s *light)
{
    if(light == NULL) {
        pdebug("Invalid pointer to traffic light\r\n");
        return false;
    }
    // Make sure there are no logic conflicts
    if((light->Current_Status != TR_YELLOW)) {
        if((light->Current_Status != TR_STANDBY)) {
            Set_Traffic_State[TR_ERROR](light);
            return false;
        }
    }
    Clr_Pin(light->LIGHT_PORT, light->GREEN_LIGHT | light->YELLOW_LIGHT);
    Set_Pin(light->LIGHT_PORT, light->RED_LIGHT);

    light->Current_Status = TR_RED;

    return true;
}

bool Turn_Green_On(traffic_light_s *light)
{
    if(light == NULL) {
        pdebug("Invalid pointer to traffic light\r\n");
        return false;
    }
    // Make sure there are no logic conflicts
    if((light->Current_Status != TR_RED)) {
        Set_Traffic_State[TR_ERROR](light);
        return false;
    }
    Set_Pin(light->LIGHT_PORT, light->GREEN_LIGHT);
    Clr_Pin(light->LIGHT_PORT, light->RED_LIGHT | light->YELLOW_LIGHT);

    light->Current_Status = TR_GREEN;

    return true;
}

bool Turn_Yellow_On(traffic_light_s *light)
{
    if(light == NULL) {
        pdebug("Invalid pointer to traffic light\r\n");
        return false;
    }
    // Make sure there are no logic conflicts
    if((light->Current_Status != TR_GREEN)) {
        Set_Traffic_State[TR_ERROR](light);
        return false;
    }
    Clr_Pin(light->LIGHT_PORT, light->GREEN_LIGHT | light->RED_LIGHT);
    Set_Pin(light->LIGHT_PORT, light->YELLOW_LIGHT);

    light->Current_Status = TR_YELLOW;

    return true;
}

bool Traffic_Error(traffic_light_s *light)
{
    if(light == NULL) {
        pdebug("[ERROR] Invalid pointer to traffic light\r\n");
        return false;
    }
    Clr_Pin(light->LIGHT_PORT, light->GREEN_LIGHT | light->RED_LIGHT);

    light->Current_Status = TR_ERROR;

    do {
        Toggle_Pin(light->LIGHT_PORT, light->YELLOW_LIGHT);
        HAL_Delay(250);
        // Check_For_Errors(&light); => Change to STANDBY maybe?
    } while(light->Current_Status == TR_ERROR);
    Set_Traffic_State[TR_RED](light);
    return false;
}

bool Traffic_STANDBY(traffic_light_s *light)
{
    if(light == NULL) {
        pdebug("Invalid pointer to traffic light\r\n");
        return false;
    }

    light->Current_Status = TR_STANDBY;

    /* Test all lights */
    // Green
    Set_Pin(light->LIGHT_PORT, light->GREEN_LIGHT);
    Clr_Pin(light->LIGHT_PORT, light->YELLOW_LIGHT | light->RED_LIGHT);
    // Could check green light here
    delay_ms(333);
    // Yellow
    Set_Pin(light->LIGHT_PORT, light->YELLOW_LIGHT);
    Clr_Pin(light->LIGHT_PORT, light->GREEN_LIGHT | light->RED_LIGHT);
    // Could check yellow light here
    delay_ms(333);
    // Red
    Set_Pin(light->LIGHT_PORT, light->RED_LIGHT);
    Clr_Pin(light->LIGHT_PORT, light->YELLOW_LIGHT | light->GREEN_LIGHT);
    // Could check red light here
    delay_ms(333);

    return true;
}
