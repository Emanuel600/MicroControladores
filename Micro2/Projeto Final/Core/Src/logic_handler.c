#include "logic_handler.h"

uint32_t Check_Collision(pontos_t* hitbox)
{
    uint32_t x1 = hitbox->x1;
    uint32_t y1 = hitbox->y1;

    uint32_t x2 = hitbox->x2;
    uint32_t y2 = hitbox->y2;

    if(y1 > UPPER_BORDER) { // Acima da fronteira inferior
        if(y2 < (DISPLAY_H - UPPER_BORDER)) { // Abaixo da fronteira superior
            return 0;
        } else {
            return 1;
        }
    } else {
        return 1;
    }

    return 0;
}
