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
#endif
