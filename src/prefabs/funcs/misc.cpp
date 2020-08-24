#include "misc.hpp"

#include "../../libtcod.hpp"

#include "../../map/GameMap.hpp"
#include "../../Entity.hpp"
#include "../../GameState.hpp"
#include "../../SaveGame.hpp"
#include "../../GameMessages.hpp"

#include "../../components/Interactive.hpp"
#include "../../components/Fighter.hpp"

#include "../../buffs/BuffStun.hpp"

#include "../../utils.hpp"

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

    if (j["subclass"] == "ApplyDebuffsOnTrapEffect")
    {
        return ApplyDebuffsOnTrapEffect::from_json(j);
    }

    if (j["subclass"] == "DamageEnemiesInAreaEffect")
    {
        return DamageEnemiesInAreaEffect::from_json(j);
    }

    if (j["subclass"] == "ApplyDebuffsEffect")
    {
        return ApplyDebuffsEffect::from_json(j);
    }

    if (j["subclass"] == "DisplaySFXEffect")
    {
        return DisplaySFXEffect::from_json(j);
    }

    if (j["subclass"] == "MovePlayerEffect")
    {
        return MovePlayerEffect::from_json(j);
    }

    if (j["subclass"] == "EndMissionEffect")
    {
        return EndMissionEffect::from_json(j);
    }

    //if (j["subclass"] == "CompositeEffect")
    //{
        //return CompositeEffect::from_json(j);
    //}

    return nullptr;
}

/////////////////////////////////////
////// APPLY DEBUFFS TO ENTITY //////
/////////////////////////////////////

ApplyDebuffsEffect::ApplyDebuffsEffect(int entity_id,int group_id) :
    entity_id(entity_id), group_id(group_id)
{
}

ApplyDebuffsEffect::ApplyDebuffsEffect(int entity_id) :
    ApplyDebuffsEffect(entity_id, -1)
{
}

void ApplyDebuffsEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{

    //DEBUG("Trying to apply debuff effect");

    for (int i=0; i<(int)game_map->entities().size(); i++)
    {
        Entity * aa = game_map->entities()[i];
        // Entities affected by the trigger are those whose group id matches
        // with the one specified in the effect.

        if (
                (entity_id > -1 && aa->_id == (unsigned int)entity_id) || // Select by entity_id
                (group_id > -1 && aa->group_id == (unsigned int)group_id)    // Select by group
           )
        {
            
            for (int bi=0; bi<(int)buffs.size(); bi++)
            {
                // Must clone the object, otherwise it's always the same
                // instance being used
                buffs[bi]->clone()->apply(aa);
            }
        }

    }

}

json ApplyDebuffsEffect::to_json()
{
    json j;

    j["subclass"] = "ApplyDebuffsEffect";

    j["entity_id"] = entity_id;
    j["group_id"] = group_id;

    // Buffs
    json j_buffs;

    for (int i=0; i<(int)buffs.size(); i++)
    {
        //DEBUG("json: " << buffs[i]->to_json());
        j_buffs.push_back(buffs[i]->to_json());
    }

    j["buffs"] = j_buffs;

    return j;
}

ApplyDebuffsEffect * ApplyDebuffsEffect::from_json(json j)
{
    ApplyDebuffsEffect * ade = new ApplyDebuffsEffect(
        j["entity_id"], j["group_id"]);

    // Buffs
    for (int i=0; i<(int)j["buffs"].size(); i++)
    {
        //Buff * jj = Buff::from_json(j["buffs"][i]);
        //DEBUG("is nullptr? (shouldnt be) " << (jj == nullptr));
        ade->buffs.push_back(Buff::from_json(j["buffs"][i]));
    }
    
    return ade;
}

///////////////////////////////////////
////// APPLY DEBUFFS TO ENTITIES //////
////// CAUGHT ON TRAP /////////////////
///////////////////////////////////////

ApplyDebuffsOnTrapEffect::ApplyDebuffsOnTrapEffect(unsigned int group_id) :
    group_id(group_id)
{
}

void ApplyDebuffsOnTrapEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{

    //DEBUG("Trying to apply debuff effect");

    for (int i=0; i<(int)game_map->entities().size(); i++)
    {
        Entity * aa = game_map->entities()[i];
        // Entities affected by the trigger are those whose group id matches
        // with the one specified in the effect.
        if (aa->group_id == group_id)
        {
            //DEBUG("Found tile on which to apply effect");
            // Apply to entities in the same position as the "trap", if
            // applicable.
            Entity * target = game_map->get_inspectable_entity_at(aa->x, aa->y);
            
            if (target != nullptr)
            {
                //DEBUG("Applying debuffs to " << target->name);

                //DEBUG("Found entity on which to apply effect: " << target->name);
                for (int bi=0; bi<(int)buffs.size(); bi++)
                {
                    // Apply [de]buff to target
                    // Must clone the object, otherwise it's always the same
                    // instance being used
                    
                    //target->apply_buff(buffs[bi]->clone());
                    buffs[bi]->clone()->apply(target);
                }

            }
        }
    }

}

json ApplyDebuffsOnTrapEffect::to_json()
{
    json j;

    j["subclass"] = "ApplyDebuffsOnTrapEffect";

    j["group_id"] = group_id;

    // Buffs
    json j_buffs;

    for (int i=0; i<(int)buffs.size(); i++)
    {
        //DEBUG("json: " << buffs[i]->to_json());
        j_buffs.push_back(buffs[i]->to_json());
    }

    j["buffs"] = j_buffs;

    return j;
}

