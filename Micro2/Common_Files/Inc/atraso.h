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
#include <stdint.h>

#include "Global_Defines.h"

#ifndef FCPU
#define FCPU 7200000
#endif

asm(
        ".equ fcpu, FCPU\n\t"
        ".equ const_ms fcpu/3000\n\t"
        ".equ const_us, fcpu/300000\n\t"
);
/**
 * @brief Delays "delay" milliseconds
 *
 * @param delay:    Milliseconds to delay
 */
void atraso_ms(uint32_t delay);

/**
 * @brief Delays "delay" microseconds
 *
 * @param delay:    Microseconds to delay
 */
void atraso_us(uint32_t delay);