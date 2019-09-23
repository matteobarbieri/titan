#include <string>
#include <sstream>

#include "skills.hpp"

/*
#include "../GamePhase.hpp"
#include "../Entity.hpp"

#include "../components/Inventory.hpp"
#include "../components/Equipment.hpp"
#include "../components/Item.hpp"

#include "../GameState.hpp"
#include "../EquipmentSlots.hpp"

#include "../map/GameMap.hpp"
*/

#include "../Player.hpp"
#include "../skills/Skill.hpp"

#include "../GameMessages.hpp"

#include "Outcome.hpp"

UseSkillAction::UseSkillAction(Skill * skill) : skill(skill)
{
    
    //Player::singleton().skills
}

Outcome * UseSkillAction::_execute()
{

    GamePhase next_phase;

    // Build message
    std::ostringstream stringStream;

    MessageLog::singleton().add_message({skill->name, TCODColor::green});

    // Remove from equipment slots before unequipping
    //if (game_state->selected_inventory_item->item->equipped)
    //{
    //}

    // TODO must come from item's usable component
    next_phase = ENEMY_TURN;
    //next_phase = TARGETING;

    // TODO must come from item's usable component
    //stringStream << "Select target ";

    // Add message to message log
    //MessageLog::singleton().add_message(
        //{stringStream.str(), TCODColor::yellow});

    // Return outcome
    // TODO enable message log
    Outcome * outcome = new Outcome(
        next_phase,
        true,
        true);

    return outcome;
}
