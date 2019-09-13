#include "libtcod.hpp"

#include <algorithm>    // std::max

#include "Constants.h"

#include "Entity.hpp"

#include "components/Fighter.hpp"
#include "components/Usable.hpp"

#include "map/GameMap.hpp"
#include "map/Tile.hpp"
#include "map/map_utils.hpp"

#include "GameState.hpp"
#include "GameMessages.hpp"

#include "Consoles.hpp"

#include "render_functions.hpp"

#include "ui/menus.hpp"

Entity * check_if_still_in_sight(TCODMap * fov_map, Entity * entity)
{

    if (fov_map->isInFov(entity->x, entity->y))
        return entity;
    else
        return 0;
}

// Print the name of the entity on the top left tile
void render_entity_label(
    TCODConsole * terrain_layer, Entity * entity,
    int top_x, int top_y)
{

    // Print a '\' symbol in the top left square above the entity
    terrain_layer->putChar(
        entity->x - top_x - 1,
        entity->y - top_y - 1,
        '\\');

    // Print the actual label
    // TODO change to non-deprecated function
    terrain_layer->printEx(
        entity->x - top_x - 1,
        entity->y - top_y - 2,
        //TCOD_BKGND_NONE,
        TCOD_BKGND_SET,
        TCOD_LEFT,
        "%s",
        entity->name.c_str());

}

void render_death_screen(Entity * player)
{

    // XXX Use submenu console

    // First, clear the console
    Consoles::singleton().submenu->clear();

    // Extract width and height
    int w = Consoles::singleton().submenu->getWidth();
    int h = Consoles::singleton().submenu->getHeight();

    int ds_x = (int)((w - DEATH_SCREEN_WIDTH))/2;
    int ds_y = (int)((h - DEATH_SCREEN_HEIGHT))/2;

    // Draw frame
    // Reset the color to white, just in case
    Consoles::singleton().submenu->setDefaultForeground(TCODColor::white);
    Consoles::singleton().submenu->setDefaultBackground(TCODColor::black);
    Consoles::singleton().submenu->printFrame(
        ds_x, ds_y,
        DEATH_SCREEN_WIDTH, DEATH_SCREEN_HEIGHT,
        true, TCOD_BKGND_DEFAULT, "You dead");

    // Print the entity's name
    //Consoles::singleton().submenu->setDefaultForeground(TCODColor::amber);
    //Consoles::singleton().submenu->printf(
        //3, 3, "%s", player->name.c_str());

    //Consoles::singleton().submenu->setDefaultBackground(player->color());
    //Consoles::singleton().submenu->rect(
        //3, 5, 10, 10, true, TCOD_BKGND_SET);
}


void render_entity_frame(Entity * entity)
{
    // Extract width and height
    int w = Consoles::singleton().entity_frame->getWidth();
    int h = Consoles::singleton().entity_frame->getHeight();

    // Draw frame
    // Reset the color to white, just in case
    Consoles::singleton().entity_frame->setDefaultForeground(TCODColor::white);
    Consoles::singleton().entity_frame->setDefaultBackground(TCODColor::black);
    Consoles::singleton().entity_frame->printFrame(
        1, 1, 
        w -2, h -2,
        true, TCOD_BKGND_DEFAULT, "Info");

    // Print the entity's name
    //Consoles::singleton().entity_frame->setDefaultForeground(TCODColor::amber);
    Consoles::singleton().entity_frame->printf(
        3, 3, "%s", entity->name.c_str());

    Consoles::singleton().entity_frame->setDefaultBackground(entity->color());
    Consoles::singleton().entity_frame->rect(
        3, 5, 10, 10, true, TCOD_BKGND_SET);
}

/*


*/

Entity * get_entity_under_mouse(
    TCOD_mouse_t * mouse, std::vector<Entity *> entities,
    TCODMap * fov_map, int top_x, int top_y)
{
    
    int x = mouse->cx;
    int y = mouse->cy;

    Entity * entity = 0;

    for (int i=0; i<(int)entities.size(); i++)
    {
        if (
            entities[i]->x == (top_x + x) && 
            entities[i]->y == (top_y + y) && 
            fov_map->isInFov(entities[i]->x, entities[i]->y)
            )
        {
            entity = entities[i];
        }
    }

    return entity;
}

