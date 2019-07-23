#include "Constants.h"

#include "ui/menus.hpp"
#include "Entity.hpp"
#include "GameState.hpp"
#include "GamePhase.hpp"

#include "Consoles.hpp"
#include "actions/Action.hpp"

#include "components/Ai.hpp"

#include "input.hpp"
#include "fov_functions.hpp"
#include "render_functions.hpp"

#include "libtcod.hpp"


using namespace std;


void play_game(Entity * player, GameMap * game_map, GameState * game_state)
{
    
    // At the beginning of the game, recompute fov
    bool fov_recompute = true;
    bool redraw_terrain = true;
    bool redraw_entities = true;

    // Initialize fov map
    TCODMap * fov_map = initialize_fov(game_map);

    // TODO needs initialization?
    TCOD_key_t key;
    TCOD_mouse_t mouse;

    // TODO is this one really needed?
    TCOD_event_t ev;
    
    // TODO move this
    // The current turn
    int current_turn = 1;

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
            fov_map->computeFov(
                player->x, player->y, FOV_RADIUS,
                FOV_LIGHT_WALLS, FOV_ALGORITHM);

        }

        // If the player move, check if targeted entity is still in sight
        if (game_state->entity_targeted != 0 && redraw_terrain)
        {

            game_state->entity_targeted = check_if_still_in_sight(
                fov_map, game_state->entity_targeted);

            // Check if by any chance target is dead
            // TODO more generally, if it is no longer targetable for any
            // reason
            
            if (game_state->entity_targeted != NULL &&
                game_state->entity_targeted->fighter == NULL)
            {
                game_state->entity_targeted = NULL;
            }

            // TODO same for focused entity?

        }

        render_all(
            player, game_map, fov_map,
            fov_recompute, redraw_terrain,
            &mouse, game_state, current_turn,
            top_x, top_y);

        std::cout << "Checkpoint 7!" << std::endl;

        // TODO find a better place
        game_map->top_x = top_x;
        game_map->top_y = top_y;

        fov_recompute = false;
        redraw_terrain = false;
        redraw_entities = false;

        TCODConsole::root->flush();

        std::cout << "Checkpoint 7!" << std::endl;

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
            if (action != 0)
            {

                std::cout << "Checkpoint 8!" << std::endl;

                // Add all objects required to perform any action

                action->set_context(
                    game_map, player, fov_map, game_state);

                // TODO explicitly destroy action?
                // TODO explicitly destroy outcome?

                // Execute the action
                try {
                    outcome = action->execute();
                } catch(ExitGameException e) {
                    // Exit to main menu
                    return;
                }

                ////////////////////////////////////////////
                ///////////// RESOLVE OUTCOME //////////////
                ////////////////////////////////////////////
                
                if (outcome != 0)
                {
                    // TODO also need message log parameter?
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
                if (game_map->entities()[i]->ai != 0)
                {

                    // Pick an action for each entity
                    entity_action = game_map->entities()[i]->ai->pick_action(
                        player, game_map);

                    // XXX no need to set context, asit was needed previously to
                    // choose the action

                    // Execute the action
                    outcome = entity_action->execute();

                    // TODO
                    // Should we use the same method?
                    if (outcome != 0)
                    {
                        // TODO also need message log parameter?
                        game_state->update(outcome, fov_recompute, redraw_terrain);
                    }

                    // TODO destroy entity_action?
                    // TODO destroy outcome?

                }
            }

            // Go back to player's turn state
            game_state->game_phase = PLAYERS_TURN;
            redraw_entities = true;

            current_turn++;

        } // if (game_state->is_enemies_turn())

    }
}

/*

from loader_functions.initialize_new_game import get_constants, get_game_variables
from loader_functions.data_loaders import load_game, save_game
from menus import main_menu, message_box
from fov_functions import initialize_fov, recompute_fov

from game_state import GamePhase, GameState
from death_functions import kill_monster, kill_player

from actions import ShowMenuException

*/

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
        WINDOW_TITLE, false, TCOD_RENDERER_SDL);

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
            break;
        }

        if (play_game_)
        {

            // Start a new game
            //player, game_map, message_log, game_phase = get_game_variables(
                //constants)
            //game_phase = GamePhase.PLAYERS_TURN
            //
            
            init_new_game(
                 &game_map,  &player,  &game_state);

            //game_map.export_txt('maps_txt/lastmap.txt')

            //show_main_menu = False
            play_game(player, game_map, game_state);

                
            // When returning to main menu, reset play_game variable to false
            //play_game = false;
        }

        TCODConsole::root->flush();

        // Display the main title menu
        main_menu(&main_menu_background_image);
    }


    //show_main_menu = True
    //show_load_error_message = False


    //key = libtcod.Key()
    //mouse = libtcod.Mouse()

    //while not libtcod.console_is_window_closed():

        //if show_main_menu:

            //if show_load_error_message:
                //message_box(
                    //terrain_layer, 'No save game to load', 50,
                    //constants['screen_width'], constants['screen_height'])

            //libtcod.console_flush()

            //action = handle_main_menu(key)

            //new_game = action.get('new_game')
            //load_saved_game = action.get('load_game')
            //exit_game = action.get('exit')

            //if (show_load_error_message and
                //(new_game or load_saved_game or exit_game)):
                //# TODO wut?
                //show_load_error_message = False
            //elif new_game:
            // XXX removed code
            //elif load_saved_game:
                //# Load a previously saved game
                //try:
                    //player, game_map, message_log, game_phase = load_game()
                    //show_main_menu = False
                //except FileNotFoundError:
                    //show_load_error_message = True
            //elif exit_game:
                //break

        //else:
            //# migrating to tcod
            //# libtcod.console_clear(terrain_layer)
            //terrain_layer.clear()

            //game_state = GameState()
            //# game_state.player = player
            //game_state.game_phase = game_phase
            //# game_state.game_map = game_map

            //play_game(player, game_map,
                //game_state, message_log,
                //terrain_layer, panel, entity_frame, inventory_frame,
                //main_window, constants)

            //show_main_menu = True

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
