#include "logic_handler.h"

static volatile pontos_t player_hitbox;

static uint32_t player_status = 2;

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
/**
 * @brief
 *
 * @param hitbox
 * @return Collision_e
 *
 * X*************
 * *            * => X = (x1, y1), Y = (x2, y2)
 * *************Y
 */
Collision_e Check_For_Player(pontos_t* hitbox)
{
    uint32_t px1 = player_hitbox.x1;
    uint32_t py1 = player_hitbox.y1;

    uint32_t px2 = player_hitbox.x2;
    uint32_t py2 = player_hitbox.y2;

    uint32_t x1 = hitbox->x1;
    uint32_t y1 = hitbox->y1;

    uint32_t x2 = hitbox->x2;
    uint32_t y2 = hitbox->y2;

    if(y2 < py1) { // hitbox acima do player
        return NO_COLLISION;
    }
    if(y1 > py2) { // hitbox abaixo do player
        return NO_COLLISION;
    }

    if(x2 < px1) { // hitbox à esquerda do player
        return NO_COLLISION;
    } if(x1 > px2) {// hitbox à direita do player
        return NO_COLLISION;
    }
    return Player_Collision;
}

void set_player_hitbox(pontos_t* h)
{
    player_hitbox = *h;
}

pontos_t get_player_hitbox(void)
{
    return player_hitbox;
}

void dec_player_status(pontos_t* h)
{
    if(!player_status) {
        Game_Over();
    }
    player_status--;
}

uint32_t get_player_status(void)
{
    return player_status;
}
