#include <string>
#include <sstream>

#include "../Constants.h"

#include "../libtcod.hpp"
//#include "libtcod/console/console.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include "../sdl/custom_renderer.hpp"
#include "../sdl/timer.hpp"
#include "../sdl/text.hpp"

const int SCREEN_FPS = 100;
//const int SCREEN_FPS = 5;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//#include "sdl/custom_renderer.cpp"
bool sdl_callback_enabled = false;

using namespace std;

//Rendered texture
LTexture gTextTexture;

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

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
    }

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }

     //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" );

	return success;
}

bool loadMedia(SDL_Renderer * renderer)
{
	//Loading success flag
	bool success = true;

	//Open the font
	//gFont = TTF_OpenFont( "16_true_type_fonts/lazy.ttf", 28 );

    TTF_Font *gFont = NULL;

	gFont = TTF_OpenFont( "DobkinPlain.ttf", 50 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
        SDL_Color textColor = { 0, 255, 0, 0xFF };
		if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor , gFont, renderer) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	gTextTexture.free();

	//Free global font
	//TTF_CloseFont( gFont );
	//gFont = NULL;

	////Destroy window	
	//SDL_DestroyRenderer( gRenderer );
	//SDL_DestroyWindow( gWindow );
	//gWindow = NULL;
	//gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void render_all(TCODConsole * a)
{
    TCODConsole::root->clear();
    a->clear();


    a->setDefaultBackground(TCODColor::green);
    a->setDefaultBackground(TCODColor::yellow);
    a->rect(0, 0, 10, 10, true);

    TCODConsole::blit(
        a,
        0, 0, 20, 20,
        TCODConsole::root,
        50, 10);

}

int main(int argc, char *argv[])
{

    TCODSystem::setFps(30);

    TCODConsole::root->setDefaultBackground(TCODColor::black);

    TCODConsole * a = new TCODConsole(20, 20);

    // Set Custom font to use
    TCODConsole::setCustomFont(
            "data/fonts/16x16-sb-ascii.png",
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

    init();

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
        //TCODConsole::root->flush();

    }

    return 0;
}
