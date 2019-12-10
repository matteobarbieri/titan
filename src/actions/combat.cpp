#include "../libtcod.hpp"
#include "combat.hpp"

#include "../utils.hpp"
#include "../GamePhase.hpp"
#include "../Entity.hpp"
#include "../GameMessages.hpp"
#include "../GameState.hpp"

#include "../map/GameMap.hpp"

#include "../components/Equipment.hpp"
#include "../components/Fighter.hpp"

#include "Outcome.hpp"


Outcome * AttackAction::_execute()
{

    Entity * target = game_state->entity_targeted;
    
    GamePhase next_phase = PLAYERS_TURN;
    
    //bool position_changed = false;
    // Attack succeeded means only that the player was able to carry out the
    // attack, not that he necessarily hit
    bool attack_succeeded = false;

    // If no entity has been targeted, warn the player
    if (target == nullptr)
    {
        MessageLog::singleton().add_message(
            {"Select target before attacking!",
            TCODColor::yellow});
    }
    else if (!player->equipment->has_weapon_equipped())
    {
        MessageLog::singleton().add_message(
            {"You have no equipped weapons!",
            TCODColor::yellow});
    }
    else
    {

        //player->interact_with(target, game_map, game_state);

        // TODO might also be able to modify map?
        attack_succeeded = player->fighter->attack(target);
        next_phase = ENEMY_TURN;
    }


    /*
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
    }
    else
    {
        MessageLog::singleton().add_message(
                {"Multiple enemies in range, target one manually!",
                TCODColor::yellow});
    }
    */

    //bool redraw_terrain = position_changed || interacted_with_something;
    bool redraw_terrain = attack_succeeded;
    bool fov_recompute = redraw_terrain;

    // Return outcome
    Outcome * outcome = new Outcome(
        next_phase,
        fov_recompute,
        redraw_terrain);

    return outcome;
}
