#include <algorithm>    // std::max
#include <vector>    // std::max
#include <sstream>

// SDL2 includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "libtcod.hpp"

#include "Constants.h"

#include "Entity.hpp"
#include "Player.hpp"
#include "skills/Skill.hpp"

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

/* number of frames in the last second */
// TODO must be coherent with the value set by TCODSystem::setFps();
static int fps=60;
/* current number of frames */
static int cur_fps=0;
/* length of the last rendering loop */
static float last_frame_length=0.0f;

/* minimum length for a frame (when fps are limited) */
static int min_frame_length=1000/fps;
//static int min_frame_length=0;

/**
 * Function to limit FPS
 */
void sync_time() {
    static uint32_t old_time = 0;
    static uint32_t new_time = 0;
    static uint32_t elapsed = 0;
    int32_t frame_time;
    int32_t time_to_wait;

    old_time = new_time;
    new_time = TCOD_sys_elapsed_milli();
    /* If TCOD has been terminated and restarted. */
    if (old_time > new_time)
    {
        old_time = elapsed = 0;
    }

    if ( new_time / 1000 != elapsed ) {
        /* update fps every second */
        fps = cur_fps;
        cur_fps = 0;
        elapsed = new_time / 1000;
    }

    /* if too fast, wait */
    frame_time = (new_time - old_time);
    last_frame_length = frame_time * 0.001f;
    cur_fps++;
    time_to_wait = min_frame_length - frame_time;

    if (old_time > 0 && time_to_wait > 0)
    {
        TCOD_sys_sleep_milli(time_to_wait);
        new_time = TCOD_sys_elapsed_milli();
        frame_time=(new_time - old_time);
    }

    last_frame_length = frame_time * 0.001f;
}


bool entity_in_sight(TCODMap * fov_map, Entity * entity)
{
    return fov_map->isInFov(entity->x, entity->y);
}

/**
 * Render the black empty frame for the popup message
 */
void render_popup_message_frame(int w, int h)
{
    int x = (SCREEN_WIDTH - w)/2;
    int y = (SCREEN_HEIGHT - h)/2;
    Consoles::singleton().popup_message->clear();
    Consoles::singleton().popup_message->printFrame(x, y, w, h, true, TCOD_BKGND_SET);
}

void render_message_log()
{
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

}

/*
// As TTF_RenderText_Solid could only be used on SDL_Surface then you have to
//  create the surface first.
SDL_Surface* text_surface = TTF_RenderText_Blended(font, visible_messages[i].text.c_str() , text_color); 

// Create texture
SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
SDL_SetTextureAlphaMod(text_texture, 255);

TTF_SizeText(font, visible_messages[i].text.c_str(), &w, &h);
*/

template <class Container>
void split2(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

void render_popup_message_text_sdl(SDL_Renderer * renderer, std::string text,
    int x, int y, int frame_w, int font_size=20, SDL_Color text_color={255, 255, 255, 255})
{

    int charw, charh;

    TCODSystem::getCharSize(&charw,&charh);

    TTF_Font * font = TTF_OpenFont("data/fonts/ttf/SairaExtraCondensed/SairaExtraCondensed-Regular.ttf", font_size);

    // Auxiliary variables to store size values
    int w, h;
    SDL_Rect Message_rect; //create a rect

    // Split the text in several lines
    std::vector<std::string> text_lines;
    split2(text, text_lines, '\n');

    int line_y = y * charh;

    for (int i=0; i<(int)text_lines.size(); i++)
    {
        // The text wrap value is computed keeping into account a padding of 1 on
        // each size.
        SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(
            font, text_lines[i].c_str(), text_color, (frame_w-4)*charw); 

        // Create texture
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_SetTextureAlphaMod(text_texture, 255);

        // Must retrieve size of rectangle from surface
        SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);

        Message_rect.x = x * charw;  //controls the rect's x coordinate
        Message_rect.y = line_y; // controls the rect's y coordinte
        Message_rect.w = w; // controls the width of the rect
        Message_rect.h = h; // controls the height of the rect

        SDL_RenderCopy(renderer, text_texture, NULL, &Message_rect);

        // Once copied, they can be destroyed
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);

        //line_y += h + 20;
        line_y += h - 5;
    }

    TTF_CloseFont(font);

}

