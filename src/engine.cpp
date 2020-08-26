#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "libtcod.hpp"

#include "Constants.h"

#include "ui/menus.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "GameState.hpp"
#include "GamePhase.hpp"
#include "SaveGame.hpp"
#include "Overseer.hpp"

#include "Consoles.hpp"
#include "actions/Action.hpp"
#include "actions/Outcome.hpp"

#include "buffs/Buff.hpp"
#include "skills/Skill.hpp"

#include "components/Ai.hpp"
#include "components/Fighter.hpp"

#include "input.hpp"
#include "fov_functions.hpp"
#include "render_functions.hpp"


using namespace std;

void tick_buffs(GameMap * game_map);
void tick_cooldown_skills();

void play_game(Entity * player, GameMap * game_map, GameState * game_state, Overseer * overseer)
{
    
    // At the beginning of the game, recompute fov
    bool fov_recompute = true;
    bool redraw_terrain = true;
    bool redraw_entities = true;

    // Initialize fov map
    //DEBUG("Initializing fov map");
    game_map->initialize_fov_map();

    // TODO needs initialization?
    TCOD_key_t key;
    TCOD_mouse_t mouse;

    // TODO is this one really needed?
    TCOD_event_t ev;
    
    // TODO move this?
    int top_x, top_y;

    // Objects required for Player Actions
    Action * action;
    Outcome * outcome;

    // Objects required for Enemies' Actions
    AIAction * entity_action;

    ////////////////////////////////////////////
    /////////////// MAIN LOOP //////////////////
    ////////////////////////////////////////////

    while (!TCODConsole::root->isWindowClosed())
    {

        // Check for a mouse or keyboard event
        ev = TCODSystem::checkForEvent(
            TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse);

        ////////////////////////////////////////////
        /////////// RENDER GAME WINDOW /////////////
        ////////////////////////////////////////////
        
        if (fov_recompute)
        {
            game_map->recompute_fov(player);
        }

        // If the player move, check if targeted entity is still in sight
        if (game_state->entity_targeted != nullptr && redraw_terrain)
        {

            // If no longer in sight, remove target lock
            if (!entity_in_sight(game_map->fov_map, game_state->entity_targeted))
            {
                game_state->entity_targeted = nullptr;
            }

            // Check if by any chance target is dead
            // TODO more generally, if it is no longer targetable for any
            // reason
            
            if (game_state->entity_targeted != nullptr &&
                game_state->entity_targeted->fighter == nullptr)
            {

                game_state->entity_targeted = nullptr;
            }

            // TODO same for focused entity?

        }

        render_all(
            player, game_map, game_map->fov_map,
            fov_recompute, redraw_terrain,
            &mouse, game_state,
            top_x, top_y);

        // TODO find a better place
        game_map->top_x = top_x;
        game_map->top_y = top_y;

        fov_recompute = false;
        redraw_terrain = false;
        redraw_entities = false;

        ////////////////////////////////////////////
        ////////////// PLAYER'S TURN ///////////////
        ////////////////////////////////////////////

        if (game_state->is_players_turn())
        {

            ////////////////////////////////////////////
            ///////////// EXECUTE ACTIONS //////////////
            ////////////////////////////////////////////

            action = handle_input(key, mouse, game_state);

            // Only do something if there actually is an input of some kind
            if (action != nullptr)
            {

                // Add all objects required to perform any action
                action->set_context(
                    game_map, player, game_map->fov_map, game_state);

                //std::cout << "play_game: Checkpoint 8" << std::endl;
                // TODO explicitly destroy action?
                // TODO explicitly destroy outcome?

                // Execute the action
                try {
                    outcome = action->execute();
                } catch(ShowMenuException e) {

                    // Save game
                    SaveGame sg;
                    sg.save("latest.json", player, game_map, game_state, overseer);

                    // Exit to main menu
                    return;
                }
                catch(CancelActionException e) {
                    // TODO check fov_recompute and redraw_terrain
                    outcome = new Outcome(PLAYERS_TURN, true, true);
                }


                ////////////////////////////////////////////
                ///////////// RESOLVE OUTCOME //////////////
                ////////////////////////////////////////////
                
                if (outcome != nullptr)
                {
                    game_state->update(outcome, fov_recompute, redraw_terrain);
                }

                if (fov_recompute)
                {
                    game_map->recompute_fov(player);
                }

            }

        } // if (game_state->is_players_turn())

        ////////////////////////////////////////////
        ////////////// ENEMIES' TURN ///////////////
        ////////////////////////////////////////////
        else if (game_state->is_enemies_turn())
        {

            // Each entity takes a turn
            for (int i=0; i<(int)(game_map->entities().size()); i++)
            {
                if (
                        game_map->entities()[i]->ai != nullptr && // Is a monster
                        !game_map->entities()[i]->is_disabled()) // Is not disabled
                {

                    // Pick an action for each entity
                    entity_action = game_map->entities()[i]->ai->pick_action(
                        player, game_map);

                    // XXX no need to set context, as it was needed previously to
                    // choose the action

                    // Execute the action
                    //if (entity_action != nullptr)
                        //outcome = entity_action->execute();
                    //else
                        //outcome = nullptr;

                    if (entity_action != nullptr)
                    {
                        entity_action->execute();
                    }

                    // Check if player is dead after each entity action
                    // Since the fighter component of an entity is deleted on
                    // death, this is a way to check if the player is dead.
                    if (player->fighter->is_dead())
                    {
                        game_state->game_phase = PLAYER_DEAD;
                        break;
                    }

                    // Side effect of action outcome
                    //if (outcome != nullptr)
                    //{
                        //game_state->update(outcome, fov_recompute, redraw_terrain);
                    //}

                    // TODO destroy entity_action?

                }
            }

            // Resolves buffs effects
            tick_buffs(game_map);

            tick_cooldown_skills();

            ////////////////////////////////////////////
            /////////// CHECK GAME EVENTS  /////////////
            ////////////////////////////////////////////
            
            overseer->trigger_events();
            
            if (game_state->game_phase != PLAYER_DEAD)
            {
                // Go back to player's turn state
                game_state->game_phase = PLAYERS_TURN;
                // Increase turn number
                game_state->current_turn++;
            }

            redraw_entities = true;
            redraw_terrain = true;

        } // if (game_state->is_enemies_turn())

    } // while (!TCODConsole::root->isWindowClosed())

} // play_game()

