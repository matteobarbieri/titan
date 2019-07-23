#ifndef R20177_INPUT
#define R20177_INPUT

#include "libtcod.hpp"

// Forward declarations
class Action;
class GameState;

char handle_main_menu(TCOD_key_t);

Action * handle_input(
    TCOD_key_t key, TCOD_mouse_t mouse, GameState * game_state);

Action * handle_entity_info(TCOD_key_t key, TCOD_mouse_t mouse);

Action * handle_inventory_item_keys(TCOD_key_t key, TCOD_mouse_t mouse);

Action * handle_inventory_keys(TCOD_key_t key, TCOD_mouse_t mouse);

Action * handle_character_screen(TCOD_key_t key, TCOD_mouse_t mouse);

Action * handle_player_turn_keys(TCOD_key_t key, TCOD_mouse_t mouse);

Action * handle_targeting_keys(TCOD_key_t key);

/*
 *The set of keys for a dead player.
 *
 *Can only see the inventory and toggle fullscreen.
 */
Action * handle_player_dead_keys(TCOD_key_t key);

Action * handle_mouse(TCOD_mouse_t mouse);

#endif
