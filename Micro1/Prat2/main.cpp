#define F_CPU 16000000UL // Define para "util/delay"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "SevenSegments.h"
#include "funsape/globaldefines.hpp"

#define BUTTON_DDR  DDRB
#define BUTTON_PORT PORTB
#define BUTTON_PIN  PINB
#define BUTTON_BIT  PB2

#define SetBit(auxReg, auxBit)      ((auxReg) |= (1<<(auxBit)))
#define isBitClear(auxReg, auxBit)  (!(isBitSet(auxReg, auxBit)))

vuint8_t direction   = 0;

int main()
{
    // SetUp
    // uint8_t direction   = 0; // bool
    uint8_t counter     = 0;

    UCSR0B      = 0;    // Disable USArt
    DDRD        = 0xFF; // Utilizando todas as portas D como saída
    PORTD       = 0xFF; // Set all pins to HIGH
    BUTTON_PORT = 0X00; // Setting all pins as input for simplicity
    PCICR       = 0X01; // Set to detect interrupt on PCINT0-7
    PCMSK0     |= 0X01; // Sets interrupt mask to enable only PCINT0 pin

    while(1) {
        PORTD   = hex2SevenSegments(counter, DISPLAY_COMMON_ANODE, 0);

        counter = ((counter == 0xF) & direction)  ? 0 : counter + 1;
        counter = ((counter == 0x0) & !direction) ? 0 : counter - 1;

        _delay_ms(300);
    }
    return 0;
}

// Handle INT0 interrupt
ISR(PCINT0_vect)
{
    direction = !direction;
}
