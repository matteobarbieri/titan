#include "Ai.hpp"
#include <iostream>

#include "../utils.hpp"
#include "../Entity.hpp"

#include "../map/GameMap.hpp"

#include "../components/Fighter.hpp"

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
    monster->fighter->attack(player);
}

AttackPlayerAIAction::AttackPlayerAIAction(
        Entity * monster, Entity * player, GameMap * game_map) :
    AIAction(monster, player, game_map)
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
    game_map->path_astar->compute(
    monster->x, monster->y, target_x, target_y);

    int new_x, new_y;

    game_map->path_astar->get(0, &new_x, &new_y);
    monster->x = new_x;
    monster->y = new_y;

}