//std::vector<std::string> get_names_under_mouse(
std::string get_names_under_mouse(
    TCOD_mouse_t * mouse, std::vector<Entity *> entities, TCODMap * fov_map,
    int top_x, int top_y)
{
    int x = mouse->cx;
    int y = mouse->cy;

    //std::vector<std::string> names;
    std::string names = "";

    for (int i=0; i<(int)entities.size(); i++)
    {
        if (
            entities[i]->x == (top_x + x) && 
            entities[i]->y == (top_y + y) && 
            fov_map->isInFov(entities[i]->x, entities[i]->y)
            )
        {
            names.append(entities[i]->name);
            names.append(", ");
        }
    }

    // Remove last ", "
    if (names.length() > 0)
    {
        names = names.substr(0, names.length() - 2);
        // TODO
        // Capitalize first name
    }

    return names;
}

void draw_entity(TCODConsole * terrain_layer, Entity * entity,
                 TCODMap * fov_map, GameMap * game_map, int top_x, int top_y)
{

    // Draw an entity if it is in player's FOV or is a non-moving entity
    // (e.g., stairs) on an explored tile.
    if (
            fov_map->isInFov(entity->x, entity->y) ||
            (entity->fixed() and
             game_map->tiles[
                compute_tile_index(entity->x, entity->y, game_map->width)
             ]->explored())
       )

    {

        // TODO evaluate whether to use Consoles::singleton() or pass the
        // console as a parameter
        
        // Draw the symbol
        //Consoles::singleton().terrain_layer->putChar(
            //entity->x - top_x, entity->y - top_y,
            //entity->symbol, TCOD_BKGND_NONE);
        terrain_layer->putChar(
            entity->x - top_x, entity->y - top_y,
            entity->symbol, TCOD_BKGND_NONE);

        // Color the symbol
        //Consoles::singleton().terrain_layer->setCharForeground(
            //entity->x - top_x, entity->y - top_y,
            //entity->color());
        terrain_layer->setCharForeground(
            entity->x - top_x, entity->y - top_y,
            entity->color());

    }


}

void render_bar(TCODConsole * console, int x, int y, int total_width,
               std::string name, int value, int maximum,
               TCODColor bar_color, TCODColor back_color)
{


    // Compute bar width, based on current value and maximum
    //bar_width = int(float(value) / maximum * total_width)
    int bar_width = (int) ((float)value / maximum * total_width);

    // Draw a rectangle of the background color for the full
    // length of the bar
    console->setDefaultBackground(back_color);
    console->rect(x, y, total_width, 1, false, TCOD_BKGND_SCREEN);

    // Now draw the 'active' part of the bar
    console->setDefaultBackground(bar_color);
    if (bar_width > 0)
    {
        console->rect(x, y, bar_width, 1, false, TCOD_BKGND_SCREEN);
    }

    // Draw the bar name
    console->setDefaultForeground(TCODColor::white);
    console->printEx(
        x + total_width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER,
        "%s: %d/%d", name.c_str(), value, maximum );

}

/*


*/

