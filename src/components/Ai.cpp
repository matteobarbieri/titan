#include "Ai.hpp"
#include <sstream>
#include <iostream>

#include "../utils.hpp"
#include "../Entity.hpp"

#include "../map/GameMap.hpp"

#include "../components/Fighter.hpp"

#include "../EquipmentSlots.hpp"
#include "Equipment.hpp"
#include "Equippable.hpp"
#include "Item.hpp"
#include "Reloadable.hpp"
#include "../GameMessages.hpp"

///////////////////////////////////
//////////// AI ACTION ////////////
///////////////////////////////////

AIAction::AIAction(
    Entity * monster, Entity * player, GameMap * game_map) :
        monster(monster), player(player), game_map(game_map)
{
}

void AIAction::execute()
{
    return _execute();
}

void AIAction::_execute()
{
}

AIAction * MonsterAi::pick_action(Entity * player, GameMap * game_map)
{
    return nullptr;
}

///////////////////////////////////
/////////// MONSTER AI ////////////
///////////////////////////////////

MonsterAi::MonsterAi()
{
    state = AIState::IDLE;
}

json MonsterAi::to_json()
{
    json j;

    j["state"] = state;

    return j;
}

MonsterAi * MonsterAi::from_json(json j)
{

    MonsterAi * mai = nullptr;

    if (j["subclass"] == "ImmobileAi")
    {
        mai = ImmobileAi::from_json(j);
    }

    if (j["subclass"] == "SeekerAi")
    {
        mai = SeekerAi::from_json(j);
    }

    if (j["subclass"] == "RangedAi")
    {
        mai = RangedAi::from_json(j);
    }

    // Restore state
    mai->state = j["state"];

    return mai;
}

///////////////////////////////////
////////// IMMOBILE AI ////////////
///////////////////////////////////

ImmobileAi::ImmobileAi() : MonsterAi()
{
}

AIAction * ImmobileAi::pick_action(Entity * player, GameMap * game_map)
{
    return nullptr;
}

json ImmobileAi::to_json()
{
    // Set common properties for AI
    json j = MonsterAi::to_json();

    // Set 
    j["subclass"] = "ImmobileAi";

    return j;
}

ImmobileAi * ImmobileAi::from_json(json j)
{
    ImmobileAi * c = new ImmobileAi();

    // Restore state
    c->state = j["state"];

    return c;
}

///////////////////////////////////
//////////// RANGED AI ////////////
///////////////////////////////////

RangedAi::RangedAi() : MonsterAi()
{
    // Init coordinates of player's last known position to invalid values,
    // meaning that the current monster has no memory of such position.
    player_last_seen_x = -1;
    player_last_seen_y = -1;
}

int RangedAi::getRange()
{
    // TODO extract this from monster's equipment
    return 5;
}


Entity * RangedAi::mustReload()
{
    // If an entity has only weapons that need reloading, return the pointer to
    // the entity which needs to be reloaded.

    // Loop through equipment slots
    std::map<EquipmentSlot, Entity *>::iterator it;
    for (it=owner->equipment->slots.begin(); it!=owner->equipment->slots.end(); ++it)
    {

        // Check that it has a weapon equipped in that slot
        if (it->second != nullptr && it->second->item->is_weapon())
        {

            //if (it->second->item->is_ranged()) // check that it is a ranged weapon
            //{
            //}

            //if (it->second->equippable->reloadable == nullptr || it->second->equippable->reloadable->ammo > 0)
            //{
                //return false;
            //}

            if (it->second->equippable->reloadable != nullptr && it->second->equippable->reloadable->ammo == 0)
            {
                return it->second;
            }

        }

    }

    //return true;
    return nullptr;

}


AIAction * RangedAi::pick_action(Entity * player, GameMap * game_map)
{
    // First check if he sees the player
    
    // Save the distance from player
    float dist = l2(owner->x, owner->y, player->x, player->y);
    bool player_in_sight = false;

    // TODO replace 100 with actual sight range
    if (game_map->aux_fov_map_100->isInFov(owner->x, owner->y) && \
         dist <= 100)
    {
        // Update player's last known position
        player_last_seen_x = player->x;
        player_last_seen_y = player->y;
        player_in_sight = true;

    }

    // If there is a last position
    if (player_last_seen_x != -1 && (owner->x != player_last_seen_x || owner->y != player_last_seen_y))
    {
        // TODO move in separate action object
        
        Entity * weapon = mustReload();

        if (!player_in_sight || dist >= getRange())
        {
            if (weapon != nullptr)
            {
                return new ReloadWeaponAIAction(owner, weapon);
            }
            else
            {
                return new MoveTowardsAIAction(
                    owner, player, game_map,
                    player_last_seen_x, player_last_seen_y);
            }
        }
        else
        {

            if (weapon != nullptr)
            {
                return new ReloadWeaponAIAction(owner, weapon);
            }
            else
            {
                return new AttackPlayerAIAction(owner, player, game_map);
            }
        }

    }

    //DEBUG("I last saw the player at (" << player_last_seen_x << ", " << player_last_seen_y << ")");

    return nullptr;
}

json RangedAi::to_json()
{
    // Set common properties for AI
    json j = MonsterAi::to_json();

    j["subclass"] = "RangedAi";

    // Save properties specific to RangedAi
    j["player_last_seen_x"] = player_last_seen_x;
    j["player_last_seen_y"] = player_last_seen_y;
    j["state"] = state;

    return j;
}

