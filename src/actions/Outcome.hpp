#ifndef R20177_OUTCOME
#define R20177_OUTCOME

#include "../libtcod.hpp"
#include "../GamePhase.hpp"

// Forward declarations
class Entity;

/** The outcome of an action performed by a player 
 */

class Outcome
{
    public:

        GamePhase next_phase;
        bool fov_recompute;
        bool redraw_terrain;

        Entity * entity_focused;
        Entity * entity_targeted;
        Entity * selected_inventory_item;

        Outcome(GamePhase next_phase, bool fov_recompute, bool redraw_terrain,
                Entity * entity_focused=nullptr, Entity * entity_targeted=nullptr);

};

#endif
