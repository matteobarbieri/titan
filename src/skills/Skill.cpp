#include "../Constants.h"

#include <sstream>
//#include <string>

#include "Skill.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include "../actions/Outcome.hpp"
#include "../GamePhase.hpp"
#include "../GameState.hpp"
#include "../Entity.hpp"
#include "../GameMessages.hpp"

#include "../map/GameMap.hpp"

#include "../utils.hpp"

/*
 ____  _    _ _ _ 
/ ___|| | _(_) | |
\___ \| |/ / | | |
 ___) |   <| | | |
|____/|_|\_\_|_|_|
*/

Skill::Skill(const char * name, const char * icon_path, int cooldown_max) :
    icon_path(icon_path), name(name), cooldown_max(cooldown_max), cooldown(0)
{
    icon_texture = NULL;
}

Skill::~Skill()
{
    // Destroy the texturs
    SDL_DestroyTexture(icon_texture);
}


void Skill::load_texture(SDL_Renderer * renderer)
{
    
    // First create a SDL_Surface from the png image
    SDL_Surface * icon_surface = IMG_Load(icon_path);

    // Then create a texture from the surface
    icon_texture = SDL_CreateTextureFromSurface(renderer, icon_surface);

    // Destroy surface as it is no longer needed
    SDL_FreeSurface(icon_surface);
}


SDL_Texture * Skill::get_icon_texture()
{
    return icon_texture;
}

void Skill::set_context(
    GameMap * game_map, Entity * player,
    GameState * game_state)
{
    this->game_map = game_map;
    this->player = player;
    this->game_state = game_state;
}

void Skill::tick_cooldown()
{
    if (cooldown > 0)
    {
        cooldown--;
    }
}

Outcome * Skill::use()
{
    // Check for cooldown
    
    if (cooldown > 0)
    {
        std::ostringstream log_text;

        log_text << name << " is on cooldown (" << cooldown << " turn";
        if (cooldown > 1)
        {
            log_text << "s";
        }

        log_text << ").";

        // Add message to message log
        MessageLog::singleton().add_message(
            {log_text.str(), TCODColor::yellow});

        return nullptr;
    }
    else
    {
        // Simply call actual function
        Outcome * outcome = _use();

        // If everything went well, set cooldown
        // TODO must fix
        cooldown = cooldown_max;

        return outcome;
    }

}

Outcome * Skill::resolve(int x, int y)
{
    // Simply call actual function
    return _resolve(x, y);
}

Outcome * Skill::_resolve(int x, int y)
{
    // default behaviour, not required for most cases
    return nullptr;
}


/*
 ____  _    _ _ _   ____  _ _       _    
/ ___|| | _(_) | | | __ )| (_)_ __ | | __
\___ \| |/ / | | | |  _ \| | | '_ \| |/ /
 ___) |   <| | | | | |_) | | | | | |   < 
|____/|_|\_\_|_|_| |____/|_|_|_| |_|_|\_\
*/


// TODO move somewhere else!
SkillBlink::SkillBlink(const char * name, const char * icon_path, int cooldown_max, int range) :
    Skill(name, icon_path, cooldown_max), Targetable(0, range)
{
}

Outcome * SkillBlink::_use()
{

    //DEBUG("Inside blink use");

    GamePhase next_phase = TARGETING_SKILL;

    // Save skill in game state
    game_state->selected_skill = this;

    //DEBUG("Radius: " << radius << ", range: " << ((Targetable *)this)->range);
    // Build message
    std::ostringstream stringStream;

    stringStream << "Select target ";


    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::yellow});

    // Return outcome
    Outcome * outcome = new Outcome(
        next_phase,
        false,
        false);

    return outcome;
}

Outcome * SkillBlink::_resolve(int x, int y)
{

    bool position_changed = false;

    int source_x, source_y;
    source_x = player->x;
    source_y = player->y;

    x += game_map->top_x;
    y += game_map->top_y;

    //DEBUG("Currently at " << source_x << ", " << source_y);
    //DEBUG("Destination at " << x << ", " << y);

    if (! this->is_in_range(source_x, source_y, x, y))
    {
        MessageLog::singleton().add_message({
                "Out of range.", TCODColor::yellow});

        return nullptr;
    }

    if (! game_map->fov_map->isInFov(x, y))
    {
        MessageLog::singleton().add_message({
                "You must be able to see the destination!",
                TCODColor::yellow});

        return nullptr;
    }

    if (! game_map->is_blocked(x, y))
    {
        Entity * target = get_blocking_entities_at_location(
            game_map->entities(), x, y);

        if (target != nullptr)
        {
            MessageLog::singleton().add_message({"Can't blink here!", TCODColor::yellow});
            //player->interact_with(target, game_map, game_state);
            //interacted_with_something = true;
        }
        else
        {

            // Previous tile is now walkable again
            game_map->fov_map->setProperties(
                player->x, player->y,
                game_map->fov_map->isTransparent(
                    player->x, player->y),
                true);

            // Update player's position
            player->x = x;
            player->y = y;

            // New tile is now not walkable
            game_map->fov_map->setProperties(
                player->x, player->y,
                game_map->fov_map->isTransparent(
                    player->x, player->y),
                false);

            // Change phase to enemy turn manually!
            game_state->game_phase = ENEMY_TURN;

        }
    }
    else
    {
        // Stupid things said when bumping into walls
        // TODO enable message log
        //possible_messages = ['Ouch!', 'Hey!', 'Stop it!']
        //messages.append(
            //Message(random.choice(possible_messages), libtcod.yellow))

        //game_state->message_log->add_message(
            //Message("Ouch!", TCODColor::yellow));
        
        // TODO replace with a random one
        MessageLog::singleton().add_message({
                "Blinking into walls is severely forbidden.",
                TCODColor::yellow});
    }

    // Check if the position has changed
    position_changed = (
        source_x != player->x or source_y != player->y);

    bool redraw_terrain = position_changed;
    bool fov_recompute = redraw_terrain;

    // Return outcome
    Outcome * outcome = new Outcome(
        // ENEMY_TURN, // TODO this is the right one
        game_state->game_phase, // TODO this is the right one
        fov_recompute,
        redraw_terrain);

    //# TODO check terrain/enemies!!!

    return outcome;
}
