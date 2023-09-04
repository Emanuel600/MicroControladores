#include <stdio.h>

#include "HD44780.h"
#include "atraso.h"
#include "main.h"

void Set_Control_Port(HD44780_GPIO* Control_Pinout, GPIO_TypeDef* Port, \
        GPIO_Pin First_Pin, Control_Mode_e CM)
{
    // Error guard
    GUARD_BLOCK(
    if(Control_Pinout == NULL) {
    perrorf("[ERROR] Invalid Pinout Struct\r\n");
        return;
    } else if(!IS_GPIO_ALL_INSTANCE(Port)) {
    perrorf("[ERROR] Invalid GPIO Port\r\n");
        return;
    } else if(!IS_GPIO_PIN(First_Pin)) {
    perrorf("[ERROR] Invalid GPIO Pin For D0\r\n");
        return;
    }
    )
    Control_Pinout->Control_Mode = CM;
    Control_Pinout->Control_Port = Port;
    Control_Pinout->RS_Pin = First_Pin;
    return;
}

void Set_Data_Port(HD44780_GPIO* Data_Struct, GPIO_TypeDef* Port, \
        GPIO_Pin First_Pin, Data_Mode_e DM)
{
    // Error guard
    GUARD_BLOCK(
    if(!Data_Struct) {
    perrorf("[ERROR] Invalid Pinout Struct\r\n");
        return;
    } else if(!IS_GPIO_ALL_INSTANCE(Port)) {
    perrorf("[ERROR] Invalid GPIO Port\r\n");
        return;
    } else if(!IS_GPIO_PIN(First_Pin)) {
    perrorf("[ERROR] Invalid GPIO Pin For D0\r\n");
        return;
    }
    )
    Data_Struct->Data_Port = Port;
    Data_Struct->Data_Mode = DM;
    Data_Struct->First_Data_Pin = First_Pin;
    return;
}

void HD44780_Init(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    if(lcd->HD_GPIO.Data_Mode == LCD_4_BITS) {
        lcd->Display_Function = LCD_4_BITS | LCD_1_LINE | LCD_5x8_DOTS;
    } else {
        lcd->Display_Function = LCD_8_BITS | LCD_1_LINE | LCD_5x8_DOTS;
    }

    return;
}

void HD44780_Begin(HD44780* lcd, uint8_t cols, uint8_t rows, HD44780_Charsize_e charsize)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    } else if(!(cols && rows)) {
    perrorf("[ERROR] Columns and rows must be different from 0\r\n");
        return;
    }
    )
    if(rows - 1) {
        lcd->Display_Function |= LCD_2_LINES;
    }
    lcd->lines = rows;
    lcd->colunms = cols;

    lcd->new_line_offset = 0x40;


    if((charsize != LCD_5x8_DOTS) && (rows == 1)) {
        lcd->Display_Function |= LCD_5x10_DOTS;
    }

    // Need at least 40ms of delay
    delay_ms(50);
    // Set all pins to low
    Clr_Pin(lcd->HD_GPIO.Control_Port, lcd->HD_GPIO.RS_Pin);
    Clr_Pin(lcd->HD_GPIO.Control_Port, lcd->HD_GPIO.RS_Pin << 1);
    if(lcd->HD_GPIO.Control_Mode == CONTROL_WITH_RW) {
        // Clear RW Pin
        Clr_Pin(lcd->HD_GPIO.Control_Port, lcd->HD_GPIO.RS_Pin << 2);
    }
    // Initialize display (4 or 8 bits)
    if(!(lcd->Display_Function & (lcd->HD_GPIO.Data_Mode == LCD_8_BITS))) {
        // Tell HD44780 to use 4 bits, needs to be done thrice
        HD44780_Write_4bits(lcd, 0x03);
        atraso_us(4500);
        HD44780_Write_4bits(lcd, 0x03);
        atraso_us(4500);
        HD44780_Write_4bits(lcd, 0x03);
        atraso_us(150);
        // Set 4 bit interface
        HD44780_Write_4bits(lcd, 0x02);
    } else {
        // Tell HD44780 to use 8 bits, needs to be done twice
        HD44780_Command(lcd, LCD_FUNCTION_SET | lcd->Display_Function);
        atraso_us(4400);
        HD44780_Command(lcd, LCD_FUNCTION_SET | lcd->Display_Function);
        atraso_us(150);
        HD44780_Command(lcd, LCD_FUNCTION_SET | lcd->Display_Function);
    }
    // Set number of lines and rows
    HD44780_Command(lcd, LCD_FUNCTION_SET | lcd->Display_Function);
    // Set cursor to default values
    lcd->Display_Control = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
    HD44780_Display(lcd);
    // Clear display
    HD44780_Clear(lcd);
    // Set text direction (left->right)
    lcd->Display_Mode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
    HD44780_Command(lcd, LCD_ENTRY_MODE_SET | lcd->Display_Mode);

    return;
}

