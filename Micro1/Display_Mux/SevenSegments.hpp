/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			sevenSegments.h
 * Module:			Seven segments display controller
 * Author:			Leandro Schwarz
 * Version:			1.2
 * Last edition:	2016-01-15
 * Purpose:			Seven segments display controller with support to common
 *					anode and common cathode displays. Special characters dash
 *					(0xFD), U (0xFE) and display off (0xFF) were also implented
 * -------------------------------------------------------------------------- */

#ifndef __SEVENSEGMENTS_H
#define __SEVENSEGMENTS_H 12

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------
#include <stdint.h>
// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define DISPLAY_DASH	0xFD
#define DISPLAY_U		0xFE
#define DISPLAY_OFF		0xFF

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef enum displayType_t {
    DISPLAY_COMMON_ANODE	= 0,
    DISPLAY_COMMON_CATHODE
} displayType_t;

// -----------------------------------------------------------------------------
// Functions declarations ------------------------------------------------------

uint8_t hex2SevenSegments(uint8_t hexValue, displayType_t displayType, uint8_t point);

#endif
