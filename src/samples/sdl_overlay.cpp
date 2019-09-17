#include <string>
#include <sstream>

#include "../Constants.h"

#include "../libtcod.hpp"
//#include "libtcod/console/console.h"

#include <SDL2/SDL.h>

#include "../sdl/custom_renderer.hpp"
#include "../sdl/timer.hpp"
#include "../sdl/text.hpp"

const int SCREEN_FPS = 100;
//const int SCREEN_FPS = 5;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//#include "sdl/custom_renderer.cpp"
bool sdl_callback_enabled = false;

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

    a->setDefaultBackground(TCODColor::black);
    a->clear();

    a->setDefaultBackground(TCODColor::yellow);
    a->rect(0, 0, 10, 10, true, TCOD_BKGND_SET);

    TCODConsole::blit(
        a,
        0, 0, 20, 20,
        TCODConsole::root,
        50, 10);

}

int main(int argc, char *argv[])
{

    // Init root console
    TCODConsole::initRoot(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        //WINDOW_TITLE, false, TCOD_RENDERER_SDL2);
        //WINDOW_TITLE, false, TCOD_RENDERER_GLSL);
        WINDOW_TITLE, false, TCOD_RENDERER_SDL);

    TCODSystem::setFps(30);

    TCODConsole::root->setDefaultBackground(TCODColor::black);

    TCODConsole * a = new TCODConsole(20, 20);

    // Set Custom font to use
    TCODConsole::setCustomFont(
            "data/fonts/16x16-sb-ascii.png",
            TCOD_FONT_LAYOUT_ASCII_INROW);

    // Load the background image
    //TCODImage main_menu_background_image("menu_background2.png");

    TCOD_key_t key;
    TCOD_mouse_t mouse;

    // TODO is this one really needed?
    TCOD_event_t ev;

    bool exit_game = false;
    bool play_game_ = false;
    bool load_game = false;

    //TCODSystem::registerSDLRenderer(new SampleRenderer());

    DEBUG("Before!");
    SDL_Renderer * renderer = TCOD_sys_get_SDL_renderer();
    SDL_Window * sdl_window = TCOD_sys_get_SDL_window();

    bool ccc = (SDL_GetRenderer(sdl_window) == renderer);

    bool bbb = renderer == NULL;
    bool aaa = sdl_window == NULL;
    DEBUG(aaa << " (should be 0)");
    DEBUG(bbb << " (should be 0)");
    DEBUG(ccc << " (should be 1)");
    DEBUG("After!");

    //return 1;

    SDL_Rect sdl_rect;

    sdl_rect.x = 0;
    sdl_rect.y = 0;
    sdl_rect.w = 256;
    sdl_rect.h = 256;

    //The frames per second timer
    LTimer fpsTimer;

    //The frames per second cap timer
    LTimer capTimer;

    //In memory text stream
    stringstream timeText;

    //init();

    //Start counting frames per second
    int countedFrames = 0;

    fpsTimer.start();

    int rr = 0;

    //TCOD_Console * messages_layer = TCOD_console_new(20, 20);
    //messages_layer->setCustomFont(
            //"data/fonts/Alloy-curses-12x12.png",
            //TCOD_FONT_LAYOUT_ASCII_INROW);

    //TCOD_sys_get_sdl_window();
    //TCOD_sys_accumulate_console(messages_layer);

    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    while (!TCODConsole::root->isWindowClosed())
    {

        render_all(a);

        //Start cap timer
        capTimer.start();

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
                sdl_callback_enabled = !sdl_callback_enabled;

                if (sdl_callback_enabled) {
                    DEBUG("SDL ENABLED!");
                    TCODSystem::registerSDLRenderer(new SampleRenderer());
                } else {
                    DEBUG("SDL DISABLED!");
                    TCODSystem::registerSDLRenderer(NULL);
                    // we want libtcod to redraw the sample console even if nothing has changed in it
                    TCODConsole::root->setDirty(0, 0, SCREEN_WIDTH , SCREEN_HEIGHT);
                }
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

        /*
        if (rr > 20)
        {
            sdl_rect.x = rr % 200;
            sdl_rect.y = rr % 200;

            //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
            SDL_RenderFillRect(renderer, &sdl_rect);

            int frameTicks = capTimer.getTicks();
            if( frameTicks < SCREEN_TICKS_PER_FRAME )
            {
                //Wait remaining time
                //SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
                //SDL_Delay(1000);
            }

            DEBUG("Step 1");
            SDL_Delay(1000);
            //Render current frame
            loadMedia(renderer);
            gTextTexture.render( 300, 300 , renderer);

            DEBUG("Step 2");
            
            //SDL_Delay(1000);
            TCODConsole::root->flush();
            //TCODConsole::root->setDirty(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_Delay(1000);
            SDL_RenderPresent(renderer);
            //DEBUG("Presenting!");
            DEBUG("Step 3");
            SDL_Delay(1000);
            DEBUG("Step 4");
        }
        */

        TCODConsole::flush();

    }

    return 0;
}