void render_all(
    Entity * player,
    GameMap * game_map, TCODMap * fov_map,
    bool fov_recompute, bool redraw_terrain, 
    TCOD_mouse_t * mouse, GameState * game_state,
    int & top_x, int & top_y)
{

    /////////////////////////////////////////
    ///////// Render terrain first //////////
    /////////////////////////////////////////

    // First compute the part of visible map, based on the player's position
    // Compute top left corner coordinates
    
    top_x = player->x - SCREEN_WIDTH/2;
    top_x = std::max(0, top_x);
    top_x = std::min(game_map->width - SCREEN_WIDTH, top_x);

    top_y = player->y - SCREEN_HEIGHT/2;
    top_y = std::max(0, top_y);
    top_y = std::min(game_map->height - SCREEN_HEIGHT + PANEL_HEIGHT, top_y);

    int target_x = -100;
    int target_y = -100;

    int range_flag = 1;
    AOEUsable * a;

    // Force terrain redrawing on targeting
    if (game_state->game_phase == TARGETING)
    {
        redraw_terrain = true;

        target_x = mouse->cx;
        target_y = mouse->cy;

        a = (AOEUsable *)game_state->selected_inventory_item->usable;
        //Targetable * a = (Targetable *)game_state->selected_inventory_item->usable;
        
        if (game_map->aux_fov_map_100->isInFov(target_x+top_x, target_y+top_y))
        {
            if (a->is_in_range(target_x, target_y, player->x-top_x, player->y-top_y))
            {
                range_flag = 1;
            }
            else
            {
                range_flag = 3;
            }
        }
        else
        {
            range_flag = 3;
        }
    }

    // Only redraw terrain if needed
    if (redraw_terrain)
    {

        // Clear the console before drawing on it
        Consoles::singleton().terrain_layer->clear();

        for (int y=top_y; y<top_y + SCREEN_HEIGHT - PANEL_HEIGHT; y++)
        {
            for (int x=top_x; x< top_x + SCREEN_WIDTH; x++)
            {

                bool visible = fov_map->isInFov(x, y);

                // Compute array index of current tile
                int tile_index = compute_tile_index(x, y, game_map->width);


                // Determine if tile must be highlighted in a special way
                // because targeted
                // TODO improve
                int target_flag = 0;

                //if (x-top_x == target_x and y-top_y == target_y)
                if (game_state->game_phase == TARGETING)
                {

                    if (a->is_in_radius(target_x, target_y, x-top_x, y-top_y))
                    {
                        target_flag = range_flag;
                    }

                    //if (x-top_x == target_x and y-top_y == target_y)
                    //{
                        //target_flag = 1;
                    //}
                }

                if (visible)
                {

                    (game_map->tiles[tile_index])->render_at(
                        Consoles::singleton().terrain_layer,
                        x-top_x, y-top_y, visible, target_flag);

                    (game_map->tiles[tile_index])->explored(true);
                }

                else if ((game_map->tiles[tile_index])->explored())
                {
                    // Render as currently out of sight
                    (game_map->tiles[tile_index])->render_at(
                        Consoles::singleton().terrain_layer,
                        x-top_x, y-top_y, visible, target_flag);
                }
            }
        }

        //std::cout << "render_all: Checkpoint 3" << std::endl;

        if (game_state->entity_targeted != 0)
        {

            // Check if target is [still] visible
            if (fov_map->isInFov(
                game_state->entity_targeted->x,
                game_state->entity_targeted->y))
            {

                // TODO find another way to highlight targeted entity
                // Set background red
                Consoles::singleton().terrain_layer->setCharBackground(
                    game_state->entity_targeted->x - top_x,
                    game_state->entity_targeted->y - top_y,
                    TCODColor::red, TCOD_BKGND_SET);
            }


        }


    }


    /////////////////////////////////////////
    /////////// Render entities  ////////////
    /////////////////////////////////////////

    if (redraw_terrain)
    {

        //std::cout << "render_all: Checkpoint 4" << std::endl;

        // Draw all entities in the list in the correct order
        // Entities are kept sorted in GameMap::add_entity
        for (int i=0; i < (int)game_map->entities().size(); i++)
        {
            //std::cout << i << " " << game_map->entities()[i]->name << std::endl;
            draw_entity(
                Consoles::singleton().terrain_layer, game_map->entities()[i],
                fov_map, game_map, top_x, top_y);
        }

        // Blit terrain layer on root console
        TCODConsole::blit(
            Consoles::singleton().terrain_layer,
            0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
            TCODConsole::root,
            0, 0);

    }

    /////////////////////////////////////////
    //////////// Render panel  //////////////
    /////////////////////////////////////////

    // Now render the health bar
    Consoles::singleton().panel->setDefaultBackground(TCODColor::black);
    Consoles::singleton().panel->clear();
    //Consoles::singleton().panel->rect(
        //BAR_WIDTH + 2, 0,
        //PANEL_WIDTH-BAR_WIDTH-2, PANEL_HEIGHT,
        //TCOD_BKGND_NONE);

    // Retrieve the list of visible messages
    //std::vector<Message> visible_messages =
        //game_state->message_log->visible_messages();
    std::vector<Message> visible_messages =
        MessageLog::singleton().visible_messages();

    // Initialize message height
    int y = 1;

    // TODO consider including timestamp for message (turn no)
    for (int i=0; i<(int)visible_messages.size(); i++)
    {
        // Set text color
        Consoles::singleton().panel->setDefaultForeground(
            visible_messages[i].color);

        // TODO replace deprecated function
        Consoles::singleton().panel->printEx(
            BAR_WIDTH + 2, y, TCOD_BKGND_SET, TCOD_LEFT,
            "%s", visible_messages[i].text.c_str());

        // Increment height of next message
        y++;
    }

    render_bar(
        Consoles::singleton().panel, 1, 1, BAR_WIDTH,
        "HP", player->fighter->hp() , player->fighter->max_hp(),
        TCODColor::lightRed, TCODColor::darkerRed);

    // Show current dungeon level
    // TODO change with non deprecated function?
    Consoles::singleton().panel->printEx(
        1, 3, TCOD_BKGND_NONE, TCOD_LEFT, "Dungeon level: %d",
        game_map->dungeon_level);

    // Show current turn
    // TODO change with non deprecated function?
    Consoles::singleton().panel->printEx(
        1, 5, TCOD_BKGND_NONE, TCOD_LEFT, "Time: %d",
        game_state->current_turn);

    Consoles::singleton().panel->setDefaultForeground(TCODColor::lightGrey);
    Consoles::singleton().panel->printEx(
        1, 0, TCOD_BKGND_NONE, TCOD_LEFT,
        "%s",
        get_names_under_mouse(
            mouse, game_map->entities(), fov_map,
            top_x, top_y).c_str());

    /////////////////////////////////////////
    /////// Blit panel on root console //////
    /////////////////////////////////////////

    TCODConsole::blit(
        Consoles::singleton().panel, 0, 0,
        SCREEN_WIDTH, PANEL_HEIGHT,
        TCODConsole::root, 0, PANEL_Y);
    
    /////////////////////////////////////////
    /// Blit terrain layer on root console //
    /////////////////////////////////////////

    TCODConsole::blit(
        Consoles::singleton().terrain_layer, 0, 0,
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT,
        Consoles::singleton().main_window,
        0, 0);

    /////////////////////////////////////////
    ///////// Render entity label ///////////
    /////////////////////////////////////////

    Entity * entity_under_mouse = get_entity_under_mouse(
            mouse, game_map->entities(), fov_map, top_x, top_y);

    if (entity_under_mouse)
    {
        render_entity_label(
            Consoles::singleton().main_window, entity_under_mouse,
            top_x, top_y);

    }

    /////////////////////////////////////////
    ///////// Render entity frame  //////////
    /////////////////////////////////////////

    // Render the focused entity
    if (game_state->game_phase == ENTITY_INFO)
    {
        render_entity_frame(
            game_state->entity_focused);
    }

    // Render the selected inventory item
    if (game_state->game_phase == INVENTORY_ITEM_MENU)
    {
        render_entity_frame(
            game_state->selected_inventory_item);
    }

    // Blit the frame on the console below (main window)
    if (game_state->game_phase == ENTITY_INFO ||
        game_state->game_phase == INVENTORY_ITEM_MENU)
    {
        TCODConsole::blit(
            Consoles::singleton().entity_frame, 0, 0,
            FRAME_WIDTH, FRAME_HEIGHT,
            Consoles::singleton().main_window,
            SCREEN_WIDTH - FRAME_WIDTH, 0);
    }

    // Inventory item submenu
    if (game_state->game_phase == INVENTORY_ITEM_MENU)
    {
        item_submenu(player, game_state->selected_inventory_item);
    }

    // Inventory item submenu
    if (game_state->game_phase == PLAYER_DEAD)
    {
        render_death_screen(player);
        TCODConsole::blit(
            Consoles::singleton().submenu, 0, 0,
            TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT,
            Consoles::singleton().main_window,
            0, 0,
            1.0f, 0.8f);
    }

    // Finally blit main window console on root console
    TCODConsole::blit(
        Consoles::singleton().main_window,
        0, 0,
        TERRAIN_LAYER_WIDTH, TERRAIN_LAYER_HEIGHT,
        TCODConsole::root,
        0, 0);

    // Show inventory menu
    if (game_state->game_phase == INVENTORY_MENU ||
        game_state->game_phase == INVENTORY_ITEM_MENU)
    {
        inventory_menu(player);
        
        // Blit inventory frame on root console
        TCODConsole::blit(
            Consoles::singleton().inventory_frame,
            0, 0,
            FRAME_WIDTH, FRAME_HEIGHT,
            TCODConsole::root,
            0, 0);

    }

/*


    # Show character screen
    elif game_phase == GamePhase.CHARACTER_SCREEN:
        character_screen(player, 30, 10, screen_width, screen_height)

    return top_x, top_y
*/

    TCODConsole::root->flush();

}


/*

from game_state import GamePhase

from menus import (
    character_screen, inventory_menu, item_submenu)

*/


