#define F_CPU 16000000UL // Define para "util/delay"

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

#include "SevenSegments.hpp"
// Posição do botão
#define BUTTON_DDR      DDRB
#define BUTTON_PORT     PORTB
#define BUTTON_PIN      PINB
#define BUTTON_M        PB0
#define BUTTON_C        PB1
#define BUTTON_D        PB2
#define BUTTON_U        PB3
// Interrupção do botão
#define BUTTON_INT      PCINT0_vect
#define INT_PORT        PCIE0
#define INT_MSK         PCMSK0
// Posição do Display
#define DISP_DDR        DDRD
#define DISP_PORT       PORTD
// Botões do Display
#define DISP_CTRL_DDR   DDRC
#define DISP_CTRL_PORT  PORTC
#define DISP_CTRL_U     PC0
#define DISP_CTRL_D     PC1
#define DISP_CTRL_C     PC2
#define DISP_CTRL_M     PC3
// Funções
#define ClrBit(auxReg, auxBit)      ((auxReg) &= (~(1<<(auxBit))))
#define SetBit(auxReg, auxBit)      ((auxReg) |= (1<<(auxBit)))
#define isBitSet(auxReg, auxBit)    ((auxReg>>auxBit) & 0x01)
#define isBitClear(auxReg, auxBit)  (!(isBitSet(auxReg, auxBit)))
#define Push_to_LED_Port(v1, v2)    (LED_PORT = (v1 | v2))
#define Loop_Until_Clear(var)       do {} while (var)
#define Is_But_Press(BPin, BBit)    isBitClear(BPIN, BBIT)
#define LSL(reg)                    (reg = reg<<1)
#define LSR(reg)                    (reg = reg>>1)
#define NSL(reg)					(reg = reg<<4)
#define NSR(reg)					(reg = reg>>4)
#define Set_Single_Bit(reg, bit)    (reg = (_BV(bit)))
#define Clr_Single_Bit(reg, bit)    (reg = (~(_BV(bit))))
#define Get_Nibble(num, nib)        ((num >> (nib<<2)) & 0xF)
// Value encoded in BCD => overflows if over 1000, returning to 0
volatile uint16_t cntgm = 0x1802;
volatile uint8_t  bttns = 0x0000;
volatile uint16_t temp	= 0x0000;

void display(uint16_t num);
int main()
{
    // SetUp
    UCSR0B      =   0;          // Disable USArt
    DISP_DDR    =   0xFF;       // Utilizando todas as portas D como saída
    DISP_PORT   =   0x00;       // Set all pins to HIGH
    BUTTON_DDR  =   0x00;       // Set B Port as input for simplicity

    DISP_CTRL_DDR	= 0xFF;     // Set Control Port as Output
    DISP_CTRL_PORT	= 0xFF;     // Set all pins to HIGH

    // Set Pull-up on all button pins
    BUTTON_PORT = 0xFF;
    // Enable interrupt in all button pins
    INT_MSK = 0xFF;
    SetBit(PCICR, INT_PORT);    // Enable PCINT interrupt on PCINT 0-7 (BUTTON_PORT)
    sei();                      // Enable global interrupts

    while(1) {
        // Se espera que o display funcione em 1KHz
        display(cntgm);
        if(bttns) {
            uint16_t addr    = 0x1000;
            // Valor bttns não pode ser alterado aqui
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                while(bttns) {
                    cntgm   += (isBitSet(bttns, 0) ? (addr) : (0));
                    NSR(addr);
                    LSR(bttns);
                }
            }
            // addr = 0x10 - 0x0A = 0x06
            addr    = 0x06;
            temp    = 0x00; // i = 0;
            // cntgm = 0x1110
            // tmp   = 4
            while(temp != 4) {
                cntgm += ((Get_Nibble(cntgm, temp) < 0xA) ? 0 : addr);
                temp++;
                // Shift addr to the next nibble
                NSL(addr);
            }
        }
    }
    return 0;
}

void display(uint16_t num)
{
    //  Se o display é refrescado a cada 1ms (min),
    // cada dígito deve ficar aceso por ~<250us.
    /* ================================== */
    temp = Get_Nibble(cntgm, 3);
    // Select 'M' display
    //DISP_CTRL_PORT = 0b0111;
    Clr_Single_Bit(DISP_CTRL_PORT, 3);
    DISP_PORT = hex2SevenSegments(temp, DISPLAY_COMMON_ANODE, 0);
    _delay_us(245);
    /* ================================== */
    temp = Get_Nibble(cntgm, 2);
    // Select 'C' display
    //DISP_CTRL_PORT = 0b1011;
    Clr_Single_Bit(DISP_CTRL_PORT, 2);
    DISP_PORT = hex2SevenSegments(temp, DISPLAY_COMMON_ANODE, 0);
    _delay_us(245);
    /* ================================== */
    temp = Get_Nibble(cntgm, 1);
    // Select 'D' display
    //DISP_CTRL_PORT = 0b1101;
    Clr_Single_Bit(DISP_CTRL_PORT, 1);
    DISP_PORT = hex2SevenSegments(temp, DISPLAY_COMMON_ANODE, 0);
    _delay_us(245);
    /* ================================== */
    temp = Get_Nibble(cntgm, 0);
    // Select 'U' display
    //DISP_CTRL_PORT = 0b1110;
    Clr_Single_Bit(DISP_CTRL_PORT, 0);
    DISP_PORT = hex2SevenSegments(temp, DISPLAY_COMMON_ANODE, 0);
    _delay_us(245);
    /* ================================== */
}

ISR(BUTTON_INT)
{
    // Buttons are set on '0'
    bttns = ~(BUTTON_PIN);
}
