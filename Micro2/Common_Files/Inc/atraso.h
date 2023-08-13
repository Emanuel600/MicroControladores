/**
 * @file atraso.h
 * @author your name (you@domain.com)
 * @brief Delay functions, written in assembly
 * @version 0.1
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ATRASO_H
#define ATRASO_H    10

#include <stdint.h>

#include "Global_Defines.h"

/**
 * @brief Waits n cycles
 *
 */
void wait_cycles(uint32_t cycles);

/**
 * @brief Delays "delay" milliseconds
 *
 * @param delay:    Milliseconds to delay
 */
void atraso_ms(uint32_t delay);

/**
 * @brief Delays "delay" microseconds, only acurate for delay >= 3
 *
 * @param delay:    Microseconds to delay
 */
void atraso_us(uint32_t delay);

#endif
