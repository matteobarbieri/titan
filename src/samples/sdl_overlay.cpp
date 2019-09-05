#include "../Constants.h"

#include "../libtcod.hpp"
//#include "libtcod/console/console.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

//#include "sdl/custom_renderer.cpp"

using namespace std;

/*

*/

int main(int argc, char *argv[])
{

    // Set Custom font to use
    TCODConsole::setCustomFont(
            "../data/fonts/16x16-sb-ascii.png",
            TCOD_FONT_LAYOUT_ASCII_INROW);

    // Init root console
    TCODConsole::initRoot(
        SCREEN_WIDTH, SCREEN_HEIGHT,
        //WINDOW_TITLE, false, TCOD_RENDERER_SDL2);
        //WINDOW_TITLE, false, TCOD_RENDERER_GLSL);
        WINDOW_TITLE, false, TCOD_RENDERER_SDL);

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

    //TCODSystem::registerSDLRenderer(new SampleRenderer());

    DEBUG("Before!");
    SDL_Renderer * renderer = TCOD_sys_get_SDL_renderer();
    SDL_Window * sdl_window = TCOD_sys_get_SDL_window();

    bool bbb = renderer == NULL;
    bool aaa = sdl_window == NULL;
    DEBUG(bbb);
    DEBUG(aaa);
    DEBUG("After!");

    SDL_Rect sdl_rect;

    sdl_rect.x = 0;
    sdl_rect.y = 0;
    sdl_rect.w = 256;
    sdl_rect.h = 256;

    int rr = 0;

    //TCOD_Console * messages_layer = TCOD_console_new(20, 20);
    //messages_layer->setCustomFont(
            //"data/fonts/Alloy-curses-12x12.png",
            //TCOD_FONT_LAYOUT_ASCII_INROW);

    //TCOD_sys_get_sdl_window();
    //TCOD_sys_accumulate_console(messages_layer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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
            // Load game
            SaveGame sg;
            sg.load("latest.json", &player, &game_map, &game_state);

            play_game(player, game_map, game_state);

            load_game = false;
            //break;
        }

        if (play_game_)
        {

            // Start a new game
            
            // When returning to main menu, reset play_game variable to false
            play_game_ = false;
        }

        
        rr++;
        DEBUG(rr);

        if (rr > 20)
        {

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
        SDL_RenderFillRect(renderer, &sdl_rect);

        SDL_RenderPresent(renderer);
        //DEBUG("Presenting!");
        }

        TCODConsole::root->flush();

        // Display the main title menu
        main_menu(&main_menu_background_image);
    }

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