/*
void render_skill_icons_sdl(SDL_Renderer * renderer)
{
    // TODO test
    SDL_Surface * icon_surface = IMG_Load("data/graphics/icons/skills/skill_stun.png");

    int charw, charh;

    TCODSystem::getCharSize(&charw,&charh);
    SDL_Rect skill_icon_rect; //create a rect
    
    int center_offset = 37;
    int SKILL_BOX_W = 128;

    for (int i=0; i<7; i++)
    {
        // Create texture
        SDL_Texture* icon_texture = SDL_CreateTextureFromSurface(renderer, icon_surface);
        // TODO change alpha based on existing popups/menus
        SDL_SetTextureAlphaMod(icon_texture, 255);

        skill_icon_rect.x = SKILLS_AREA_X * charw + (SKILL_BOX_W * i) + center_offset;  //controls the rect's x coordinate
        skill_icon_rect.y = SKILLS_AREA_Y * charh + 10; // controls the rect's y coordinte
        skill_icon_rect.w = 64; // controls the width of the rect
        skill_icon_rect.h = 64; // controls the height of the rect

        SDL_RenderCopy(renderer, icon_texture, NULL, &skill_icon_rect);
        SDL_DestroyTexture(icon_texture);
    }

    // Destroy surface
    SDL_FreeSurface(icon_surface);

}
*/

void render_skill_icons_sdl(SDL_Renderer * renderer)
{
    int charw, charh;

    TCODSystem::getCharSize(&charw,&charh);

    SDL_Rect skill_icon_rect; //create a rect
    
    int center_offset = 37;
    int SKILL_BOX_W = 128;

    for (unsigned int i=0; i<Player::singleton().skills.size(); i++)
    {
        Skill * skill = Player::singleton().skills[i];

        // TODO change alpha based on existing popups/menus
        SDL_SetTextureAlphaMod(skill->get_icon_texture(), 255);

        skill_icon_rect.x = (SKILLS_AREA_X + 1) * charw + (SKILL_BOX_W * i) + center_offset;  //controls the rect's x coordinate
        skill_icon_rect.y = (SKILLS_AREA_Y + 1) * charh; // controls the rect's y coordinte
        skill_icon_rect.w = 64; // controls the width of the rect
        skill_icon_rect.h = 64; // controls the height of the rect

        SDL_RenderCopy(renderer, skill->get_icon_texture(), NULL, &skill_icon_rect);
    }
}

/**
 * Render message log using a custom font
 */
