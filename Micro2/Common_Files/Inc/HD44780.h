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
#define HD44780_H    10

#include <inttypes.h>

#include "Global_Defines.h"
#include "main.h"

/// @addtogroup LCD LCD_Functions
/// @{
// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00
/// @}

/**
 * @brief           What mode the LCD is on
 *
 * @param SELECT_COMMAND
 * @param SELECT_CHAR
 */
typedef enum {
    SELECT_COMMAND,
    SELECT_CHAR
} Register_Select_e;

/**
 * @brief           How many pins are being used to control the LCD
 *
 * @param CONTROL_1_PIN
 * @param CONTROL_2_PINS
 * @param CONTROL_3_PINS
 */
typedef enum {
    CONTROL_1_PIN,      //!< Just Register Select (RS)
    CONTROL_2_PINS,     //!< RS and Read/Write (RW)
    CONTROL_3_PINS      //!< RS, RW and Enable (E)
} Control_Mode_e;

/**
 * @brief           How many data pins are being used
 *
 * @param LCD_4_BITS
 * @param LCD_8_BITS
 */
typedef enum {
    LCD_4_BITS,
    LCD_8_BITS
} Data_Mode_e;

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

    uint8_t         initialized;        //!< If it was initialized properly
} HD44780;
/**
 * @brief           HD44780 - GPIO Definitions
 * @param Data_Port
 * @param Control_Port
 * @param First_Data_Pin    All data pins (D0 | D1 | ... | D7)
 * @param RS_Pin            Register Select Pin
 * @param Enable_Pin
 * @param RW_Pin            Read/Write Pin
 */
typedef struct {
    // Ports
    GPIO_TypeDef    Data_Port;          //!< Data Port
    GPIO_TypeDef    Control_Port;       //!< Control Port
    // Pins
    uint16_t        First_Data_Pin;
    uint16_t        RS_Pin;             //!< Register Select Pin
    // Modes
    Control_Mode_e  Control_Mode;       //!< How many pins on control port
    Data_Mode_e     Data_Mode;          //!< How many pins on data port
} HD44780_GPIO;

/**
 * @brief           HD44780 Init Function, should be called after setting ports
 *
 * @param lcd       Used screen
 * @param Pins      Pin Struct for HD44780
 */
void HD_Init(HD44780 *lcd, HD44780_GPIO Pins);

/**
 * @brief           Sets control port pins
 *
 * @param Control_Struct    Struct pointer to set control port
 * @param Port              Control Port Used
 * @param First_Pin         First pin used for control
 * @param CM                Control Mode
 */
void Set_Control_Port(HD44780_GPIO *Control_Struct, GPIO_TypeDef *Port, uint16_t First_Pin, Control_Mode_e CM);

/**
 * @brief           Set data port pins
 *
 * @param Data_Struct   Struct pointer to set dataa port
 * @param Port          Data Port Used
 * @param First_Pin     All pins used
 * @param DM            Data Mode
 */
void Set_Data_Port(HD44780_GPIO *Data_Struct, GPIO_TypeDef *Port, uint16_t First_Pin, Data_Mode_e DM);

/**
 * @brief           Writes a single character to LCD display
 *
 * @param lcd
 * @param c
 */
void hd44780_put_char(HD44780 *lcd, char c);

/**
 * @brief           Sends a command to *lcd
 *
 * @param lcd
 * @param c
 */
void hd44780_command(HD44780 *lcd, uint8_t c);

/**
 * @brief           Prints a string to the LCD
 *
 * @param lcd
 * @param str
 *
 * @return          Number of bytes written
 */
size_t lcd_print(HD44780 *lcd, char *str);

void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

void clear();
void home();

void noDisplay();
void display();
void noBlink();
void blink();
void noCursor();
void cursor();
void scrollDisplayLeft();
void scrollDisplayRight();
void leftToRight();
void rightToLeft();
void autoscroll();
void noAutoscroll();

void setRowOffsets(int row1, int row2, int row3, int row4);
void setCursor(uint8_t column, uint8_t line);

void send(char byte, uint8_t mode);
void write4bits(uint8_t half_byte);
void write8bits(uint8_t byte);
void pulseEnable();

#endif