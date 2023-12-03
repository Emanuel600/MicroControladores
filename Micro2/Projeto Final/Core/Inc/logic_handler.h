#ifndef LOGIC_HANDLER_H
#define LOGIC_HANDLER_H

#include "NOKIA5110_fb.h"

typedef struct pontos_t pontos_t;
// Dimensões do Display
#define DISPLAY_W       83U // Width
#define DISPLAY_H       47U // Height
// Espessura das fronteiras
#define UPPER_BORDER    8U
#define LEFT_BORDER     7U

/**
 * @brief           Contains game entities, used for indexing
 *
 * @param Character Player Character
 * @param Octupus   Octupus, Enemy
 * @param Enemy     Enemy
 * @param Object    Rocks, trees, doors, stair, etc.
 * @param Item      Health, Sword, Shield, etc.
 */
typedef enum {
    Character,
    Octopus,
    Enemy,
    Object,
    Item
} Entity_e;

/**
 * @brief           Generic statuses that entities can have
 *
 * @param Static
 * @param Moving
 * @param Attacking
 * @param Damaged
 */
typedef enum {
    Static,
    Moving,
    Attacking,
    Damaged
} General_Status_e;

/**
 * @brief           Facing directions of an object
 *
 * @param Up
 * @param Down
 * @param Left
 * @param Right
 */
typedef enum {
    Up      = 0,
    Down    = 1,
    Left    = 2,
    Right   = 4
} Direction_e;

/**
 * @brief               Directions of a collision
 *
 * @param NO_COLLISION
 */
typedef enum {
    NO_COLLISION,
    X_COLLISION,
    Y_COLLISION,
    XY_COLLISION,
    Player_Collision,
    Weapon_Collision
} Collision_e;

/**
 * @brief
 *
 * @param status    The status of the entity
 * @param direction Facing direction
 * @param type      What entity it is
 * @param hitbox    Hitbox (x1, y1), (x2, y2) rectangle
 */
typedef struct {
    General_Status_e status;
    Direction_e direction;
    Entity_e  type;
    pontos_t* hitbox;
} Entity_s;

Collision_e Check_Collision(pontos_t* hitbox);

Collision_e Check_For_Player(pontos_t* hitbox);

pontos_t get_player_hitbox(void);

void set_player_hitbox(pontos_t* h);

uint32_t get_player_health(void);

uint32_t dec_player_health(void);

#endif