void HD44780_Display(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Control |= LCD_DISPLAY_ON;
    HD44780_Command(lcd, lcd->Display_Control | LCD_DISPLAY_CONTROL);
}

void HD44780_No_Display(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Control &= ~LCD_DISPLAY_ON;
    HD44780_Command(lcd, lcd->Display_Control | LCD_DISPLAY_CONTROL);
}

void HD44780_Clear(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    HD44780_Command(lcd, LCD_CLEAR_DISPLAY);
    delay_ms(2);
    return;
}

void HD44780_Home(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    HD44780_Command(lcd, LCD_RETURN_HOME);
    delay_ms(2);
    return;
}

void HD44780_Send(HD44780* lcd, char byte, Register_Select_e RS)
{
    const GPIO_TypeDef* Control_Port = lcd->HD_GPIO.Control_Port;
    const GPIO_Pin RS_Pin = lcd->HD_GPIO.RS_Pin;
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    if(!(IS_GPIO_ALL_INSTANCE(Control_Port))) {
    perrorf("[ERROR] Invalid control port\r\n");
        return;
    }
    if(!(IS_GPIO_PIN(RS_Pin) || IS_GPIO_PIN(RS_Pin << 1))) {
    perrorf("[ERROR] Invalid control pinout\r\n");
        return;
    }
    if((lcd->HD_GPIO.Control_Mode == CONTROL_WITH_RW) && !(IS_GPIO_PIN(RS_Pin << 2))) {
    perrorf("[ERROR] Invalid RW pin\r\n");
        return;
    }
    )
    // Select register
    Write_Pin((GPIO_TypeDef*) Control_Port, RS_Pin, (GPIO_PinState) RS);
    // Clear RW Pin to write command
    if(lcd->HD_GPIO.Control_Mode == CONTROL_WITH_RW) {
        Clr_Pin((GPIO_TypeDef*) Control_Port, RS_Pin << 2);
    }
    if(lcd->Display_Function & LCD_8_BITS) {
        HD44780_Write_8bits(lcd, byte);
    } else {
        // Send most significant bits first
        HD44780_Write_4bits(lcd, byte >> 4);
        // Least significant last
        HD44780_Write_4bits(lcd, byte);
    }
    return;
}

void HD44780_Command(HD44780* lcd, HD44780_Command_e command)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    HD44780_Send(lcd, command, SELECT_COMMAND);
    return;
}

void HD44780_Put_Char(HD44780* lcd, char c)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    if(c != '\n') {
        HD44780_Send(lcd, c, SELECT_CHAR);
    } else {
        HD44780_Command(lcd, LCD_SET_DDRAM_ADDR | (lcd->new_line_offset));
    }
    return;
}

void HD44780_Put_Custom_Char(HD44780* lcd, uint8_t location)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    } else if(location > 7) {
    perrorf("[ERROR] Invalid CGRAM address, maximum of 7 (Received %u)\r\n", location);
        return;
    }
    )
    HD44780_Send(lcd, location, SELECT_CHAR);
}

size_t HD44780_Print(HD44780* lcd, char* str)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return -1;
    } else if(str == NULL) {
    perrorf("[ERROR] Null pointer for const char\r\n");
        return -1;
    }
    )
    size_t len = 0;
    while(str[len] != '\0') {
        HD44780_Put_Char(lcd, str[len]);
        len++;
    }
    return len;
}

void HD44780_Write_4bits(HD44780* lcd, uint8_t half_byte)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    const GPIO_Pin D0 = lcd->HD_GPIO.First_Data_Pin;
    nuint i = 4;
    do {
        i--;
        Write_Pin(lcd->HD_GPIO.Data_Port, D0 << i, (half_byte >> i) & 0x01);
    } while(i);

    HD44780_Pulse_Enable(lcd);
    return;
}

