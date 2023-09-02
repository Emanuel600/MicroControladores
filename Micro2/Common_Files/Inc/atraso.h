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

// On real hardware, divide by 6; on PROTEUS, divide by 4
#define CYCLES_PER_LOOP                 4U

// Loop frequency (kHz), also loops/ms
#define ATRASO_MS_LOOP                  FCPUk / CYCLES_PER_LOOP

// Loop frequency (MHz), also loops/us
#define ATRASO_US_LOOP                  FCPUM / CYCLES_PER_LOOP

/**
 * @brief           Waits n cycles
 *
 * @note            Can have a lot of overhead, depending on the value of CYCLES_PER_LOOP
 */
void wait_cycles(nuint cycles);

/**
 * @brief           Delays "delay" milliseconds
 *
 * @param delay:    Milliseconds to delay
 */
void atraso_ms(nuint delay);

/**
 * @brief           Delays "delay" microseconds, only acurate for delay >= 3
 *
 * @param delay:    Microseconds to delay
 */
void atraso_us(nuint delay);

#endif
