#define F_CPU 16000000UL // Define para "util/delay"

#include <avr/io.h>
#include <util/delay.h>
// Registrador de Deslocamentos - Pinos
#define SHIFTREG_DDR    DDRD
#define SHIFTREG_PORT   PORTD
#define SHIFTREG_PIN    PIND
// Registrador de Deslocamentos - Entradas / Tamaho
#define SHIFTREG_INPUT  PD0
#define SHIFTREG_SHIFT  PD1
#define SHIFTREG_STORE  PD2
#define SHIFTREG_SIZE   16
// Funções Auxiliares
#define clrBit(auxReg, auxBit)      ((auxReg) &= (~(1<<(auxBit))))
#define SetBit(auxReg, auxBit)      ((auxReg) |= (1<<(auxBit)))
#define isBitSet(auxReg, auxBit)    ((auxReg>>auxBit) & 0x01)
#define isBitClear(auxReg, auxBit)  (!(isBitSet(auxReg, auxBit)))

void shift();
void store();
void light_single_led(uint8_t led);

int main()
{
    UCSR0B  = 0;    // Disable USArt
    SHIFTREG_DDR    = 0xFF; // Utilizando todas as portas como saída
    SHIFTREG_PORT   = 0x00; // Set all pins to LOW
    uint8_t i;

    while(1) {
        SetBit(SHIFTREG_PORT, SHIFTREG_INPUT);
        // Delay deve ser maior que 15n
        __BUILTIN_AVR_NOP;
        shift();
        // Delay deve ser maior que 22n
        __BUILTIN_AVR_NOP;
        store();
        clrBit(SHIFTREG_PORT, SHIFTREG_INPUT);
        _delay_ms(250);
        for(i = 1; i < SHIFTREG_SIZE; i++) {
            shift();
            // Delay deve ser maior que 22n
            __BUILTIN_AVR_NOP;
            store();
            _delay_ms(250);
        }
        i = 15;
        while(i != 0) {
            light_single_led(i--);
        }
    }
    return 0;
}

void shift()
{
    SetBit(SHIFTREG_PORT, SHIFTREG_SHIFT);
    // Pulso deve durar ao menos 22n - período de ~62ns
    __BUILTIN_AVR_NOP;
    clrBit(SHIFTREG_PORT, SHIFTREG_SHIFT);
}

void store()
{
    SetBit(SHIFTREG_PORT, SHIFTREG_STORE);
    // Pulso deve durar ao menos 22n - período de ~62ns
    __BUILTIN_AVR_NOP;
    clrBit(SHIFTREG_PORT, SHIFTREG_STORE);
}
// numbers LEDs 1-16 instead of 0-15
void light_single_led(uint8_t led)
{
    uint8_t i = SHIFTREG_SIZE;
    while(i != 0) {
        // O Bit Mais Significativo é enviado primeiro
        (i == led) ? SetBit(SHIFTREG_PORT, SHIFTREG_INPUT) : clrBit(SHIFTREG_PORT, SHIFTREG_INPUT);
        // Delay deve ser maior que 15n
        clrBit(SHIFTREG_PORT, SHIFTREG_INPUT);
        shift();
        i--;
    }
    // Store só precisa ser ativado uma vez
    __BUILTIN_AVR_NOP;
    store();
}
