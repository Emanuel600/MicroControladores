#define F_CPU 16000000UL // Define para "util/delay"

#include <avr/io.h>
#include <util/delay.h>

#include "lcd_aux.h"
#include "funsape/peripheral/timer1"

#define clrBit(auxReg, auxBit)      ((auxReg) &= (~(1<<(auxBit))))
#define SetBit(auxReg, auxBit)      ((auxReg) |= (1<<(auxBit)))
#define isBitSet(auxReg, auxBit)    ((auxReg>>auxBit) & 0x01)
#define isBitClear(auxReg, auxBit)  (!(isBitSet(auxReg, auxBit)))

int main()
{
    // SetUp
    uint8_t hours   = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    UCSR0B      = 0;    // Disable USArt
    LCD_DDR     = 0xFF; // Utilizando todas as portas D como saída
    LCD_PORT    = 0x00; // Set all pins to LOW



    while(1) {

    }
    return 0;
}
