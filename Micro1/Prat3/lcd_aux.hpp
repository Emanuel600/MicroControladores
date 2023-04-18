#include <avr/io.h>

#define LCD_DDR             DDRD
#define LCD_PORT            PORTD
#define LCD_FIRST_PIN       PC2

#define LCD_CONTROL_DDR     DDRB
#define LCD_CONTROL_PORT    PORTB
#define LCD_CONTROL_PIN     PINB
#define LCD_CONTROL_E       PB0
#define LCD_RW_PIN          PB1
#define LCD_RS_PIN          PB2

#define ClearDisp()         (LCD_PORT &= 0x01                                  )
#define ReturnHome()        (LCD_PORT &= 0x02                                  )
#define EntryMode(ID, S)    (LCD_PORT &= (0x04 | (ID<<1) | S               )   )
#define Disp_OnOff(D, C, B) (LCD_PORT &= (0x08 | (D<<2)  | (C<<1) | B      )   )
#define CD_Shift(SC, RL)    (LCD_PORT &= (0x10 | (SC<<3) | (RL<<2)         )   )
#define Func_Set(DL, N, F)  (LCD_PORT &= (0x20 | (DL<<4) | (N<<3) | (F<<2) )   )
#define Set_Cg(Addr)        (LCD_PORT &= (0x40 | Addr                      )   )
#define Set_Dd(Addr)        (LCD_PORT &= (0x80 | Addr                      )   )

void lcd_init();