ApplyDebuffsOnTrapEffect * ApplyDebuffsOnTrapEffect::from_json(json j)
{
    ApplyDebuffsOnTrapEffect * ade = new ApplyDebuffsOnTrapEffect(
        j["group_id"]);

    //DEBUG("Restoring " << (int)j["buffs"].size() << " effects");

    // Buffs
    for (int i=0; i<(int)j["buffs"].size(); i++)
    {
        //Buff * jj = Buff::from_json(j["buffs"][i]);
        //DEBUG("is nullptr? (shouldnt be) " << (jj == nullptr));
        ade->buffs.push_back(Buff::from_json(j["buffs"][i]));
    }
    
    return ade;
}

/////////////////////
//// END MISSION ////
/////////////////////

EndMissionEffect::EndMissionEffect()
{
}

json EndMissionEffect::to_json()
{
    json j;

    j["subclass"] = "EndMissionEffect";

    return j;
}

EndMissionEffect * EndMissionEffect::from_json(json j)
{
    EndMissionEffect * e = new EndMissionEffect();

    return e;
}

void EndMissionEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{
    game_state->game_phase = LEVEL_SUMMARY;
    //game_state->game_phase = PLAYER_DEAD;
    //DEBUG("Ending mission...");
}

////////////////////////////////
//// DAMAGE ENTITIES IN AOE ////
////////////////////////////////

DamageEnemiesInAreaEffect::DamageEnemiesInAreaEffect(int radius, int damage) :
    radius(radius), damage(damage)
{
}

void DamageEnemiesInAreaEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{

    // Correct coordinates
    x = game_map->top_x + x;
    y = game_map->top_y + y;

    // Cycle through a square which contains the area
    // TODO check
    for (int xx=x-radius; xx<=x+radius; xx++)
    {
        for (int yy=y-radius; yy<=y+radius; yy++)
        {
            
            // Check if is in range
            if (l2(x, y, xx, yy ) > radius)
            {
                continue;
            }

            //DEBUG("Applying effect to " << xx << " " << yy);

            // Else apply damage
            Entity * target = game_map->get_inspectable_entity_at(xx, yy);

            if (target != nullptr && target->fighter != nullptr)
            {
                //DEBUG("Found entity: " << target->name);
                target->fighter->take_damage(damage);
                
                // Check if target is dead 
                if (target->fighter->is_dead())
                {
                    // TODO must check other arguments
                    target->die(nullptr, game_map, nullptr);
                }
            }
        }
    }

}

json DamageEnemiesInAreaEffect::to_json()
{
    json j;

    j["subclass"] = "DamageEnemiesInAreaEffect";

    j["radius"] = radius;
    j["damage"] = damage;

    return j;
}

DamageEnemiesInAreaEffect * DamageEnemiesInAreaEffect::from_json(json j)
{
    DamageEnemiesInAreaEffect * deiae = new DamageEnemiesInAreaEffect(
        j["radius"], j["damage"]);

    return deiae;
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
/////////// OPEN DOOR ////////////
//////////////////////////////////

DisplaySFXEffect::DisplaySFXEffect(int symbol, TCODColor color, int x, int y, int duration) :
    symbol(symbol), color(color), duration(duration), x(x), y(y)
{
}

// Extrapolate position from entity
DisplaySFXEffect::DisplaySFXEffect(int symbol, TCODColor color, Entity * e, int duration) : 
    //symbol(symbol), x(e->x), y(e->y)
    DisplaySFXEffect(symbol, color, e->x, e->y, duration)
{
}

void DisplaySFXEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{
    Entity * sfx = new Entity(x, y, symbol, color, "", SFX);
    DelayedRemoveBuff * drm = new DelayedRemoveBuff(duration);
    drm->apply(sfx);

    // TODO delayed remove buffss
    game_map->add_entity(sfx);
}

json DisplaySFXEffect::to_json()
{
    json j;

    j["subclass"] = "DisplaySFXEffect";

    j["symbol"] = symbol;
    j["color"] = tcodcolor_to_json(color);
    j["x"] = x;
    j["y"] = y;
    j["duration"] = duration;
    return j;
}

DisplaySFXEffect * DisplaySFXEffect::from_json(json j)
{
    DisplaySFXEffect * ude = new DisplaySFXEffect(
        j["symbol"],
        json_to_tcodcolor(j["color"]),
        j["x"],
        j["y"],
        j["duration"]
    );

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

/*
 __  __                  ____  _                       
|  \/  | _____   _____  |  _ \| | __ _ _   _  ___ _ __ 
| |\/| |/ _ \ \ / / _ \ | |_) | |/ _` | | | |/ _ \ '__|
| |  | | (_) \ V /  __/ |  __/| | (_| | |_| |  __/ |   
|_|  |_|\___/ \_/ \___| |_|   |_|\__,_|\__, |\___|_|   
                                       |___/ 
*/

MovePlayerEffect::MovePlayerEffect(int x, int y) :
    x(x), y(y)
{
}

void MovePlayerEffect::apply(Entity * player, GameMap * game_map, GameState * game_state)
{
    // TODO check
    player->x = x;
    player->y = y;
}

json MovePlayerEffect::to_json()
{
    json j;

    j["subclass"] = "MovePlayerEffect";

    j["x"] = x;
    j["y"] = y;

    return j;
}

MovePlayerEffect * MovePlayerEffect::from_json(json j)
{
    MovePlayerEffect * mpe = new MovePlayerEffect(j["x"], j["y"]);

    return mpe;
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

