#include <string>
#include <sstream>

#include "../Constants.h"

#include "../libtcod.hpp"
#include "../libtcod.h"
//#include "libtcod/console/console.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

//#include "../sdl/custom_renderer.hpp"
//#include "../sdl/timer.hpp"
//#include "../sdl/text.hpp"

const int SCREEN_FPS = 100;
//const int SCREEN_FPS = 5;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

using namespace std;

char handle_main_menu(TCOD_key_t key)
{
    switch(key.c)
    {
        case 'a':
        case 'b':
        case 'c':
            return key.c;
            break;
        default:
            return '\0';
            break;
    }
}


void render_sdl()
{

    SDL_Renderer * renderer = TCOD_sys_get_sdl_renderer();
    //SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 10);
    SDL_Rect sdl_rect = {0, 0, 100, 100};
    SDL_RenderFillRect(renderer, &sdl_rect);

}

void render_all(TCODConsole * a, TCODConsole * b, TCOD_Console * c, shared_ptr<tcod::tileset::Tileset> ts1, shared_ptr<tcod::tileset::Tileset> ts2)
{
    SDL_Renderer * renderer = TCOD_sys_get_sdl_renderer();


    TCODConsole::root->clear();
    a->clear();

    //a->setDefaultBackground(TCODColor::black);
    //a->setDefaultForeground(TCODColor::green);
    a->setDefaultBackground(TCODColor::darkerBlue);
    a->rect(0, 0, 10, 10, true);
    a->printf(11, 11, "%s", "Test1");
    a->printf(11, 12, "%s", "Test2");

    TCODConsole::blit(
        a,
        0, 0, 20, 20,
        TCODConsole::root,
        50, 10);

    //tcod::engine::set_tileset(ts1);

    b->setKeyColor(TCODColor::darkerRed);
    b->setDefaultBackground(TCODColor::black);
    b->clear();
    b->setDefaultBackground(TCODColor::darkerRed);
    b->rect(0, 0, 10, 10, true, TCOD_BKGND_SET);
    b->setDefaultBackground(TCODColor::darkerGreen);
    b->rect(0, 10, 10, 20, true, TCOD_BKGND_SET);
    b->printf(0, 0, "%s", "01234567890123456789");
    b->printf(11, 11, "%s", "Test1");
    b->printf(11, 12, "%s", "Test2");

    TCODConsole::blit(
        b,
        0, 0, 20, 20,
        TCODConsole::root,
        50, 30);

    int charw,charh;
    TCODSystem::getCharSize(&charw,&charh);

    //DEBUG(charw << " " << charh);

    // compute size in pixels
    int W_p = SCREEN_WIDTH * charw;
    int H_p = SCREEN_HEIGHT * charh;

    //SDL_Rect r1 = {0, 0, W_p/2, H_p/2};
    //SDL_Rect r2 = {W_p/2, 0, W_p/2, H_p/2};

    TCOD_sys_accumulate_console(TCODConsole::root->get_data());
    //TCOD_sys_accumulate_console_(TCODConsole::root->get_data(), &r1);
    //TCOD_sys_accumulate_console_(TCODConsole::root->get_data(), &r2);
    //SDL_RenderPresent(renderer);

    // C
    //tcod::engine::set_tileset(ts2);
    
    /*
    TCOD_console_set_default_background(c, TCOD_black);
    TCOD_console_clear(c);
    TCOD_console_set_default_background(c ,TCOD_darker_yellow);
    TCOD_console_rect(c, 0, 0, 40, 20, true, TCOD_BKGND_SET);


    TCOD_console_set_alignment(c, TCOD_LEFT);
    TCOD_console_set_background_flag(c, TCOD_BKGND_SET);
    TCOD_console_printf(c, 1, 1, "%d, %d %s", 6, 7, "wewea");
    */

    //TCOD_console_blit(c, 0,0, 20, 20, NULL ,5,5, 1.0, 1.0);

    // Test accumulate console
    //TCOD_sys_accumulate_console(c);

    //tcod::engine::set_tileset(ts1);

}

int main(int argc, char *argv[])
{

    bool whichts = true;
    
    // Init root console
    TCODConsole::initRoot(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        WINDOW_TITLE, false, TCOD_RENDERER_SDL2);
        //WINDOW_TITLE, false, TCOD_RENDERER_GLSL);
        //WINDOW_TITLE, false, TCOD_RENDERER_SDL);

    TCODConsole::root->setDefaultBackground(TCODColor::black);
    TCODConsole::flush();

    TCODConsole * a = new TCODConsole(20, 20);
    TCODConsole * b = new TCODConsole(20, 20);

    //TCOD_Console * c = TCOD_console_new(20,20);
    TCOD_Console * c = TCOD_console_new(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set Custom font to use
    TCODConsole::setCustomFont(
            "data/fonts/16x16-sb-ascii.png",
            TCOD_FONT_LAYOUT_ASCII_INROW);

    //auto ts1 = tcod::engine::get_tileset();
    shared_ptr<tcod::tileset::Tileset> ts1= tcod::engine::get_tileset();

    //TCODConsole::setCustomFont(
            //"data/fonts/terminal12x12_gs_ro.png",
            //TCOD_FONT_LAYOUT_ASCII_INROW);
    //TCODConsole::setCustomFont(
            //"data/fonts/16x16-sb-ascii.png",
            //TCOD_FONT_LAYOUT_ASCII_INROW);


    //auto ts2 = tcod::engine::get_tileset();
    shared_ptr<tcod::tileset::Tileset> ts2= tcod::engine::get_tileset();

    bool aa = (ts1 == ts2);

    DEBUG(aa << " (should be 0)");

    //tcod::engine::set_tileset(ts1);

    TCODSystem::setFps(30);

    // Load the background image
    //TCODImage main_menu_background_image("menu_background2.png");

    TCOD_key_t key;
    TCOD_mouse_t mouse;

    // TODO is this one really needed?
    TCOD_event_t ev;

    bool exit_game = false;
    bool play_game_ = false;
    bool load_game = false;

    //In memory text stream
    stringstream timeText;

    int rr = 0;

    SDL_Renderer * renderer = TCOD_sys_get_sdl_renderer();
    bool renderer_check = (renderer == NULL);
    DEBUG("Renderer check: " << renderer_check << " (should be 0)");

    aa = tcod::engine::get_display() == NULL;
    DEBUG("tcod::engine::get_display(): " << renderer_check << "(1 -> equals NULL)");

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

        if (play_game_)
        {

            // Start a new game
            
            // When returning to main menu, reset play_game variable to false
            play_game_ = false;
        }

        
        rr++;
        DEBUG(rr);

        if (false && (rr + 1)%30 == 0)
        {

            if (whichts)
            {
                tcod::engine::set_tileset(ts2);
            }
            else
            {
                tcod::engine::set_tileset(ts1);
            }
            whichts = !whichts;
        }


        render_all(a, b, c, ts1, ts2);
        render_sdl();
        
        // TODO limit FPS manually
        SDL_RenderPresent(renderer);

    }

    return 0;
}
