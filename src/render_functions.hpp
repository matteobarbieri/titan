#ifndef R20177_RENDER_FUNCTIONS
#define R20177_RENDER_FUNCTIONS

#include "libtcod.hpp"

#include <string>

// Consider replacing with forward declarations?

#include "Entity.hpp"
#include "map/GameMap.hpp"
#include "GameState.hpp"


bool entity_in_sight(TCODMap * fov_map, Entity * entity);

// TODO missing message_log parameter
void render_all(
    Entity * player,
    GameMap * game_map, TCODMap * fov_map,
    bool fov_recompute, bool redraw_terrain, 
    TCOD_mouse_t * mouse, GameState * game_state,
    int & top_x, int & top_y);

/*
 * Draw an entity on the terrain layer
*/
void draw_entity(TCODConsole * terrain_layer, Entity * entity,
                 TCODMap * fov_map, GameMap * game_map, int top_x=0, int top_y=0);

/**
 * Render a generic bar on a console
 */
void render_bar(TCODConsole * console, int x, int y, int total_width,
               std::string name, int value, int maximum,
               TCODColor bar_color, TCODColor back_color);

/**
 * Return the string of all entities at a given map position
 */
//std::vector<std::string> get_names_under_mouse(
std::string get_names_under_mouse(
    TCOD_mouse_t * mouse, std::vector<Entity *> entities, TCODMap * fov_map,
    int top_x, int top_y);

Entity * get_entity_under_mouse(
    TCOD_mouse_t * mouse, std::vector<Entity *> entities,
    TCODMap * fov_map, int top_x, int top_y);

void render_entity_label(
    TCODConsole * terrain_layer, Entity * entity,
    int top_x, int top_y);

/**
 * Print a frame containing information about the entity.
 */
void render_entity_frame(
    Entity * entity);

/**
 * Print a frame containing information about the entity.
 */
void render_death_screen(
    Entity * player);

#endif
