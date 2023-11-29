#include "logic_handler.h"

Collision_e Check_Collision(pontos_t* hitbox)
{
    uint32_t x1 = hitbox->x1;
    uint32_t y1 = hitbox->y1;

    uint32_t x2 = hitbox->x2;
    uint32_t y2 = hitbox->y2;

    Collision_e collision_type;

    if(y1 > UPPER_BORDER) { // Acima da fronteira inferior
        if(y2 < (DISPLAY_H - UPPER_BORDER)) { // Abaixo da fronteira superior
            collision_type = NO_COLLISION;
        } else {
            collision_type = Y_COLLISION;
        }
    } else {
        collision_type = Y_COLLISION;
    }
    // X_COLLISION + Y_COLLISION = XY_COLLISION
    if(x1 > LEFT_BORDER) { // Acima da fronteira inferior
        if(x2 < (DISPLAY_W - LEFT_BORDER)) { // Abaixo da fronteira superior
            collision_type += NO_COLLISION;
        } else {
            collision_type += X_COLLISION;
        }
    } else {
        collision_type += X_COLLISION;
    }

    return collision_type;
}
