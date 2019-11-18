#include "../libtcod.hpp"
#include "combat.hpp"

#include "../utils.hpp"
#include "../GamePhase.hpp"
#include "../Entity.hpp"
#include "../GameMessages.hpp"
#include "../GameState.hpp"

#include "../map/GameMap.hpp"

#include "Outcome.hpp"


Outcome * AttackAction::_execute()
{

    // TODO adapt range based on equipped weapon
    int melee_weapon_range = 1;

    bool position_changed = false;
    bool interacted_with_something = false;

    GamePhase next_phase = PLAYERS_TURN;

    Entity * target = nullptr;
    int n_enemies_in_range = game_map->search_target_in_range(
        player->x, player->y, melee_weapon_range, &target);

    if (n_enemies_in_range == 0)
    {
        MessageLog::singleton().add_message(
                {"No enemies in melee range!",
                TCODColor::yellow});
    }
    else if (n_enemies_in_range == 1)
    {
        next_phase = ENEMY_TURN;
        player->interact_with(target, game_map, game_state);
        interacted_with_something = true;
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
