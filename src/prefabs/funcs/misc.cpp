#include "misc.hpp"

#include "../../libtcod.hpp"

#include "../../map/GameMap.hpp"
#include "../../Entity.hpp"
#include "../../SaveGame.hpp"
#include "../../components/Interactive.hpp"
#include "../../GameMessages.hpp"

#include "../../buffs/BuffStun.hpp"

void unlock_doors(Entity * player, GameMap * game_map, GameState * game_state, unsigned int unlock_doors_id)
{
    if (unlock_doors_id != 0)
    {

        //DEBUG("Unlocking doors with id " << unlock_doors_id);
        //std::vector<Entity *>::iterator e = game_map->entities().begin();

        for (int i=0; i<(int)game_map->entities().size(); i++)
        {
            Entity * e = game_map->entities()[i];

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

Effect * Effect::from_json(json j)
{

    if (j["subclass"] == "UnlockDoorsEffect")
    {
        return UnlockDoorsEffect::from_json(j);
    }

    if (j["subclass"] == "AddLogMessageEffect")
    {
        return AddLogMessageEffect::from_json(j);
    }

    if (j["subclass"] == "StunEnemyOnTrapEffect")
    {
        return StunEnemyOnTrapEffect::from_json(j);
    }

    return nullptr;
}

//////////////////////////////////
////// STUN ENEMY ON TRAP  ///////
//////////////////////////////////

StunEnemyOnTrapEffect::StunEnemyOnTrapEffect(unsigned int group_id, int stun_duration) :
    group_id(group_id), stun_duration(stun_duration)
{
}

void StunEnemyOnTrapEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{

    for (int i=0; i<(int)game_map->entities().size(); i++)
    {
        Entity * aa = game_map->entities()[i];
        if (aa->group_id == group_id)
        {
            Entity * target = game_map->get_inspectable_entity_at(aa->x, aa->y);
            
            if (target != nullptr)
            {
                DEBUG("Applying stun to " << target->name);
                // Apply debuff to target
                target->apply_buff(new BuffStun(target, stun_duration));
            }
        }
    }

}

json StunEnemyOnTrapEffect::to_json()
{
    json j;

    j["subclass"] = "StunEnemyOnTrapEffect";

    j["group_id"] = group_id;
    j["stun_duration"] = stun_duration;

    return j;
}

StunEnemyOnTrapEffect * StunEnemyOnTrapEffect::from_json(json j)
{
    StunEnemyOnTrapEffect * seote = new StunEnemyOnTrapEffect(
        j["stun_duration"],
        j["group_id"]);

    return seote;
}

//////////////////////////////////
/////////// OPEN DOOR ////////////
//////////////////////////////////

UnlockDoorsEffect::UnlockDoorsEffect(unsigned int key_id) : key_id(key_id)
{
}

void UnlockDoorsEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{
    //DEBUG("Unlocking doors with key_id " << key_id);
    unlock_doors(player, game_map, game_state, key_id);
}

json UnlockDoorsEffect::to_json()
{
    json j;

    j["subclass"] = "UnlockDoorsEffect";

    j["key_id"] = key_id;
    return j;
}

UnlockDoorsEffect * UnlockDoorsEffect::from_json(json j)
{
    UnlockDoorsEffect * ude = new UnlockDoorsEffect(
        j["key_id"]);

    return ude;
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
    json j;

    j["subclass"] = "AddLogMessageEffect";

    j["text"] = text;
    j["text_color"] = tcodcolor_to_json(text_color);

    return j;
}

AddLogMessageEffect * AddLogMessageEffect::from_json(json j)
{
    AddLogMessageEffect * alme = new AddLogMessageEffect(
        j["text"],
        json_to_tcodcolor(j["text_color"]));

    return alme;
}