void HD44780_Write_8bits(HD44780* lcd, char byte)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    nuint i = 8;
    do {
        i--;
        Write_Pin(lcd->HD_GPIO.Data_Port, lcd->HD_GPIO.First_Data_Pin << i, (byte >> i) & 0x01);
    } while(i);

    HD44780_Pulse_Enable(lcd);
    return;
}

void HD44780_Pulse_Enable(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    GPIO_Pin E_Pin = lcd->HD_GPIO.RS_Pin;
    E_Pin = E_Pin << 1;
    Clr_Pin(lcd->HD_GPIO.Control_Port, E_Pin);
    atraso_us(1);
    Set_Pin(lcd->HD_GPIO.Control_Port, E_Pin);
    atraso_us(1);
    Clr_Pin(lcd->HD_GPIO.Control_Port, E_Pin);
    atraso_us(60);
    return;
}

void HD44780_No_Cursor(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Control &= ~LCD_CURSOR_ON;
    HD44780_Command(lcd, LCD_DISPLAY_CONTROL | lcd->Display_Control);
}

void HD44780_Cursor(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Control |= LCD_CURSOR_ON;
    HD44780_Command(lcd, LCD_DISPLAY_CONTROL | lcd->Display_Control);
}

void HD44780_Set_Cursor(HD44780* lcd, uint8_t column, uint8_t row)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    if(row >= lcd->lines) {
        row = lcd->lines - 1;
    }
    HD44780_Command(lcd, LCD_SET_DDRAM_ADDR | (column + (0x40 * row)));
}

void HD44780_Blink(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Control |= LCD_BLINK_ON;
    HD44780_Command(lcd, LCD_DISPLAY_CONTROL | lcd->Display_Control);
}

void HD44780_No_Blink(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Control &= ~LCD_BLINK_ON;
    HD44780_Command(lcd, LCD_DISPLAY_CONTROL | lcd->Display_Control);
}

void HD44780_Left_To_Right(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Mode |= LCD_ENTRY_LEFT;
    HD44780_Command(lcd, lcd->Display_Mode | LCD_ENTRY_MODE_SET);
}

void HD44780_Right_To_Left(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Mode &= ~LCD_ENTRY_LEFT;
    HD44780_Command(lcd, lcd->Display_Mode | LCD_ENTRY_MODE_SET);
}

void HD44780_Scroll_Display_Left(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    // Adjust offset
    if(lcd->new_line_offset == 0x67) {
        lcd->new_line_offset = 0x40;
    } else {
        lcd->new_line_offset++;
    }
    HD44780_Command(lcd, LCD_CURSOR_SHIFT | LCD_DISPLAY_CONTROL | LCD_MOVE_LEFT);
}

void HD44780_Scroll_Display_Right(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    // Adjust offset
    if(lcd->new_line_offset == 0) {
        lcd->new_line_offset = 0x27;
    } else {
        lcd->new_line_offset--;
    }
    HD44780_Command(lcd, LCD_CURSOR_SHIFT | LCD_DISPLAY_CONTROL | LCD_MOVE_RIGHT);
}

void HD44780_Autoscroll(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Mode |= LCD_ENTRY_SHIFT_INCREMENT;
    HD44780_Command(lcd, lcd->Display_Mode | LCD_ENTRY_MODE_SET);
}

void HD44780_No_Autoscroll(HD44780* lcd)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    }
    )
    lcd->Display_Mode &= ~LCD_ENTRY_SHIFT_INCREMENT;
    HD44780_Command(lcd, lcd->Display_Mode | LCD_ENTRY_MODE_SET);
}

void HD44780_Create_Char(HD44780* lcd, uint8_t location, uint8_t* charmap)
{
    GUARD_BLOCK(
    if(lcd == NULL) {
    perrorf("[ERROR] Null pointer for LCD\r\n");
        return;
    } else if(charmap == NULL) {
    perrorf("[ERROR] Null pointer for custom character\r\n");
        return;
    } else if(location > 7) {
    perrorf("[ERROR] Maximum of 8 custom characters\r\n");
        return;
    }
    )
    location &= 0x7;
    HD44780_Command(lcd, LCD_SET_CGRAM_ADDR | (location << 3));
    for(nuint i = 0; i < 8; i++) {
        HD44780_Send(lcd, charmap[i], SELECT_CHAR);
    }
}
