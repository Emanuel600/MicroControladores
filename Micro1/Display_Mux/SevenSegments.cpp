/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			sevenSegments.c
 * Module:			Seven segments display controller
 * Author:			Leandro Schwarz
 * Version:			1.2
 * Last edition:	2016-01-15
 * Purpose:			Seven segments display controller with support to common
 *					anode and common cathode displays. Special characters dash
 *					(0xFD), U (0xFE) and display off (0xFF) were also implented
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "sevenSegments.hpp"
#if __SEVENSEGMENTS_H != 12
#error Error 101 - Version mismatch on header and source code files (sevenSegments).
#endif

/* -----------------------------------------------------------------------------
 * Function:	hex2SevenSegments
 * Purpose:		Seven segments display decoder
 * Arguments:	hexValue		hex number or special character value
 *				displayType		Display type (use enumerations at displayType_t)
 *				point			ON or OFF
 * Returns:		decoded data value
 * -------------------------------------------------------------------------- */

uint8_t hex2SevenSegments(uint8_t hexValue, displayType_t displayType, uint8_t point)
{
    uint8_t data = 0;

    switch(hexValue) { // PGFEDCBA
    case 0x00:	data = 0b00111111;	break;
    case 0x01:	data = 0b00000110;	break;
    case 0x02:	data = 0b01011011;	break;
    case 0x03:	data = 0b01001111;	break;
    case 0x04:	data = 0b01100110;	break;
    case 0x05:	data = 0b01101101;	break;
    case 0x06:	data = 0b01111101;	break;
    case 0x07:	data = 0b00000111;	break;
    case 0x08:	data = 0b01111111;	break;
    case 0x09:	data = 0b01101111;	break;
    case 0x0A:	data = 0b01110111;	break;
    case 0x0B:	data = 0b01111100;	break;
    case 0x0C:	data = 0b00111001;	break;
    case 0x0D:	data = 0b01011110;	break;
    case 0x0E:	data = 0b01111001;	break;
    case 0x0F:	data = 0b01110001;	break;
    case DISPLAY_DASH:	data = 0b01000000;	break;	// Dash
    case DISPLAY_U:		data = 0b00111110;	break;	// U
    case DISPLAY_OFF:	data = 0b00000000;	break;	// OFF
    }
    if(point) {
        data |= (1 << 7);
    }
    data = (displayType == DISPLAY_COMMON_CATHODE) ? data : (~data);

    return data;
}
