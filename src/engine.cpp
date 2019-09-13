#include "Constants.h"

#include "ui/menus.hpp"
#include "Entity.hpp"
#include "GameState.hpp"
#include "GamePhase.hpp"
#include "SaveGame.hpp"

#include "Consoles.hpp"
#include "actions/Action.hpp"

#include "components/Ai.hpp"
#include "components/Fighter.hpp"

#include "input.hpp"
#include "fov_functions.hpp"
#include "render_functions.hpp"

#include "libtcod.hpp"
#include "libtcod/console/console.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "sdl/custom_renderer.cpp"

using namespace std;


void play_game(Entity * player, GameMap * game_map, GameState * game_state)
{
    
    // At the beginning of the game, recompute fov
    bool fov_recompute = true;
    bool redraw_terrain = true;
    bool redraw_entities = true;

    // Initialize fov map
    //TCODMap * fov_map = initialize_fov(game_map);
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
        if (game_state->entity_targeted != 0 && redraw_terrain)
        {

            game_state->entity_targeted = check_if_still_in_sight(
                game_map->fov_map, game_state->entity_targeted);

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
                    sg.save("latest.json", player, game_map, game_state);

                    // Exit to main menu
                    return;
                }

                ////////////////////////////////////////////
                ///////////// RESOLVE OUTCOME //////////////
                ////////////////////////////////////////////
                
                if (outcome != 0)
                {
                    game_state->update(outcome, fov_recompute, redraw_terrain);
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
                if (game_map->entities()[i]->ai != nullptr)
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
    GameMap ** game_map, Entity ** player, GameState ** game_state);

int main(int argc, char *argv[])
{

    // Set Custom font to use
    TCODConsole::setCustomFont(
            "data/fonts/16x16-sb-ascii.png",
            TCOD_FONT_LAYOUT_ASCII_INROW);

    // Init root console
    TCODConsole::initRoot(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        //WINDOW_TITLE, false, TCOD_RENDERER_SDL2);
        //WINDOW_TITLE, false, TCOD_RENDERER_GLSL);
        WINDOW_TITLE, false, TCOD_RENDERER_SDL);

    TCODSystem::setFps(30);

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

    // TODO enable message log again
    //message_log = None
    
    //TCODSystem::registerSDLRenderer(new SampleRenderer());

    DEBUG("Before!");
    SDL_Renderer * renderer = TCOD_sys_get_SDL_renderer();
    SDL_Window * sdl_window = TCOD_sys_get_SDL_window();

    bool bbb = renderer == NULL;
    bool aaa = sdl_window == NULL;
    DEBUG(bbb);
    DEBUG(aaa);
    DEBUG("After!");

    SDL_Rect sdl_rect;

    sdl_rect.x = 0;
    sdl_rect.y = 0;
    sdl_rect.w = 256;
    sdl_rect.h = 256;

    int rr = 0;

    TCOD_Console * messages_layer = TCOD_console_new(20, 20);
    //messages_layer->setCustomFont(
            //"data/fonts/Alloy-curses-12x12.png",
            //TCOD_FONT_LAYOUT_ASCII_INROW);

    //TCOD_sys_get_sdl_window();
    //TCOD_sys_accumulate_console(messages_layer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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
            sg.load("latest.json", &player, &game_map, &game_state);

            play_game(player, game_map, game_state);

            load_game = false;
            //break;
        }

        if (play_game_)
        {

            // Start a new game
            //player, game_map, message_log, game_phase = get_game_variables(
                //constants)
            //game_phase = GamePhase.PLAYERS_TURN
            
            init_new_game(
                 &game_map,  &player,  &game_state);

            //game_map.export_txt('maps_txt/lastmap.txt')

            //show_main_menu = False
            play_game(player, game_map, game_state);

            // When returning to main menu, reset play_game variable to false
            play_game_ = false;
        }

        
        rr++;
        DEBUG(rr);

        if (rr > 20)
        {

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
        SDL_RenderFillRect(renderer, &sdl_rect);

        SDL_RenderPresent(renderer);
        //DEBUG("Presenting!");
        }

        TCODConsole::root->flush();

        // Display the main title menu
        main_menu(&main_menu_background_image);
    }

    return 0;
}

/*

def main():

    args = parse_args()

    if args.seed is None:
        args.seed = random.randrange(sys.maxsize)

    # Initialize random number generator
    print("Seed was:", args.seed)
    random.seed(args.seed)

*/
