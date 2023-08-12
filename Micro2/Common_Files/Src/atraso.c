#include "atraso.h"

void atraso_ms(uint32_t delay)
{
    // Division takes at most 12 cycles, but since this is a constant
    // that's determined at compile time, it takes 0 cycles
    const uint32_t atraso_ms_constant = FCPUk / 6U;
    // Multiplication is a single clock cycle,
    // while a single millisecond requires 12,000 loops at 72 MHz
    delay *= atraso_ms_constant;
    ASM(
            "atraso_ms_loop:"
            "CMP    %[delay], #0\n\t" : : [delay] "r"(delay)
    );
    ASM(
            "BEQ    atraso_ms_return\n\t"
            "NOP    \n\t"
            "SUB    %[delay], %[delay], #1\n\t" : : [delay] "r"(delay)
    );
    ASM(
            "B      atraso_ms_loop\n\t"
            "atraso_ms_return:\n\t"
    );
    return;
}

void atraso_us(uint32_t delay)
{
    const uint32_t atraso_us_constant = FCPUM / 6U;
    delay *= atraso_us_constant;
    ASM(
            "atraso_us_loop:"
            "CMP    %[delay], #0\n\t" : : [delay] "r"(delay)
    );
    ASM(
            "BEQ    atraso_us_return\n\t"
            "NOP    \n\t"
            "SUB    %[delay], %[delay], #1\n\t" : : [delay] "r"(delay)
    );
    ASM(
            "B      atraso_us_loop\n\t"
            "atraso_us_return:\n\t"
    );
    return;
}
