#include "atraso.h"

inlined void wait_cycles(nuint cycles)
{
    nuint loops = cycles / CYCLES_PER_LOOP;
    ASM(
            "0:" "SUBS %[loops], 1;" "BNE 0b;" : [loops]"+r"(loops)
    );

    return;
}

inlined void atraso_ms(nuint delay)
{
    // Amount of times the loop needs to be executed
    delay *= ATRASO_MS_LOOP;
    ASM(
            "0:" "SUBS %[delay], 1;" "BNE 0b;" : [delay]"+r"(delay)
    );

    return;
}

inlined void atraso_us(nuint delay)
{
    // Amount of times the loop needs to be executed
    delay *= ATRASO_US_LOOP;
    ASM(
            "0:" "SUBS %[delay], 1;" "BNE 0b;" : [delay]"+r"(delay)
    );

    return;
}
