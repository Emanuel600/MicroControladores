/**
 * @file HD44780.h
 * @author Emanuel Staub
 * @brief LCD Controller
 * @version 0.1
 * @date 2023-08-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef HD44780_H
#define HD44780_H    120

#include <inttypes.h>

#include "Global_Defines.h"
#include "main.h"

/// @addtogroup LCD LCD_Functions
/// @{
/**
 * @brief          LCD commands
 *
 * @param LCD_CLEAR_DISPLAY
 * @param LCD_RETURN_HOME
 * @param LCD_ENTRY_MODE_SET
 * @param LCD_DISPLAY_CONTROL
 * @param LCD_CURSOR_SHIFT
 * @param LCD_FUNCTION_SET
 * @param LCD_SET_CGRAM_ADDR
 * @param LCD_SET_DDRAM_ADDR
 */
typedef enum {
    LCD_CLEAR_DISPLAY           =   0x01,
    LCD_RETURN_HOME             =   0x02,
    LCD_ENTRY_MODE_SET          =   0x04,
    LCD_DISPLAY_CONTROL         =   0x08,
    LCD_CURSOR_SHIFT            =   0x10,
    LCD_FUNCTION_SET            =   0x20,
    LCD_SET_CGRAM_ADDR          =   0x40,
    LCD_SET_DDRAM_ADDR          =   0x80
} HD44780_Command_e;

// flags for display entry mode
typedef enum {
    LCD_ENTRY_RIGHT             =   0x00,
    LCD_ENTRY_LEFT              =   0x02,
    LCD_ENTRY_SHIFT_INCREMENT   =   0x01,
    LCD_ENTRY_SHIFT_DECREMENT   =   0x00
} HD44780_Display_Entry_Mode_e;

/**
 * @brief           Functions for display control
 *
 * @param LCD_DISPLAY_ON
 * @param LCD_DISPLAY_OFF
 * @param LCD_CURSOR_ON
 * @param LCD_CURSOR_OFF
 * @param LCD_BLINK_ON
 * @param LCD_BLINK_OFF
 */
typedef enum {
    LCD_DISPLAY_ON              =   0x04,
    LCD_DISPLAY_OFF             =   0x00,
    LCD_CURSOR_ON               =   0x02,
    LCD_CURSOR_OFF              =   0x00,
    LCD_BLINK_ON                =   0x01,
    LCD_BLINK_OFF               =   0x00
} HD44780_Display_Control_e;

/**
 * @brief           Flags for display/cursor shift
 *
 * @param LCD_DISPLAY_MOVE
 * @param LCD_CURSOR_MOVE
 * @param LCD_MOVE_RIGHT
 * @param LCD_MOVE_LEFT
 */
typedef enum {
    LCD_DISPLAY_MOVE            =   0x08,
    LCD_CURSOR_MOVE             =   0x00,
    LCD_MOVE_RIGHT              =   0x04,
    LCD_MOVE_LEFT               =   0x00
} HD44780_Dispaly_Cursor_Shift_e;

/**
 * @brief           Size of the on-screen characters
 *
 * @param LCD_5x10_DOTS
 * @param LCD_5x8_DOTS
 */
typedef enum {
    LCD_5x10_DOTS               =   0x04,
    LCD_5x8_DOTS                =   0x00
} HD44780_Charsize_e;

/**
 * @brief           Amount of rows on the LCD
 *
 * @param LCD_1_Line
 * @param LCD_2_LINES
 */
typedef enum {
    LCD_1_LINE                  =   0x00,
    LCD_2_LINES                 =   0x08
} HD44780_Lines_e;

/// @}
/// @brief  Wether to add error guards or not
#define ENABLE_ERROR_GUARD              1
#if (ENABLE_ERROR_GUARD == 1)
    #define GUARD_BLOCK(BLOCK)          {BLOCK}
#else
    #define GUARD_BLOCK(BLOCK)          {}
#endif

/**
 * @brief           What mode the LCD is on
 *
 * @param SELECT_COMMAND
 * @param SELECT_CHAR
 */
typedef enum {
    SELECT_COMMAND  = (uint32_t) GPIO_PIN_RESET,
    SELECT_CHAR     = (uint32_t) GPIO_PIN_SET
} Register_Select_e;

/**
 * @brief           How many pins are being used to control the LCD
 *
 * @param CONTROL_WITHOUT_RW
 * @param CONTROL_WITH_RW
 */
typedef enum {
    CONTROL_WITHOUT_RW,         //!< Register Select (RS) and Enable (E)
    CONTROL_WITH_RW,            //!< RS, E, and Read/Write (RW)
} Control_Mode_e;

/**
 * @brief           How many data pins are being used
 *
 * @param LCD_4_BITS
 * @param LCD_8_BITS
 */
typedef enum {
    LCD_4_BITS                  =   0x00,
    LCD_8_BITS                  =   0x10
} Data_Mode_e;

/**
 * @brief           HD44780 - GPIO Definitions
 * @param Data_Port
 * @param Control_Port
 * @param First_Data_Pin    D0, Dx = D0 << x
 * @param RS_Pin            Register Select Pin, E = RS << 1 and RW = RS << 2
 * @param Enable_Pin
 * @param RW_Pin            Read/Write Pin
 */
typedef struct {
    // Ports
    GPIO_TypeDef*   Data_Port;          //!< Data Port
    GPIO_TypeDef*   Control_Port;       //!< Control Port
    // Pins
    GPIO_Pin        First_Data_Pin;
    GPIO_Pin        RS_Pin;             //!< Register Select Pin
    // Modes
    Control_Mode_e  Control_Mode;       //!< How many pins on control port
    Data_Mode_e     Data_Mode;          //!< How many pins on data port
} HD44780_GPIO;

