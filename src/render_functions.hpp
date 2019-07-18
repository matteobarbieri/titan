#ifndef R20177_RENDER_FUNCTIONS
#define R20177_RENDER_FUNCTIONS

// Consider replacing with forward declarations?

#include "Entity.hpp"
#include "map/GameMap.hpp"
#include "GameState.hpp"

#include "libtcod.hpp"

Entity * check_if_still_in_sight(TCODMap * fov_map, Entity * entity);

// TODO missing message_log parameter
void render_all(
    Entity * player,
    GameMap * game_map, TCODMap * fov_map,
    bool fov_recompute, bool redraw_terrain, 
    TCOD_mouse_t * mouse, GameState * game_state,
    int current_turn, int & top_x, int & top_y);

/*
 * Draw an entity on the terrain layer
*/
void draw_entity(TCODConsole * terrain_layer, Entity * entity,
                 TCODMap * fov_map, GameMap * game_map, int top_x=0, int top_y=0);


/*
# import libtcodpy as libtcod
import tcod as libtcod

from enum import Enum, auto

from game_state import GamePhase

from menus import (
    character_screen, inventory_menu, item_submenu)


class RenderOrder(Enum):
    STAIRS = auto()
    CORPSE = auto()
    ITEM = auto()
    ACTOR = auto()


def get_entity_under_mouse(mouse, entities, fov_map, top_x, top_y):
    (x, y) = (mouse.cx, mouse.cy)

    entities_list = [
        entity for entity in entities if
            entity.x == (top_x + x) and  # noqa
            entity.y == (top_y + y) and  # noqa
            libtcod.map_is_in_fov(fov_map, entity.x, entity.y)]  # noqa

    if entities_list:
        sorted(entities_list, key=lambda e: e.render_order.value)
        return entities_list[-1]  # The last one
    else:
        return None


def get_names_under_mouse(mouse, entities, fov_map, top_x, top_y):
    (x, y) = (mouse.cx, mouse.cy)

    names = [
        entity.name for entity in entities if
            entity.x == (top_x + x) and  # noqa
            entity.y == (top_y + y) and  # noqa
            libtcod.map_is_in_fov(fov_map, entity.x, entity.y)]  # noqa
    names = ', '.join(names)

    return names.capitalize()


def render_entity_label(terrain_layer, entity, top_x, top_y):

    # Print the name of the entity on the top left tile
    libtcod.console_put_char(
        terrain_layer,
        entity.x-top_x-1, entity.y-top_y-1,
        '\\', libtcod.BKGND_DEFAULT)

    libtcod.console_print_ex(
        terrain_layer,
        # 0,
        # top_x - entity.x - 1, top_y - entity.y - 1,
        entity.x - top_x - 1,  entity.y - top_y - 2,
        libtcod.BKGND_NONE,
        libtcod.LEFT,
        '{}'.format(entity.name))


def render_entity_frame(entity_frame, entity):

    # Draw a rectangle of the background color for the full
    # length of the bar
    # libtcod.console_set_default_background(entity_frame, libtcod.red)
    # libtcod.console_rect(entity_frame, 3, 3, 7, 2,
                         # False, libtcod.BKGND_SCREEN)

    # Extract width and height
    w = entity_frame.width
    h = entity_frame.height

    # Draw frame
    entity_frame.draw_frame(
        1, 1,
        w-2, h-2,
        'Info')

    # Print the entiy's name
    entity_frame.print(
        3, 3, '{}'.format(entity.name))

    # Draw entity graphics
    # TODO
    # Mockup for entity detail
    # entity_frame.draw_rect(
        # 3, 5, 10, 10, 0, bg=libtcod.red)
    entity_frame.draw_rect(
        3, 5, 10, 10, 0, bg=entity.color)


def render_bar(panel, x, y, total_width,
               name, value, maximum,
               bar_color, back_color):

    # Compute bar width, based on current value and maximum
    bar_width = int(float(value) / maximum * total_width)

    # Draw a rectangle of the background color for the full
    # length of the bar
    libtcod.console_set_default_background(panel, back_color)
    libtcod.console_rect(panel, x, y, total_width, 1,
                         False, libtcod.BKGND_SCREEN)

    # Now draw the 'active' part of the bar
    libtcod.console_set_default_background(panel, bar_color)
    if bar_width > 0:
        libtcod.console_rect(panel, x, y, bar_width, 1,
                             False, libtcod.BKGND_SCREEN)

    # Draw the event log
    libtcod.console_set_default_foreground(panel, libtcod.white)
    libtcod.console_print_ex(
        panel, int(x + total_width / 2), y,
        libtcod.BKGND_NONE,
        libtcod.CENTER,
        '{0}: {1}/{2}'.format(name, value, maximum))





*/

#endif