RangedAi * RangedAi::from_json(json j)
{
    RangedAi * c = new RangedAi();

    // Restore state
    c->state = j["state"];

    // Restore properties specific to SeekerAi
    c->player_last_seen_x = j["player_last_seen_x"];
    c->player_last_seen_y = j["player_last_seen_y"];

    return c;
}


///////////////////////////////////
//////////// SEEKER AI ////////////
///////////////////////////////////

SeekerAi::SeekerAi() : MonsterAi()
{
    // Init coordinates of player's last known position to invalid values,
    // meaning that the current monster has no memory of such position.
    player_last_seen_x = -1;
    player_last_seen_y = -1;
}

AIAction * SeekerAi::pick_action(Entity * player, GameMap * game_map)
{
    // First check if he sees the player
    // TODO replace 100 with actual sight range
    
    // Save the distance from player
    float dist = l2(owner->x, owner->y, player->x, player->y);

    if (game_map->aux_fov_map_100->isInFov(owner->x, owner->y) && \
         dist <= 100)
    {
        // Update player's last known position
        player_last_seen_x = player->x;
        player_last_seen_y = player->y;

    }

    // If there is a last position
    if (player_last_seen_x != -1 && (owner->x != player_last_seen_x || owner->y != player_last_seen_y))
    {
        // TODO move in separate action object
        
        if (dist >= 2)
        {
            return new MoveTowardsAIAction(
                owner, player, game_map,
                player_last_seen_x, player_last_seen_y);
        }
        else
        {
            return new AttackPlayerAIAction(owner, player, game_map);
        }

    }

    //DEBUG("I last saw the player at (" << player_last_seen_x << ", " << player_last_seen_y << ")");

    return nullptr;
}

json SeekerAi::to_json()
{
    // Set common properties for AI
    json j = MonsterAi::to_json();

    j["subclass"] = "SeekerAi";

    // Save properties specific to SeekerAi
    j["player_last_seen_x"] = player_last_seen_x;
    j["player_last_seen_y"] = player_last_seen_y;
    j["state"] = state;

    return j;
}

SeekerAi * SeekerAi::from_json(json j)
{
    SeekerAi * c = new SeekerAi();

    // Restore state
    c->state = j["state"];

    // Restore properties specific to SeekerAi
    c->player_last_seen_x = j["player_last_seen_x"];
    c->player_last_seen_y = j["player_last_seen_y"];

    return c;
}

///////////////////////////////////
///// ATTACK PLAYER AI ACTION /////
///////////////////////////////////

void AttackPlayerAIAction::_execute()
{
    monster->fighter->attack(player, game_map);
}

AttackPlayerAIAction::AttackPlayerAIAction(
        Entity * monster, Entity * player, GameMap * game_map) :
    AIAction(monster, player, game_map)
{
}

///////////////////////////////////
///// RELOAD WEAPON AI ACTION /////
///////////////////////////////////

void ReloadWeaponAIAction::_execute()
{
    // In case we need to do something with it
    bool outcome = weapon->equippable->reloadable->reload();


    if (outcome)
    {
    }
    else
    {
        DEBUG("Should not get to this point... [ReloadWeaponAIAction::_execute()]");
    }

    // Build message
    std::ostringstream stringStream;

    //stringStream << "You miss the " << target->name << "!";
    stringStream << "The " << monster->name << " reloads their " << weapon->name << "";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::lightYellow});

}

ReloadWeaponAIAction::ReloadWeaponAIAction(
        Entity * monster, Entity * weapon) :
    AIAction(monster, nullptr, nullptr), weapon(weapon)
{
}

///////////////////////////////////
///// MOVE TOWARDS AI ACTION //////
///////////////////////////////////

MoveTowardsAIAction::MoveTowardsAIAction(
        Entity * monster, Entity * player, GameMap * game_map,
        int target_x, int target_y) :
    AIAction(monster, player, game_map),
    target_x(target_x), target_y(target_y)
{
}

void MoveTowardsAIAction::_execute()
{
    // Save previous state of cell
    bool is_transparent = game_map->fov_map->isTransparent(target_x, target_y);
    bool is_walkable = game_map->fov_map->isWalkable(target_x, target_y);

    // Temporarily set the destination tile as walkable, to allow the
    // computation of the path
    game_map->update_fov_map_properties(target_x, target_y, is_transparent, true);

    // Compute path
    bool res = game_map->path_astar->compute(
    monster->x, monster->y, target_x, target_y);

    // Restore original map properties
    game_map->update_fov_map_properties(target_x, target_y, is_transparent, is_walkable);

    if (!res)
    {
        DEBUG("No path found :(");
        return;
    }

    int new_x, new_y;

    game_map->path_astar->get(0, &new_x, &new_y);

    // Previous tile is now walkable again
    game_map->fov_map->setProperties(
        monster->x, monster->y,
        game_map->fov_map->isTransparent(
            monster->x, monster->y),
        true);

    monster->x = new_x;
    monster->y = new_y;

    // Previous tile is now walkable again
    game_map->fov_map->setProperties(
        monster->x, monster->y,
        game_map->fov_map->isTransparent(
            monster->x, monster->y),
        false);

}
