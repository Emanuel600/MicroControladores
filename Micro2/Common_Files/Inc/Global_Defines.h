/**
 * @file Global_Defines.h
 * @author Emanuel Staub
 * @brief Global defines used in (nearly) all projects
 * @version 0.2
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef GLOBAL_DEFINES
#define GLOBAL_DEFINES  21
/**
 * @addtogroup Global_Defines Global_Defines
 * @{
 */
//! CPU Details
#ifdef STM32F103x6
    #define FCPU        72000000
    #define FCPUk       72000
    #define FCPUM       72
#endif
//! Change pin status
#define Set_Pin(GPIOx, PINy)            HAL_GPIO_WritePin(GPIOx, PINy, GPIO_PIN_SET)
#define Clr_Pin(GPIOx, PINy)            HAL_GPIO_WritePin(GPIOx, PINy, GPIO_PIN_RESET)
#define Toggle_Pin(GPIOx, PINy)         HAL_GPIO_TogglePin(GPIOx, PINy)
#define Write_Pin(GPIOx, PINy, W)       HAL_GPIO_WritePin(GPIOx, PINy, W)
//! Read Pin
#define Read_Pin(GPIOx, PINy)           HAL_GPIO_ReadPin(GPIOx, PINy)
//! Delay utils
#define delay_ms(delay)                 HAL_Delay((uint32_t)delay)
#define delay_s(delay)                  delay_ms(delay*1000U)
//! Debug Utils
#ifdef _DEBUG
    #define pdebug(...)                     fprintf(stderr, __VA_ARGS__)
    #define DEBUG_BLOCK(BLOCK)              {BLOCK}
#else
    #define pdebug(...)
    #define DEBUG_BLOCK(BLOCK)              {}
#endif
//! Pragmas - GCC
#ifdef __GNUC__
    #define ATTRIBUTE(att)                  __attribute__((att))
    /* Code optimization controll */
    /// @addtogroup     GCC Pragmas
    /// @{
    /// @brief          Pack struct
    #define PACKED                          ATTRIBUTE(packed)
    /// @brief          Pack with alignement
    #define PACKED_ALIGN(X)                 ATTRIBUTE(packed, aligned(X))
    /// @brief          Function is not optimized
    #define __UNOPTIMIZED__                 ATTRIBUTE(optimize("O0"))
    /// @brief          Function is optimized for speed
    #define __FASTEST__                     ATTRIBUTE(optimize("O3"))
    /// @brief          Function is aggressively optimized for size
    #define __MAX_COMPRESS__                ATTRIBUTE(optimize("Oz"))
    /// @brief          Function is optimized for size
    #define __COMPRESS__                    ATTRIBUTE(optimize("Os"))
    /// @}
    /* Function types */
    /// \defgroup       Function types
    /// \brief          Always inlines the function
    #define inlined                         inline ATTRIBUTE(always_inline)
    /// \addtogroup     Function types
    /// \brief          Function can be overwritten by a "strong" function
    #define weakened                        ATTRIBUTE(weak)
#endif
//! Misc Utils
/// @brief          Inserts asm instruction
#define ASM(instruction)                __asm__ volatile (instruction)
/// @brief          Prints formatted to sdterr
#define perrorf(...)                    fprintf(stderr, __VA_ARGS__)
/// @brief          GPIO_Pin typedef
typedef uint16_t                        GPIO_Pin;
/// @brief          Native uint
typedef uint32_t                        nuint;
/** @} */
#endif
