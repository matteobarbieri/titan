#ifndef R20177_ENTITY
#define R20177_ENTITY

#include "Constants.h"

#include "libtcod.hpp"

// Components
#include "components/Fighter.hpp"
#include "components/Ai.hpp"

/** A container for all consoles that are used in the game, except for the root
 * one, which is accessed directly via TCODConsole::root.
 */
class Entity
{
    public:

        // Entity coordinates in the game map
        int x;
        int y;

        Entity();
        ~Entity();

        // Components
        Fighter * fighter;
        MonsterAi * ai;

};

#endif
