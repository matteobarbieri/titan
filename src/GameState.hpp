#ifndef R20177_GAME_STATE
#define R20177_GAME_STATE

#include "Constants.h"
#include "Entity.hpp"

#include "libtcod.hpp"

/** A container for all consoles that are used in the game, except for the root
 * one, which is accessed directly via TCODConsole::root.
 */
class GameState
{
    public:

        // Entity being inspected
        Entity * entity_focused = NULL;

        // Entity being targeted
        Entity * entity_targeted = NULL;

        // Inventory item being selected
        Entity * selected_inventory_item = NULL;

        GameState();
        ~GameState();

};

#endif

