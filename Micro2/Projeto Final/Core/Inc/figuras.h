#ifndef FIGURAS_H
#define FIGURAS_H
#include "Global_Defines.h"
#include "main.h"

/**
 * @brief           Structure containing an image to display
 *
 * @param width:    The width of the image
 * @param height:   The height of the image
 * @param pixels:   Array of bools containing the image
 */
typedef struct figura_t figura_s;

const figura_s Cursor = {
    5,
    5,
    {
        0b11011,
        0b10001,
        0b00100,
        0b10001,
        0b11011
    }
};

const figura_s Char_fig = {
    .largura = 14,
    .altura  = 16,
    .pixels = {
        0b00000000,
        0b01111100,
        0b11110000,
        0b11111110,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111110,
        0b11110000,
        0b01111100,
        0b00000000, // Closes first block
        0b00001111, // Opens second block
        0b00011111,
        0b00011111,
        0b01111111,
        0b01111111,
        0b01111111,
        0b00111111,
        0b00111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b00011111,
        0b00011111,
        0b00001111
    }
};

const figura_s nave0f = {
    4,							// largura em pixels = numero de colunas da matriz
    24,							// altura em pixels = cada 8 pixels corresponte a uma linha da matriz
    {
        0b11111111, 0b10000011, 0b10000011, 0b11111111,
        0b11111110, 0b11000001, 0b11000001, 0b11111110,
        0b11111110, 0b11000001, 0b11000001, 0b11111110,
    }
};

#endif
