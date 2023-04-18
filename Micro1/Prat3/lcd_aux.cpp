#include <util/delay.h>

#include "lcd_aux.h"

void lcd_init()
{
    _delay_ms(16);
    Func_Set(1, 0, 0);
    _delay_ms(5);
    Func_Set(1, 0, 0);
    _delay_us(110);
    Func_Set(1, 0, 0);
}
