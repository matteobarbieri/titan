#include <string>
#include <sstream>

#include "../Constants.h"

#include "../libtcod.hpp"
//#include "libtcod/console/console.h"

#include <SDL2/SDL.h>

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


void render_all(TCODConsole * a)
{
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

}

int main(int argc, char *argv[])
{

    bool whichts = true;

    TCODSystem::setFps(30);

    TCODConsole::root->setDefaultBackground(TCODColor::black);

    TCODConsole * a = new TCODConsole(20, 20);

    // Set Custom font to use
    TCODConsole::setCustomFont(
            "data/fonts/16x16-sb-ascii.png",
            TCOD_FONT_LAYOUT_ASCII_INROW);

    auto ts1 = tcod::engine::get_tileset();

    TCODConsole::setCustomFont(
            "data/fonts/terminal12x12_gs_ro.png",
            TCOD_FONT_LAYOUT_ASCII_INROW);

    auto ts2 = tcod::engine::get_tileset();

    bool aa = (ts1 == ts2);

    DEBUG(aa << " (should be 0)");

    tcod::engine::set_tileset(ts1);

    //TCODConsole::get

    // Init root console
    TCODConsole::initRoot(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        WINDOW_TITLE, false, TCOD_RENDERER_SDL2);
        //WINDOW_TITLE, false, TCOD_RENDERER_GLSL);
        //WINDOW_TITLE, false, TCOD_RENDERER_SDL);

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

    while (!TCODConsole::root->isWindowClosed())
    {
        render_all(a);

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

        if ((rr + 1)%30 == 0)
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

        TCODConsole::flush();

    }

    return 0;
}
