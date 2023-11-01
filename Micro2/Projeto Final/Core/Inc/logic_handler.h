#ifndef LOGIC_HANDLER_H
#define LOGIC_HANDLER_H

#include "NOKIA5110_fb.h"

/**
 * @brief           Contains game entities, used for indexing
 *
 * @param Character - Player Character
 * @param Octupus   - Octupus, Enemy
 * @param Enemy     - Enemy
 * @param Object    - Rocks, trees, doors, stair, etc.
 * @param Item      - Health, Sword, Shield, etc.
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
 * @brief           Identifies what grid an entity is in, helps with collision detection
 *
 * @param Grid_None
 * @param Grid_Right
 * @param Grid_Up
 * @param Grid_Up_Right
 */
typedef enum {
    Grid_None       = 0,
    Grid_Right      = 1,
    Grid_Up         = 2,
    Grid_Up_Right   = 3  // = Right + Up
} In_Grid_e;

/**
 * @brief
 *
 * @param type      - What entity it is
 * @param grids     - What grids it's in
 * @param q         - Where it is
 */
typedef struct {
    Entity_e  type;
    In_Grid_e grids;
    pontos_t* q;
} Entity_s;

#endif
