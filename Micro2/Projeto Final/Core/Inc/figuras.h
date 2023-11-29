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


#endif
