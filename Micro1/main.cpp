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
#define BUTTON_INT  PCINT0_vect
#define INT_PORT    PCIE0
#define INT_DELAY   255
// Posição do LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
// Funções
#define clrBit(auxReg, auxBit)      ((auxReg) &= (~(1<<(auxBit))))
#define SetBit(auxReg, auxBit)      ((auxReg) |= (1<<(auxBit)))
#define isBitSet(auxReg, auxBit)    ((auxReg>>auxBit) & 0x01)
#define isBitClear(auxReg, auxBit)  (!(isBitSet(auxReg, auxBit)))
#define Push_to_LED_Port(v1, v2)    (LED_PORT = (_BV(v1) | _BV(v2)))
#define Loop_Until_Clear(var)       do {} while (var)
#define Is_But_Press()              isBitClear(BUTTON_PIN, BUTTON_BIT)

volatile uint8_t old_read   = 0;
volatile uint8_t inter      = 0;

volatile uint8_t old_time   = 0;
volatile uint8_t new_time   = 0;

int main()
{
    // SetUp
    UCSR0B      =   0;          // Disable USArt
    LED_DDR     =   0xFF;       // Utilizando todas as portas D como saída
    LED_PORT    =   0x00;       // Set all pins to LOW
    BUTTON_DDR  =   0x00;       // Set B Port as input for simplicity
    SetBit(BUTTON_PORT, BUTTON_BIT);

    sei();                      // Enable global interrupts
    SetBit(PCICR, INT_PORT);    // Enable PCINT interrupt on PCINT 0-7 (BUTTON_PORT)
    SetBit(PCMSK0, BUTTON_BIT); // Set mask only for BUTTON_PIN



    while(1) {
        uint8_t pushed  = 0x01;  // Valor inicial do LED pushed
        uint8_t pusher  = 0x00;  // Valor inicial do LED pusher
        Push_to_LED_Port(pushed, pusher);
        _delay_ms(250);
        pusher++;
        Push_to_LED_Port(pushed, pusher);
        _delay_ms(250);

        while(pusher != 7) {
            if(pusher == pushed) {
                pushed++;
                pusher = 0;
            } else {
                pusher++;
            }
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
    clrBit(PCMSK0, BUTTON_BIT);
    // Anti-repique
    bool reading = Is_But_Press();
    if(reading != old_read) {
        new_time = TCNT0;
    }
    if((TCNT0 - new_time) >= INT_DELAY) {
        (reading !=)
    }

    {
        SetBit(PCMSK0, BUTTON_BIT);
    }

    //Loop_Until_Clear(isBitSet(BUTTON_PORT, BUTTON_BIT));
}
