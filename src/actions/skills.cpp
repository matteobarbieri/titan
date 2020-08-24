#include <string>
#include <sstream>

#include "skills.hpp"

/*
#include "../GamePhase.hpp"
#include "../Entity.hpp"

#include "../components/Inventory.hpp"
#include "../components/Equipment.hpp"
#include "../components/Item.hpp"

#include "../EquipmentSlots.hpp"

#include "../map/GameMap.hpp"
*/

#include "../Player.hpp"
#include "../skills/Skill.hpp"

#include "../GameState.hpp"
#include "../GameMessages.hpp"

#include "Outcome.hpp"

/*
 _   _            ____  _    _ _ _ 
| | | |___  ___  / ___|| | _(_) | |
| | | / __|/ _ \ \___ \| |/ / | | |
| |_| \__ \  __/  ___) |   <| | | |
 \___/|___/\___| |____/|_|\_\_|_|_|
*/

UseSkillAction::UseSkillAction(Skill * skill) : skill(skill)
{
    
    DEBUG("Use skill action");
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
    DEBUG("Setting context");
}

Outcome * UseSkillAction::_execute()
{
    DEBUG("Executing");
    return skill->use();
}

/*
 ____                 _             ____  _    _ _ _ 
|  _ \ ___  ___  ___ | |_   _____  / ___|| | _(_) | |
| |_) / _ \/ __|/ _ \| \ \ / / _ \ \___ \| |/ / | | |
|  _ <  __/\__ \ (_) | |\ V /  __/  ___) |   <| | | |
|_| \_\___||___/\___/|_| \_/ \___| |____/|_|\_\_|_|_|
*/
//ResolveSkillAction::ResolveSkillAction(Skill * skill) : skill(skill)
ResolveSkillAction::ResolveSkillAction(int x, int y) : x(x), y(y)
{
    
    //Player::singleton().skills
}

void ResolveSkillAction::set_context(
    GameMap * game_map, Entity * player, TCODMap * fov_map,
    GameState * game_state)
{
    // First set context (call parent method)
    Action::set_context(game_map, player, fov_map, game_state);

    // Set the context also for the skill
    // Assumes a skill has been selected previously
    game_state->selected_skill->set_context(game_map, player, game_state);
}

Outcome * ResolveSkillAction::_execute()
{
    return game_state->selected_skill->resolve(x, y);
}
