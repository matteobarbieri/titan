#ifndef R20177_OUTCOME
#define R20177_OUTCOME

#include "../libtcod.hpp"
#include "../GamePhase.hpp"

/** The outcome of an action performed by a player 
 */

class Outcome
{
    public:

        GamePhase next_phase;
        bool fov_recompute;
        bool redraw_terrain;

        Outcome(GamePhase next_phase, bool fov_recompute, bool redraw_terrain);

};

#endif
