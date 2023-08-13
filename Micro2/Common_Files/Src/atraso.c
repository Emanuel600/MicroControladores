#include "atraso.h"

inlined void wait_cycles(uint32_t cycles)
{
    const uint32_t cycles_per_loop = 4;
    uint32_t loops = cycles / cycles_per_loop;
    ASM(
            "0:" "SUBS %[loops], 1;" "BNE 0b;" : [loops]"+r"(loops)
    );

    return;
}

inlined void atraso_ms(uint32_t delay)
{
    // Loop frequency (kHz), also loops/ms
    const uint32_t atraso_ms_loop = FCPUk / 4U;
    // Amount of times the loop needs to be executed
    delay *= atraso_ms_loop;
    ASM(
            "0:" "SUBS %[delay], 1;" "BNE 0b;" : [delay]"+r"(delay)
    );

    return;
}

inlined void atraso_us(uint32_t delay)
{
    // Loop frequency (MHz), also loops/us
    const uint32_t atraso_us_loop = FCPUM / 4U;
    // Amount of times the loop needs to be executed
    delay *= atraso_us_loop;
    ASM(
            "0:" "SUBS %[delay], 1;" "BNE 0b;" : [delay]"+r"(delay)
    );

    return;
}
