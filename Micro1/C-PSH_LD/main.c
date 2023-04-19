#define F_CPU 16000000UL // Define para "util/delay"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// Posição do botão
#define BUTTON_DDR  DDRB
#define BUTTON_PORT PORTB
#define BUTTON_PIN  PINB
#define BUTTON_BIT  PB2
// Interrupção do botão
#define BUTTON_INT  PCINT2_vect
#define BUTTON_PCI  PCINT2
#define INT_PORT    PCIE0
// Posição do LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
// Funções
#define clrBit(auxReg, auxBit)      ((auxReg) &= (~(1<<(auxBit))))
#define SetBit(auxReg, auxBit)      ((auxReg) |= (1<<(auxBit)))
#define isBitSet(auxReg, auxBit)    ((auxReg>>auxBit) & 0x01)
#define isBitClear(auxReg, auxBit)  (!(isBitSet(auxReg, auxBit)))
#define BSL(num, val)               (num << val)
#define Push_to_LED_Port(v1, v2)    (LED_PORT = (BSL(1, v1) | BSL(1, v2)))

volatile uint8_t cont = 1;

int main()
{
    // SetUp
    UCSR0B      =   0;          // Disable USArt
    LED_DDR     =   0xFF;       // Utilizando todas as portas D como saída
    LED_PORT    =   0x00;       // Set all pins to LOW
    BUTTON_DDR  =   0x00;       // Set B Port as input for simplicity

    sei();                      // Enable global interrupts
    SetBit(PCICR, INT_PORT);    // Enable PCINT interrupt on PCINT 0-7
    SetBit(PCMSK0, BUTTON_PCI); // Set mask only for PB2

    while(1) {
        uint8_t pushed  = 0x01;  // Valor inicial do LED pushed
        uint8_t pusher  = 0x00;  // Valor inicial do LED pusher
        Push_to_LED_Port(pushed, pusher);
        _delay_ms(250);
        pusher++;
        Push_to_LED_Port(pushed, pusher);
        _delay_ms(250);

        while(pusher != 7) {
            (pusher == pushed) ? (++pushed; pusher=0) : (++pusher);
            Push_to_LED_Port(pushed, pusher);
            _delay_ms(250);
        }
    }
    return 0;
}

ISR(BUTTON_INT)
{
    cont = !cont;
    if(!cont) {
        while(isBitClear(BUTTON_PIN, BUTTON_BIT)) {}
    }
}