void render_message_log_sdl(SDL_Renderer * renderer)
{

    int charw, charh;

    TCODSystem::getCharSize(&charw,&charh);

    TTF_Font* font = TTF_OpenFont("data/fonts/ttf/SairaExtraCondensed/SairaExtraCondensed-Regular.ttf", 20);

    // Auxiliary variables to store size values
    int w, h;
    SDL_Rect Message_rect; //create a rect

    // Initialize message height
    int y = 0;

    std::vector<Message> visible_messages =
        MessageLog::singleton().visible_messages();

    // TODO consider including timestamp for message (turn no)
    for (int i=0; i<(int)visible_messages.size(); i++)
    {

        // Set the color
        TCODColor c = visible_messages[i].color;    
        SDL_Color text_color = {c.r, c.g, c.b, 255};

        // As TTF_RenderText_Solid could only be used on SDL_Surface then you have to
        //  create the surface first.
        SDL_Surface* text_surface = TTF_RenderText_Blended(font, visible_messages[i].text.c_str() , text_color); 

        // Create texture
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_SetTextureAlphaMod(text_texture, 255);

        TTF_SizeText(font, visible_messages[i].text.c_str(), &w, &h);

        Message_rect.x = MESSAGE_LOG_X * charw ;  //controls the rect's x coordinate
        Message_rect.y = MESSAGE_LOG_Y * charh + (20 * y); // controls the rect's y coordinte
        Message_rect.w = w; // controls the width of the rect
        Message_rect.h = h; // controls the height of the rect

        SDL_RenderCopy(renderer, text_texture, NULL, &Message_rect);

        // Once copied, they can be destroyed
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);

        // Increment height of next message
        y++;
    }

    TTF_CloseFont(font);

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

    Entity * entity = nullptr;

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
            fov_map->isInFov(entities[i]->x, entities[i]->y) &&
            entities[i]->render_order() != NONE
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

    // Clear root console
    TCODConsole::root->clear();

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

        // Draw all entities in the list in the correct order
        // Entities are kept sorted in GameMap::add_entity
        for (int i=0; i < (int)game_map->entities().size(); i++)
        {

            // Skip entities whose render order is NONE
            if (game_map->entities()[i]->render_order() == NONE)
            {
                continue;
            }

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

    //render_message_log();

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

    // Do not render a label if the entity's render order is NONE
    if (entity_under_mouse != nullptr && entity_under_mouse->render_order() != NONE)
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
        inventory_item_submenu(player, game_state->selected_inventory_item);
    }

    // Container item submenu
    if (game_state->game_phase == CONTAINER_ITEM_MENU)
    {
        container_item_submenu(game_state->selected_inventory_item);
    }

    /////////////////////////////////////////
    ////////// Render death screen //////////
    /////////////////////////////////////////

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

    /////////////////////////////////////////
    ///////// Render terminal menu //////////
    /////////////////////////////////////////

    // Show terminal menu
    if (game_state->game_phase == TERMINAL_MENU)
    {
        terminal_menu(game_state->entity_interacted);
        
        // Blit inventory frame on root console
        TCODConsole::blit(
            Consoles::singleton().terminal,
            0, 0,
            TERMINAL_FRAME_WIDTH, TERMINAL_FRAME_HEIGHT,
            TCODConsole::root,
            (SCREEN_WIDTH - TERMINAL_FRAME_WIDTH)/2, 4,
            1.0f, 0.8f);
    }

    /////////////////////////////////////////
    ///////// Render container menu /////////
    /////////////////////////////////////////

    // Show menu for container
    if (game_state->game_phase == CONTAINER_MENU || game_state->game_phase == CONTAINER_ITEM_MENU)
    {
        container_menu(game_state->entity_interacted);
        
        // Blit inventory frame on root console
        TCODConsole::blit(
            Consoles::singleton().container_frame,
            0, 0,
            CONTAINER_FRAME_WIDTH, CONTAINER_FRAME_HEIGHT,
            TCODConsole::root,
            INVENTORY_FRAME_WIDTH + ITEM_SUBMENU_WIDTH, 0,
            1.0f, 1.0f);
    }

    /////////////////////////////////////////
    ///////// Render inventory menu /////////
    /////////////////////////////////////////

    // Show inventory menu
    if (game_state->game_phase == INVENTORY_MENU ||
        game_state->game_phase == INVENTORY_ITEM_MENU ||
        game_state->game_phase == CONTAINER_ITEM_MENU ||
        game_state->game_phase == CONTAINER_MENU)
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

    /////////////////////////////////////////
    //////// Render popup message  //////////
    /////////////////////////////////////////

    // First render the transparent frame with a console
    if (game_state->game_phase == POPUP_MESSAGE)
    {
        // TODO change width/position
        render_popup_message_frame(50, 30);

        // Blit with transparency
        TCODConsole::blit(
            Consoles::singleton().popup_message, 0, 0,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            TCODConsole::root,
            0, 0,
            1.0f, 0.8f);
    }

    /////////////////////////////////////////
    ///////////// SDL RENDERING /////////////
    /////////////////////////////////////////

    TCOD_sys_accumulate_console(TCODConsole::root->get_data());

    SDL_Renderer * renderer = TCOD_sys_get_sdl_renderer();

    render_message_log_sdl(renderer);
    render_skill_icons_sdl(renderer);

    // First render the transparent frame with a console
    if (game_state->game_phase == POPUP_MESSAGE)
    {

        std::string help_headline =  "Instructions";

        //std::string help_movement_l =  "WASD/arrows:         Movement";
        //std::string help_attack_l =    "F:                   Attack";
        //std::string help_inventory_l = "I:                   Show inventory";
        //std::string help_menu_l =      "ESC:                 Menu";
        //std::string help_inspect_l =   "LMB:                 Inspect";
        std::string help_movement_l =  "WASD/arrows:";
        std::string help_attack_l =    "F:";
        std::string help_inventory_l = "I:";
        std::string help_menu_l =      "ESC:";
        std::string help_inspect_l =   "LMB:";

        std::string help_movement_r =  "Movement";
        std::string help_attack_r =    "Attack";
        std::string help_inventory_r = "Show inventory";
        std::string help_menu_r =      "Menu";
        std::string help_inspect_r =   "Inspect";
        
        std::string help_text_l = "";
        //help_text_l += help_headline + "\n \n";
        help_text_l += help_movement_l + "\n";
        help_text_l += help_attack_l + "\n";
        help_text_l += help_inventory_l + "\n";
        help_text_l += help_inspect_l + "\n";
        help_text_l += help_menu_l + "\n";

        std::string help_text_r = "";
        //help_text_r += "\n";
        help_text_r += help_movement_r + "\n";
        help_text_r += help_attack_r + "\n";
        help_text_r += help_inventory_r + "\n";
        help_text_r += help_inspect_r + "\n";
        help_text_r += help_menu_r + "\n";

        int x = (SCREEN_WIDTH - 50)/2 + 2;
        int y = (SCREEN_HEIGHT - 30)/2 + 2;

        render_popup_message_text_sdl(renderer, help_headline, x, y, 50, 30, {255, 255, 102, 255});
        render_popup_message_text_sdl(renderer, help_text_l, x, y+4, 50);
        render_popup_message_text_sdl(renderer, help_text_r, x+36, y+4, 50);
    }

    SDL_RenderPresent(renderer);

    sync_time();

    //TCODConsole::flush();
}
