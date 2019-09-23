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

void UseSkillAction::set_context(
    GameMap * game_map, Entity * player, TCODMap * fov_map,
    GameState * game_state)
{
    // First set context (call parent method)
    Action::set_context(game_map, player, fov_map, game_state);

    // Set the context also for the skill
    skill->set_context(game_map, player, game_state);
}

Outcome * UseSkillAction::_execute()
{
    return skill->use();
}
