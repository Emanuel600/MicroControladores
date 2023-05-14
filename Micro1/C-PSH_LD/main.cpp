#define F_CPU 16000000UL // Define para "util/delay"

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
// Posição do botão
#define BUTTON_DDR  DDRB
#define BUTTON_PORT PORTB
#define BUTTON_PIN  PINB
#define BUTTON_BIT  PB2
// Interrupção do botão
#define BUTTON_INT  PCINT0_vect
#define INT_PORT    PCIE0
#define INT_REG     PCICR
#define INT_MSK     PCMSK0
// Timer - Normal, with a prescaler of 256
#define Time_Reg	TCNT1
#define Time_Cont	TCCR1B
#define Time_IntReg TIMSK1
#define Time_Int	TIMER1_OVF_vect
#define Prescaler	_BV(3)
#define INT_DELAY   249
#define TIM_MAX		((1<<16)-1)
// Posição do LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
// Funções
#define clrBit(auxReg, auxBit)      ((auxReg) &= (~(1<<(auxBit))))
#define SetBit(auxReg, auxBit)      ((auxReg) |= (1<<(auxBit)))
#define isBitSet(auxReg, auxBit)    ((auxReg>>auxBit) & 0x01)
#define isBitClear(auxReg, auxBit)  (!(isBitSet(auxReg, auxBit)))
#define Push_to_LED_Port(v1, v2)    (LED_PORT = (v1 | v2))
#define Loop_Until_Clear(var)       do {} while (var)
#define Is_But_Press()              isBitClear(BUTTON_PIN, BUTTON_BIT)
#define LSL(reg)                    (reg = reg<<1)

volatile uint8_t old_read   = 0;
volatile uint8_t gbl_read	= 0;

volatile uint16_t old_time   = 0;
volatile uint16_t new_time   = 0;

volatile uint8_t inter = 0;
int main()
{
    // SetUp
    __label__ push0;
    UCSR0B      =   0;          // Disable USArt
    LED_DDR     =   0xFF;       // Utilizando todas as portas D como saída
    LED_PORT    =   0x00;       // Set all pins to LOW
    BUTTON_DDR  =   0x00;       // Set B Port as input for simplicity
    // Set Pull-up on button
    SetBit(BUTTON_PORT, BUTTON_BIT);

    SetBit(INT_MSK, BUTTON_BIT);// Set mask only for BUTTON_PIN
    SetBit(INT_REG, INT_PORT);  // Enable PCINT interrupt on PCINT 0-7 (BUTTON_PORT)
    sei();                      // Enable global interrupts

    SetBit(Time_IntReg, TOIE1); // Enable overflow interrupt
    // clock/256
    SetBit(Time_Cont, CS12);

    while(1) {
        uint8_t pushed  = 0x02;  // Valor inicial do LED pushed
push0:
        uint8_t pusher  = 0x01;  // Valor inicial do LED pusher
        Loop_Until_Clear(inter);
        Push_to_LED_Port(pushed, pusher);
        _delay_ms(250);
        LSL(pusher);
        Loop_Until_Clear(inter);
        Push_to_LED_Port(pushed, pusher);
        _delay_ms(250);

        while(pusher != _BV(7)) {
            if(pusher == pushed) {
                LSL(pushed);
                pusher = 0;
            } else {
                if(pusher == 0) {
                    goto push0;
                }
                LSL(pusher);
            }
            Loop_Until_Clear(inter);
            Push_to_LED_Port(pushed, pusher);
            _delay_ms(250);
        }
    }
    return 0;
}

/*
void debounceDid( ) { function debounceDid
  ////debounce DigitalDebounce
  int reading = digitalRead(didPin);
  if (reading != oldDidStatus) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime)>= debounceDelay) {
    if (reading != didStatus) {
      didStatus = reading;
      Serial.print(F("Sensor state : ")); Serial.println(didStatus);
    }
  }
  oldDidStatus = reading;
}
*/

ISR(BUTTON_INT)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        uint8_t but_stat = Is_But_Press();
        // Anti-repique (10ms)
        if((Time_Reg - old_time) >= INT_DELAY) {
            inter       = !inter;
            old_time    = Time_Reg;
        }
        old_read = but_stat;
    }
}
// Overflow do timer1, reseta valores pertinentes ao anti-repique
ISR(Time_Int)
{
    old_time = 0;
    Time_Reg = 0;
}
