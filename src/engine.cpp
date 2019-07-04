#include "Constants.h"

#include "menus.hpp"
#include "Entity.hpp"
#include "GameState.hpp"

#include "input.hpp"
#include "fov_functions.hpp"
#include "render_functions.hpp"

#include "libtcod.hpp"



using namespace std;

/*

def play_game(player, game_map, game_state,
              message_log,
              terrain_layer, panel, entity_frame, inventory_frame,
              main_window, constants):




*/

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

        /*
        if game_state.entity_targeted and redraw_terrain:
            game_state.entity_targeted = check_if_still_in_sight(
                fov_map, game_state.entity_targeted)
            # Check if by any chance target is dead
            # TODO more generally, if it is no longer targetable for any
            # reason
            if (
                game_state.entity_targeted and
                not game_state.entity_targeted.fighter):  # noqa

                game_state.entity_targeted = None

            # TODO same for focused entity?

        */

        // If the player move, check if targeted entity is still in sight
        if (game_state->entity_targeted != NULL && redraw_terrain)
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

        /*
        top_x, top_y = render_all(
            terrain_layer, panel, entity_frame, inventory_frame, main_window,
            player, game_map, fov_map, fov_recompute,
            redraw_terrain, redraw_entities, message_log,
            constants, mouse, game_state,
            current_turn)

        # TODO find a better place
        game_map.top_x = top_x
        game_map.top_y = top_y

        fov_recompute = False
        redraw_terrain = False
        redraw_entities = False

        libtcod.console_flush()
        */


        ////////////////////////////////////////////
        ////////////// PLAYER'S TURN ///////////////
        ////////////////////////////////////////////

        /*
        if game_state.is_players_turn():

            ////////////////////////////////////////////
            ///////////// EXECUTE ACTIONS //////////////
            ////////////////////////////////////////////
            action = handle_input(key, mouse, game_state.game_phase)

            # Add all objects required to perform any action
            # TODO check, should the message log be passed here?
            action.set_context(
                game_map, player, message_log, fov_map,
                game_state)

            # Execute it
            try:
                outcome = action.execute()
            except ShowMenuException:
                # Exit main game loop and return to main menu
                return True

            ////////////////////////////////////////////
            ///////////// RESOLVE OUTCOME //////////////
            ////////////////////////////////////////////
            if outcome is not None:

                fov_recompute, redraw_terrain = update_game_state(
                    outcome, game_state, fov_recompute, redraw_terrain,
                    message_log)

                    */

        /*

        ////////////////////////////////////////////
        ////////////// ENEMIES' TURN ///////////////
        ////////////////////////////////////////////
        elif game_state.is_enemies_turn():

            # Each entity takes a turn
            for entity in game_map.entities:
                if entity.ai:
                    # enemy_turn_results = entity.ai.take_turn(player, fov_map, game_map)

                    # Pick an action for each entity
                    entity_action = entity.ai.pick_action(player, game_map)

                    # XXX no need to set context, asit was needed previously to
                    # choose the action

                    # Execute the action
                    outcome = entity_action.execute()

            # Go back to player's turn state
            game_state.game_phase = GamePhase.PLAYERS_TURN
            # redraw_entities = True
            redraw_terrain = True

            current_turn += 1
    */
    }
}

/*
import argparse

import random

import sys

from input_handlers import handle_input, handle_main_menu
from loader_functions.initialize_new_game import get_constants, get_game_variables
from loader_functions.data_loaders import load_game, save_game
from menus import main_menu, message_box
from fov_functions import initialize_fov, recompute_fov

from render_functions import render_all, check_if_still_in_sight

from game_state import GamePhase, GameState
from death_functions import kill_monster, kill_player

from actions import ShowMenuException

def update_game_state(
    outcome,
    game_state: GameState, fov_recompute, redraw_terrain,
    message_log):  # noqa

    # Update game state
    if outcome.get('next_state') is not None:
        game_state.game_phase = outcome.get('next_state')

    # Update focused entity
    if outcome.get('entity_focused') is not None:
        game_state.entity_focused = outcome.get('entity_focused')

    # Update targeted entity
    if outcome.get('entity_targeted') is not None:
        game_state.entity_targeted = outcome.get('entity_targeted')

    # Update selected inventory item
    if outcome.get('selected_inventory_item') is not None:
        game_state.selected_inventory_item = outcome.get(
            'selected_inventory_item')

    # Determine whether to recompute fov...
    if outcome.get('fov_recompute') is not None:
        fov_recompute = outcome.get('fov_recompute')
    else:
        fov_recompute = fov_recompute

    # Or redraw terrain
    if outcome.get('redraw_terrain') is not None:
        redraw_terrain = outcome.get('redraw_terrain')
    else:
        redraw_terrain = redraw_terrain

    # Add messages to the log
    if outcome.get('messages') is not None:
        for m in outcome.get('messages'):
            message_log.add_message(m)

    return fov_recompute, redraw_terrain



*/


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
    bool play_game = false;

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
                play_game = true;
                break;
            case 'b':
                play_game = true;
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

        if (play_game)
        {

            //play_game(player, game_map,
                //game_state, message_log,
                //terrain_layer, panel, entity_frame, inventory_frame,
                //main_window, constants)
                
            // When returning to main menu, reset play_game variable to false
            play_game = false;
        }

        TCODConsole::root->flush();

        // Display the main title menu
        main_menu(&main_menu_background_image);
    }

    //player = None
    //game_map = None
    //message_log = None

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
                //# Start a new game
                //player, game_map, message_log, game_phase = get_game_variables(
                    //constants)
                //game_phase = GamePhase.PLAYERS_TURN

                //game_map.export_txt('maps_txt/lastmap.txt')

                //show_main_menu = False
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

    ######################
    ##### UNTIL HERE #####
    ######################



*/