/**
 * Initialize a new game
 *
 */
void init_new_game(
    GameMap ** game_map, Entity ** player, GameState ** game_state, Overseer ** overseer);

/**
 * Initializes different components of the engine, mainly related to SDL.
 */
void init_engine()
{

    // Init SDL TTF
    if(TTF_Init()==-1) {
        DEBUG("TTF_Init: " << TTF_GetError());
        exit(2);
    }

    // Init SDL Image module with PNG support
    IMG_Init(IMG_INIT_PNG);

    // Set Custom font to use
    TCODConsole::setCustomFont(
            //"data/fonts/16x16-sb-ascii.png",
            "data/fonts/16x16-sb-ascii-extended.png",
            TCOD_FONT_LAYOUT_ASCII_INROW,
            16, 21);

    // Init root console
    TCODConsole::initRoot(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        //WINDOW_TITLE, false, TCOD_RENDERER_GLSL);
        //WINDOW_TITLE, false, TCOD_RENDERER_SDL);
        WINDOW_TITLE, false, TCOD_RENDERER_SDL2);

}

void tick_buffs(GameMap * game_map)
{
    // Tick buffs for entities
    unsigned int n_entities = game_map->entities().size();

    // Must copy list of entities because some of them might be deleted during
    // the loop.
    std::vector<Entity *> entities_list;

    for (unsigned int ei=0; ei<game_map->entities().size(); ei++)
    {
        entities_list.push_back(game_map->entities()[ei]);
    }

    //for (unsigned int ei=0; ei<game_map->entities().size(); ei++)
    for (unsigned int ei=0; ei<n_entities; ei++)
    {

        // Shortcut to entity
        //Entity * e = game_map->entities()[ei];
        Entity * e = entities_list[ei];

        // Refer to this for deleting while iterating:
        // https://stackoverflow.com/questions/3901356/deleting-while-iterating

        //if (e->buffs.size() > 0) 
        //{
            //DEBUG(e->name << " has " << e->buffs.size() << " buffs active" );
        //}

        // Tick each buff.
        std::vector<Buff *>::iterator b = e->buffs.begin();
        while (b != e->buffs.end())
        {
            // Tick
            (*b)->tick();

            // Remove buff if it has expired
            if ((*b)->has_expired())
            {
                (*b)->expire(game_map);
                delete (*b);
                b = e->buffs.erase(b);
            }
            else
            {
                ++b;
            }
        }
    }
}


void tick_cooldown_skills()
{
    std::vector<Skill *>::iterator s = Player::singleton().skills.begin();
    while (s != Player::singleton().skills.end())
    {
        // Tick cooldown
        (*s)->tick_cooldown();
        ++s;
    }
}


int main(int argc, char *argv[])
{

    init_engine();

    // Load the background image
    TCODImage main_menu_background_image("menu_background2.png");

    TCOD_key_t key;
    TCOD_mouse_t mouse;

    // TODO is this one really needed?
    TCOD_event_t ev;

    bool exit_game = false;
    bool play_game_ = false;
    bool load_game = false;

    Entity * player;
    GameMap * game_map;
    GameState * game_state;
    Overseer * overseer;

    while (!TCODConsole::root->isWindowClosed())
    {

        // Check for a mouse or keyboard event
        ev = TCODSystem::checkForEvent(
            TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &key, &mouse);

        // Handle input
        // TODO also handle the other cases
        switch(handle_main_menu(key))
        {
            case 'a':
                play_game_ = true;
                break;
            case 'b':
                load_game = true;
                break;
            case 'c':
                exit_game = true;
                break;

        }

        // Simply break the loop and exit the program
        if (exit_game)
        {
            break;
        }

        // Continue last game
        if (load_game)
        {
            // TODO implement
            // Load game
            SaveGame sg;
            sg.load("latest.json", &player, &game_map, &game_state, &overseer);

            play_game(player, game_map, game_state, overseer);

            load_game = false;
            //break;
        }

        if (play_game_)
        {

            // Start a new game
            //game_phase = GamePhase.PLAYERS_TURN
            
            init_new_game(
                 &game_map,  &player,  &game_state, &overseer);

            //game_map.export_txt('maps_txt/lastmap.txt')

            //show_main_menu = False
            play_game(player, game_map, game_state, overseer);

            // When returning to main menu, reset play_game variable to false
            play_game_ = false;
        }


        // Display the main title menu
        main_menu(&main_menu_background_image);
        
        TCODConsole::flush();
    }

    return 0;
}
