#include "../libtcod.hpp"
#include "combat.hpp"

#include "../utils.hpp"
#include "../GamePhase.hpp"
#include "../Entity.hpp"
#include "../GameMessages.hpp"
#include "../GameState.hpp"

#include "../map/GameMap.hpp"

#include "Outcome.hpp"

// TODO move somewhere else

Outcome * AttackAction::_execute()
{

    std::vector<Entity *> enemies_in_range;

    // TODO expand for multiple options depending on player equipment
    // Look for targets in melee range
    // TODO adapt range based on equipped weapon
    int melee_weapon_range = 1;
    for (int x = player->x-melee_weapon_range; x <=player->x+melee_weapon_range; x++)
    {
        for (int y = player->y-melee_weapon_range; y <=player->y+melee_weapon_range; y++)
        {
            // First check if it is a visible tile, to avoid to target stuff
            // over walls with greater melee range
            if (game_map->fov_map->isInFov(x, y))
            {
                Entity * target = get_blocking_entities_at_location(
                    game_map->entities(), x, y);

                // Check if it is actually a monster
                if (target != nullptr && target->ai != nullptr)
                {
                    enemies_in_range.push_back(target);
                }
            }
        }
    }

    bool position_changed = false;
    bool interacted_with_something = false;


    if (enemies_in_range.size() == 0)
    {
        MessageLog::singleton().add_message({"No enemies in melee range!", TCODColor::yellow});
    }
    else if (enemies_in_range.size() == 1)
    {
        player->interact_with(enemies_in_range[0], game_map);
        interacted_with_something = true;
    }
    else
    {
        MessageLog::singleton().add_message({"Multiple enemies in melee range, target one manually!", TCODColor::yellow});
    }

    bool redraw_terrain = position_changed || interacted_with_something;
    bool fov_recompute = redraw_terrain;

    // Return outcome
    Outcome * outcome = new Outcome(
        ENEMY_TURN, // TODO this is the right one
        fov_recompute,
        redraw_terrain);

    return outcome;
}
