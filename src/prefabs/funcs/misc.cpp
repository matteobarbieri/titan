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

    if (j["subclass"] == "ApplyDebuffsEffect")
    {
        return ApplyDebuffsEffect::from_json(j);
    }

    //if (j["subclass"] == "CompositeEffect")
    //{
        //return CompositeEffect::from_json(j);
    //}

    return nullptr;
}

//////////////////////////////////
////// STUN ENEMY ON TRAP  ///////
//////////////////////////////////

ApplyDebuffsEffect::ApplyDebuffsEffect(unsigned int group_id) :
    group_id(group_id)
{
}

void ApplyDebuffsEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{

    for (int i=0; i<(int)game_map->entities().size(); i++)
    {
        Entity * aa = game_map->entities()[i];
        // Entities affected by the trigger are those whose group id matches
        // with the one specified in the effect.
        if (aa->group_id == group_id)
        {
            // Apply to entities in the same position as the "trap", if
            // applicable.
            Entity * target = game_map->get_inspectable_entity_at(aa->x, aa->y);
            
            if (target != nullptr)
            {
                //DEBUG("Applying debuffs to " << target->name);

                for (int bi=0; bi<(int)buffs.size(); bi++)
                {
                    // Apply [de]buff to target
                    // Must clone the object, otherwise it's always the same
                    // instance being used
                    target->apply_buff(buffs[bi]->clone());
                }

            }
        }
    }

}

json ApplyDebuffsEffect::to_json()
{
    json j;

    j["subclass"] = "ApplyDebuffsEffect";

    j["group_id"] = group_id;

    // Buffs
    json j_buffs;

    for (int i=0; i<(int)buffs.size(); i++)
    {
        j_buffs.push_back(buffs[i]->to_json());
    }

    j["buffs"] = j_buffs;

    return j;
}

ApplyDebuffsEffect * ApplyDebuffsEffect::from_json(json j)
{
    ApplyDebuffsEffect * ade = new ApplyDebuffsEffect(
        j["group_id"]);

    // Buffs
    for (int i=0; i<(int)j["buffs"].size(); i++)
    {
        ade->buffs.push_back(Buff::from_json(j["buffs"][i]));
    }
    
    return ade;
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

//////////////////////////////////
/////////// Composite ////////////
//////////////////////////////////

/*
CompositeEffect::CompositeEffect()
{
}

void CompositeEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{

    // Apply all effects
    for (int i=0; i<(int)effects.size(); i++)
    {
        effects[i]->apply(player, game_map, game_state);
    }
}

json CompositeEffect::to_json()
{
    json j;

    j["subclass"] = "CompositeEffect";

    // Effects
    json j_effects;

    for (int i=0; i<(int)effects.size(); i++)
    {
        j_effects.push_back(effects[i]->to_json());
    }

    j["effects"] = j_effects;


    return j;
}

CompositeEffect * CompositeEffect::from_json(json j)
{
    CompositeEffect * ce = new CompositeEffect();

    // Effects
    for (int i=0; i<(int)j["effects"].size(); i++)
    {
        ce->effects.push_back(Effect::from_json(j["effects"][i]));
    }
    
    return ce;
}
*/

