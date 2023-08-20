#include "HD44780.h"
#include "atraso.h"
#include "main.h"

void Set_Control_Port (HD44780_GPIO* Control_Pinout, GPIO_TypeDef* Port, uint16_t First_Pin, Control_Mode_e CM)
{
    Control_Pinout->Control_Mode = CM;
    Control_Pinout->Control_Port = Port;
    Control_Pinout->RS_Pin = First_Pin;
    return;
}

void Set_Data_Port (HD44780_GPIO* Data_Struct, GPIO_TypeDef* Port, uint16_t First_Pin, Data_Mode_e DM)
{
    Data_Struct->Data_Port = Port;
    Data_Struct->Data_Mode = DM;
    Data_Struct->First_Data_Pin = First_Pin;
    return;
}

void HD44780_Init (HD44780* lcd)
{
    if (lcd->HD_GPIO.Data_Mode == LCD_4_BITS) {
        lcd->Display_Function = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    } else {
        lcd->Display_Function = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
    }

    return;
}

void HD44780_Begin (HD44780* lcd, uint8_t cols, uint8_t rows, uint8_t charsize)
{
    if (rows > 1) {
        lcd->Display_Function |= LCD_2LINE;
    }
    lcd->lines = rows;

    lcd->HD_GPIO.offsets.row0 = 0x00;
    lcd->HD_GPIO.offsets.row1 = 0x40;
    lcd->HD_GPIO.offsets.row2 = cols;
    lcd->HD_GPIO.offsets.row3 = 0x40 + cols;


    if ( (charsize != LCD_5x8DOTS) && (rows == 1)) {
        lcd->Display_Function |= LCD_5x10DOTS;
    }

    // Need at least 40ms of delay
    delay_ms (50);
    // Set all pins to low
    Clr_Pin (lcd->HD_GPIO.Control_Port, lcd->HD_GPIO.RS_Pin);
    Clr_Pin (lcd->HD_GPIO.Control_Port, lcd->HD_GPIO.RS_Pin << 1);
    if (lcd->HD_GPIO.Control_Mode == CONTROL_3_PINS) {
        // Clear RW Pin
        Clr_Pin (lcd->HD_GPIO.Control_Port, lcd->HD_GPIO.RS_Pin << 2);
    }
    // Initialize display (4 or 8 bits)
    if (! (lcd->Display_Function & (lcd->HD_GPIO.Data_Mode == LCD_8_BITS))) {
        // Tell HD44780 to use 4 bits, needs to be done thrice
        HD44780_Write_4bits (lcd, 0x03);
        atraso_us (4500);
        HD44780_Write_4bits (lcd, 0x03);
        atraso_us (4500);
        HD44780_Write_4bits (lcd, 0x03);
        atraso_us (150);
        // Set 4 bit interface
        HD44780_Write_4bits (lcd, 0x02);
    } else {
        // Tell HD44780 to use 8 bits, needs to be done twice
        HD44780_Command (lcd, LCD_FUNCTIONSET | lcd->Display_Function);
        atraso_us (4400);
        HD44780_Command (lcd, LCD_FUNCTIONSET | lcd->Display_Function);
        atraso_us (150);
        HD44780_Command (lcd, LCD_FUNCTIONSET | lcd->Display_Function);
    }
    // Set number of lines and rows
    HD44780_Command (lcd, LCD_FUNCTIONSET | lcd->Display_Function);
    // Set cursor to default values
    lcd->Display_Control = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    HD44780_Display (lcd);
    // Clear display
    HD44780_Clear (lcd);
    // Set text direction (left->right)
    lcd->Display_Mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    HD44780_Command (lcd, LCD_ENTRYMODESET | lcd->Display_Mode);
    return;
}

void HD44780_Display (HD44780* lcd)
{
    lcd->Display_Control |= LCD_DISPLAYON;
    HD44780_Command (lcd, lcd->Display_Control | LCD_DISPLAYCONTROL);
}

void HD44780_Clear (HD44780* lcd)
{
    HD44780_Command (lcd, LCD_CLEARDISPLAY);
    delay_ms (2);
    return;
}

void HD44780_Send (HD44780* lcd, char byte, Register_Select_e RS)
{
    const GPIO_TypeDef* Control_Port = lcd->HD_GPIO.Control_Port;
    const uint16_t RS_Pin = lcd->HD_GPIO.RS_Pin;
    // Select register
    Write_Pin ( (GPIO_TypeDef*) Control_Port, RS_Pin, (GPIO_PinState) RS);
    // Clear RW Pin to write command
    if (lcd->HD_GPIO.Control_Mode == CONTROL_3_PINS) {
        Clr_Pin ( (GPIO_TypeDef*) Control_Port, RS_Pin << 2);
    }
    if (lcd->Display_Function & LCD_8_BITS) {
        HD44780_Write_8bits (lcd, byte);
    } else {
        // Send most significant bits first
        HD44780_Write_4bits (lcd, byte >> 4);
        // Least significant last
        HD44780_Write_4bits (lcd, byte);
    }
    return;
}

void HD44780_Command (HD44780* lcd, char command)
{
    HD44780_Send (lcd, command, SELECT_COMMAND);
    return;
}

void HD44780_Put_Char (HD44780* lcd, char c)
{
    HD44780_Send (lcd, c, SELECT_CHAR);
    return;
}

void HD44780_Write_4bits (HD44780* lcd, uint8_t half_byte)
{
    uint16_t D0 = lcd->HD_GPIO.First_Data_Pin;
    for (uint32_t i = 0; i < 4; i++) {
        Write_Pin (lcd->HD_GPIO.Data_Port, D0 << i, (half_byte >> i) & 0x01);
    }

    HD44780_Pulse_Enable (lcd);
    return;
}

void HD44780_Write_8bits (HD44780* lcd, char byte)
{
    for (uint32_t i = 0; i < 8; i++) {
        Write_Pin (lcd->HD_GPIO.Data_Port, lcd->HD_GPIO.First_Data_Pin << i, (byte >> i) & 0x01);
    }

    HD44780_Pulse_Enable (lcd);
    return;
}

void HD44780_Pulse_Enable (HD44780* lcd)
{
    uint16_t E_Pin = lcd->HD_GPIO.RS_Pin;
    E_Pin = E_Pin << 1;
    Clr_Pin (lcd->HD_GPIO.Control_Port, E_Pin);
    atraso_us (1);
    Set_Pin (lcd->HD_GPIO.Control_Port, E_Pin);
    atraso_us (1);
    Clr_Pin (lcd->HD_GPIO.Control_Port, E_Pin);
    atraso_us (60);
    return;
}
