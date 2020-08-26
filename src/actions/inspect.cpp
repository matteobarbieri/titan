#include "inspect.hpp"

#include "../GamePhase.hpp"
#include "../GameMessages.hpp"

#include "../map/GameMap.hpp"

InspectAction::InspectAction(int x, int y) : x(x), y(y)
{
}

Outcome * InspectAction::_execute()
{

    GamePhase next_phase;

    int target_x = game_map->top_x + x;
    int target_y = game_map->top_y + y;

    // Only allow to inspect things that are within the player's vision
    if (! game_map->fov_map->isInFov(target_x, target_y))
    {
        return nullptr;
    }

    // Retrieve an entity that can be inspected at target location
    Entity * target = game_map->get_inspectable_entity_at(target_x, target_y);

    if (target != nullptr)
    {
        next_phase = ENTITY_INFO;
    }
    else
    {
        next_phase = PLAYERS_TURN;
    }

    Outcome * outcome = new Outcome(next_phase, false, true,\
        target);

    return outcome;
}