/**
 * @brief           GPIO struct for HD44780
 *
 * @param HD_GPIO               Struct containg pinout
 * @param Display_Function
 * @param Display_Control
 * @param Display_Mode
 * @param lines
 * @param columns
 * @param Control_Mode          How many control pins
 * @param Data_Mode             How many data pins
 * @param initialized
 */
typedef struct {
    HD44780_GPIO    HD_GPIO;            //!< All pins and Ports
    // Functions
    uint8_t         Display_Function;   //!< Pass Function to display
    uint8_t         Display_Control;    //!< Pass Control to display
    uint8_t         Display_Mode;       //!< Set display Mode
    // Characteristics
    uint8_t         lines;              //!< Amount of lines on display
    uint8_t         colunms;            //!< Amount of columns on display
    uint8_t         new_line_offset;    //!< DDRAM address of the second line
} HD44780;

/**
 * @brief           HD44780 Init Function, should be called after setting ports
 *
 * @param lcd       Used screen
 */
void HD44780_Init(HD44780* lcd);

/**
 * @brief           Sets control port pins
 *
 * @param Control_Struct    Struct pointer to set control port
 * @param Port              Control Port Used
 * @param First_Pin         First pin used for control
 * @param CM                Control Mode
 */
void Set_Control_Port(HD44780_GPIO* Control_Struct, GPIO_TypeDef* Port, \
        GPIO_Pin First_Pin, Control_Mode_e CM);

/**
 * @brief           Set data port pins
 *
 * @param Data_Struct   Struct pointer to set dataa port
 * @param Port          Data Port Used
 * @param First_Pin     All pins used
 * @param DM            Data Mode
 */
void Set_Data_Port(HD44780_GPIO* Data_Struct, GPIO_TypeDef* Port, \
        GPIO_Pin First_Pin, Data_Mode_e DM);

/**
 * @brief           Writes a single character to LCD display
 *
 * @param lcd
 * @param c
 */
void HD44780_Put_Char(HD44780* lcd, char c);

/**
 * @brief           Writes a custom char stored in <location>
 *
 * @param lcd
 * @param location  Where the char is stored
 */
void HD44780_Put_Custom_Char(HD44780* lcd, uint8_t location);

/**
 * @brief           Sends a command to *lcd
 *
 * @param lcd
 * @param command
 */
void HD44780_Command(HD44780* lcd, HD44780_Command_e command);

/**
 * @brief           Prints a string to the LCD
 *
 * @param lcd
 * @param str
 *
 * @return          Number of bytes written
 */
size_t HD44780_Print(HD44780* lcd, char* str);

/**
 * @brief           Starts the LCD, called after init
 */
void HD44780_Begin(HD44780* lcd, uint8_t cols, uint8_t rows, HD44780_Charsize_e charsize);

/**
 * @brief           Clears Screen
 */
void HD44780_Clear(HD44780* lcd);

/**
 * @brief           Homes Cursor
 */
void HD44780_Home(HD44780* lcd);

/**
 * @brief           Turns display off
 */
void HD44780_No_Display(HD44780* lcd);

/**
 * @brief           Turns display on
 */
void HD44780_Display(HD44780* lcd);

/**
 * @brief           Turns blinking off
 */
void HD44780_No_Blink(HD44780* lcd);

/**
 * @brief           Turns Blinking on
 */
void HD44780_Blink(HD44780* lcd);

/**
 * @brief           Turns cursor off
 */
void HD44780_No_Cursor(HD44780* lcd);

/**
 * @brief           Turns cursor on
 */
void HD44780_Cursor(HD44780* lcd);

/**
 * @brief           Scrolls to the left (no changes to RAM)
 */
void HD44780_Scroll_Display_Left(HD44780* lcd);

/**
 * @brief           Scrolls to the right (no changes to RAM)
 */
void HD44780_Scroll_Display_Right(HD44780* lcd);

/**
 * @brief           Shifts cursor to the right
 */
void HD44780_Left_To_Right(HD44780* lcd);

/**
 * @brief           Shifts cursor to the left
 */
void HD44780_Right_To_Left(HD44780* lcd);

/**
 * @brief           Turn on autoscroll
 */
void HD44780_Autoscroll(HD44780* lcd);

/**
 * @brief           Turn off autoscroll
 */
void HD44780_No_Autoscroll(HD44780* lcd);

/**
 * @brief           Sets the cursor's position
 *
 * @param column
 * @param line
 */
void HD44780_Set_Cursor(HD44780* lcd, uint8_t column, uint8_t line);

/**
 * @brief           Sends one byte of data to the LCD
 *
 * @param byte
 * @param mode      Wether it's a command or a character
 */
void HD44780_Send(HD44780* lcd, char byte, Register_Select_e mode);

/**
 * @brief           Writes one half-byte on the data pins
 *
 * @param half_byte
 */
void HD44780_Write_4bits(HD44780* lcd, uint8_t half_byte);

/**
 * @brief           Writes one byte on the data pins
 *
 * @param byte
 */
void HD44780_Write_8bits(HD44780* lcd, char byte);

/**
 * @brief           Pulses the enable pin
 */
void HD44780_Pulse_Enable(HD44780* lcd);

/**
 * @brief           Creates a custom char for lcd
 *
 * @param lcd
 * @param location  CGRAMADDR to store the character in
 * @param charmap   Pixel vector, 5 LSB determine the pixels of their row
 */
void HD44780_Create_Char(HD44780* lcd, uint8_t location, uint8_t* charmap);

#endif
