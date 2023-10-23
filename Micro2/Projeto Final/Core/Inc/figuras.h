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
typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t* pixels;
} figure_s;

const struct figura_t block_5x5 = {
    5,
    5,
    {
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111
    }
};

const struct figura_t Cursor = {
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

const struct figura_t Character = {
    .largura = 16,
    .altura =  16,
    .pixels = {
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010
    }
};

const struct figura_t nave0f = {
    4,							// largura em pixels = numero de colunas da matriz
    24,							// altura em pixels = cada 8 pixels corresponte a uma linha da matriz
    {
        0b11111111, 0b10000011, 0b10000011, 0b11111111,
        0b11111110, 0b11000001, 0b11000001, 0b11111110,
        0b11111110, 0b11000001, 0b11000001, 0b11111110,
    }
};

#endif
