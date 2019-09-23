#include <iostream>

#include "SkillStun.hpp"

#include "../actions/Outcome.hpp"
#include "../GameMessages.hpp"
#include "../GamePhase.hpp"
#include "../Entity.hpp"

#include "../map/GameMap.hpp"

SkillStun::SkillStun(const char * name, const char * icon_path) :
    Skill(name, icon_path)
{
    range = 1;
}

Outcome * SkillStun::_use()
{

    GamePhase next_phase = PLAYERS_TURN;

    Entity * target = nullptr;

    bool position_changed = false;
    bool interacted_with_something = false;

    int n_enemies_in_range = game_map->search_target_in_range(
        player->x, player->y, range, &target);

    if (n_enemies_in_range == 0)
    {
        MessageLog::singleton().add_message(
                {"No enemies in range!",
                TCODColor::yellow});
    }
    else if (n_enemies_in_range == 1)
    {
        next_phase = ENEMY_TURN;
        MessageLog::singleton().add_message(
                {"Stun!",
                TCODColor::green});
        //player->interact_with(target, game_map);
        //interacted_with_something = true;
    }
    else
    {
        MessageLog::singleton().add_message(
                {"Multiple enemies in melee range, target one manually!",
                TCODColor::yellow});
    }

    bool redraw_terrain = position_changed || interacted_with_something;
    bool fov_recompute = redraw_terrain;

    // Return outcome
    Outcome * outcome = new Outcome(
        next_phase,
        fov_recompute,
        redraw_terrain);

    return outcome;
}

