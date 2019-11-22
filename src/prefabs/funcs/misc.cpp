#include "misc.hpp"

#include "../../libtcod.hpp"

#include "../../map/GameMap.hpp"
#include "../../Entity.hpp"
#include "../../components/Interactive.hpp"
#include "../../GameMessages.hpp"

void unlock_doors(Entity * player, GameMap * game_map, GameState * game_state, unsigned int unlock_doors_id)
{
    if (unlock_doors_id != 0)
    {

        //DEBUG("Unlocking doors with id " << unlock_doors_id);
        //std::vector<Entity *>::iterator e = game_map->entities().begin();

        for (int i=0; i<(int)game_map->entities().size(); i++)
        {
            Entity * e = game_map->entities()[i];

            //DEBUG("Entity name: " << e->name);
            std::size_t found = e->tag.find("door");

            if (
                    found != std::string::npos &&
                    e->interactive != nullptr &&
                    ((InteractiveDoor *)e->interactive)->key_id == unlock_doors_id)
            {
                ((InteractiveDoor *)e->interactive)->unlock();
            }
        }

    }
}

/*********************************
 * Effects
 *********************************/

//////////////////////////////////
/////////// OPEN DOOR ////////////
//////////////////////////////////

UnlockDoorsEffect::UnlockDoorsEffect(unsigned int key_id) : key_id(key_id)
{
}

void UnlockDoorsEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{
    unlock_doors(player, game_map, game_state, key_id);
    // TODO remove
    //DEBUG("Unlocking doors with id " << key_id);
}

json UnlockDoorsEffect::to_json()
{
    // TODO to implement
    json j;

    return j;
}

UnlockDoorsEffect * UnlockDoorsEffect::from_json(json j)
{
    // TODO to implement
    return nullptr;
}

//////////////////////////////////
//////// ADD LOG MESSAGE /////////
//////////////////////////////////

AddLogMessageEffect::AddLogMessageEffect(std::string text, TCODColor text_color) :
    text(text), text_color(text_color)
{
}

void AddLogMessageEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{
    MessageLog::singleton().add_message({text, text_color});
}

json AddLogMessageEffect::to_json()
{
    // TODO to implement
    json j;

    return j;
}

AddLogMessageEffect * AddLogMessageEffect::from_json(json j)
{
    // TODO to implement
    return nullptr;
}
