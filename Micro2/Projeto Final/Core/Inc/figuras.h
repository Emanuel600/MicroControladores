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

const figura_s BackGround = {
    84,
    48,
    {
        0x00, 0x01, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0xA0, 0xC0, 0xC0, 0xC0, 0xC0, 0x40,
        0x40, 0x40, 0x40, 0x40, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x0C, 0x7F, 0x40, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
        0x40, 0x40, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10,
        0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x06, 0x02, 0x01, 0x01, 0x01, 0x01,
        0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x80,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40,
        0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x01, 0x01, 0x00, 0x01, 0x07,
        0x1C, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x00,
        0x40, 0x40, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x01,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x06, 0x04, 0x04, 0x04, 0x06, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08,
        0x04, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10,
        0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0x18, 0x08, 0x08, 0x08, 0x08,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x78, 0xC0, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x20,
        0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x82, 0x84, 0x84, 0x88,
        0xC8, 0x50, 0x50, 0x60, 0x60, 0x60, 0xA0, 0xA0, 0x30, 0x10, 0x18, 0x0C, 0x07, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x03, 0x03, 0x03, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x03, 0x01, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1E, 0xF3, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x04,
        0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x00
    }
};

const figura_s Brick = {
    12,
    12,
    {
        0x9F,
        0xF3,
        0x93,
        0x93,
        0x9F,
        0xF3,
        0x93,
        0x93,
        0x9F,
        0xF3,
        0x93,
        0x93,
        0x0F,
        0x0C,
        0x0C,
        0x0C,
        0x0F,
        0x0C,
        0x0C,
        0x0C,
        0x0F,
        0x0C,
        0x0C,
        0x0C
    }
};

const figura_s Left_Wall_Brick = {
    12,
    12,
    {
        0xFF,
        0xFF,
        0xFF,
        0x22,
        0x22,
        0xFF,
        0x88,
        0x88,
        0xFF,
        0x11,
        0x11,
        0xFF,
        0x0F,
        0x0F,
        0x0F,
        0x02,
        0x02,
        0x0F,
        0x08,
        0x08,
        0x0F,
        0x01,
        0x01,
        0x0F
    }
};

const figura_s Upper_Left_Brick = {
    12,
    12,
    {
        0xFF,
        0xFF,
        0xFF,
        0x23,
        0x23,
        0xFF,
        0x9F,
        0x93,
        0x93,
        0xF3,
        0x9F,
        0x9F,
        0x0F,
        0x0F,
        0x0F,
        0x02,
        0x02,
        0x0F,
        0x08,
        0x08,
        0x0F,
        0x01,
        0x01,
        0x0F
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
