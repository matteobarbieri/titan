//#include "libtcod.hpp"

#include "Outcome.hpp"

Outcome::Outcome(
    GamePhase next_phase, bool fov_recompute, bool redraw_terrain,
    Entity * entity_focused, Entity * entity_targeted)
{
    this->next_phase = next_phase;
    this->fov_recompute = fov_recompute;
    this->redraw_terrain = redraw_terrain;
    this->entity_focused = entity_focused;
    this->entity_targeted = entity_targeted;
}
