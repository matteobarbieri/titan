#include <string>
#include <sstream>

#include "ui.hpp"

#include "../GamePhase.hpp"
#include "../Entity.hpp"

#include "../components/Inventory.hpp"
#include "../components/Equipment.hpp"
#include "../components/Item.hpp"

//#include "../GameMessages.hpp"
#include "../GameState.hpp"
#include "../EquipmentSlots.hpp"

#include "../map/GameMap.hpp"

// Required for check_if_still_in_sight
#include "../render_functions.hpp"

#include "Outcome.hpp"

CycleTargetAction::CycleTargetAction(int direction) : direction(direction)
{
}

Outcome * CycleTargetAction::_execute()
{

    //int n_entities = game_map->entities().size();

    std::vector<Entity *> enemies_in_sight;

    // Keep track of currently targeted entity
    int ii=0;
    int current_targeted_entity_index = -1;

    for (int i=0; i<(int)game_map->entities().size(); i++)
    {
        if (game_map->entities()[i]->is_valid_target() && // Must be a valid target
            game_map->entities()[i] != player && // Must not be the player
            entity_in_sight(game_map->fov_map, game_map->entities()[i])) // Must be in sight
        {
            enemies_in_sight.push_back(game_map->entities()[i]);
            if (game_map->entities()[i] == game_state->entity_targeted)
            {
                current_targeted_entity_index = ii;
            }
            ii++;
        }
    }

    // If there are no viable entities in sight, do not change anything
    if (ii == 0)
    {
        return nullptr;
    }

    // Determine the index of the next entity to be targeted
    int next_targeted_index = current_targeted_entity_index + direction;

    // Check extreme indexes
    // Before the first one -> set to the last one
    if (next_targeted_index == -1)
    {
        next_targeted_index = ii - 1;
    }

    // After the last one -> set to 0
    if (next_targeted_index == ii)
    {
        next_targeted_index = 0;
    }

    // Finally, set targeted entity in game state
    game_state->entity_targeted = enemies_in_sight[next_targeted_index];
    
    // Return outcome
    Outcome * outcome = new Outcome(
        game_state->game_phase,
        true,
        true);

    return outcome;
}
