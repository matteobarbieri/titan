#include <string>
#include <sstream>

#include "../Constants.h"

#include "../libtcod.hpp"
//#include "libtcod/console/console.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

#include "../sdl/timer.hpp"
#include "../sdl/text.hpp"

const int SCREEN_FPS = 100;
//const int SCREEN_FPS = 5;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//#include "sdl/custom_renderer.cpp"
bool sdl_callback_enabled = false;

using namespace std;

class SampleRenderer : public ITCODSDLRenderer {
public :
	SampleRenderer() : effectNum(0), delay(3.0f) {
		noise=new TCODNoise(3);
	}
	~SampleRenderer() {
		delete noise;
	}
	void render(void *sdlSurface) {
		SDL_Surface *screen = (SDL_Surface *)sdlSurface;
		// now we have almighty access to the screen's precious pixels !!
		// get the font character size
		int charw,charh;
		TCODSystem::getCharSize(&charw,&charh);
		// compute the sample console position in pixels
		int samplex = 0 * charw;
		int sampley = 0 * charh;
		delay -= TCODSystem::getLastFrameLength();
		if ( delay < 0.0f ) {
			delay = 3.0f;
			effectNum = (effectNum + 1) % 3;
			if ( effectNum == 2 ) sdl_callback_enabled=false; // no forced redraw for burn effect
			else sdl_callback_enabled=true;
		}
		switch(effectNum) {
			case 0 : blur(screen,samplex,sampley,SCREEN_WIDTH * charw,SCREEN_HEIGHT * charh); break;
			case 1 : explode(screen,samplex,sampley,SCREEN_WIDTH * charw,SCREEN_HEIGHT * charh); break;
			case 2 : burn(screen,samplex,sampley,SCREEN_WIDTH * charw,SCREEN_HEIGHT * charh); break;
		}
	}

protected :
	TCODNoise *noise;
	int effectNum;
	float delay;

	void burn(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh) {
		int ridx=screen->format->Rshift/8;
		int gidx=screen->format->Gshift/8;
		int bidx=screen->format->Bshift/8;
		for (int x=samplex; x < samplex + samplew; x ++ ) {
			uint8_t *p = (uint8_t *)screen->pixels + x * screen->format->BytesPerPixel + sampley * screen->pitch;
			for (int y=sampley; y < sampley + sampleh; y ++ ) {
				int ir=0,ig=0,ib=0;
				uint8_t *p2 = p + screen->format->BytesPerPixel; // get pixel at x+1,y
				ir += p2[ridx];
				ig += p2[gidx];
				ib += p2[bidx];
				p2 -= 2*screen->format->BytesPerPixel; // get pixel at x-1,y
				ir += p2[ridx];
				ig += p2[gidx];
				ib += p2[bidx];
				p2 += screen->format->BytesPerPixel+ screen->pitch; // get pixel at x,y+1
				ir += p2[ridx];
				ig += p2[gidx];
				ib += p2[bidx];
				p2 -= 2*screen->pitch; // get pixel at x,y-1
				ir += p2[ridx];
				ig += p2[gidx];
				ib += p2[bidx];
				ir/=4;
				ig/=4;
				ib/=4;
				p[ridx]=ir;
				p[gidx]=ig;
				p[bidx]=ib;
				p += screen->pitch;
			}
		}
	}

	void explode(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh) {
		int ridx=screen->format->Rshift/8;
		int gidx=screen->format->Gshift/8;
		int bidx=screen->format->Bshift/8;
		TCODRandom *rng=TCODRandom::getInstance();
		int dist=(int)(10*(3.0f - delay));
		for (int x=samplex; x < samplex + samplew; x ++ ) {
			uint8_t *p = (uint8_t *)screen->pixels + x * screen->format->BytesPerPixel + sampley * screen->pitch;
			for (int y=sampley; y < sampley + sampleh; y ++ ) {
				int ir=0,ig=0,ib=0;
				for (int i=0; i < 3; i++) {
					int dx = rng->getInt(-dist,dist);
					int dy = rng->getInt(-dist,dist);
					uint8_t *p2;
					p2 = p + dx * screen->format->BytesPerPixel;
					p2 += dy * screen->pitch;
					ir += p2[ridx];
					ig += p2[gidx];
					ib += p2[bidx];
				}
				ir/=3;
				ig/=3;
				ib/=3;
				p[ridx]=ir;
				p[gidx]=ig;
				p[bidx]=ib;
				p += screen->pitch;
			}
		}
	}

	void blur(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh) {
		// let's blur that sample console
		float f[3],n=0.0f;
		int ridx=screen->format->Rshift/8;
		int gidx=screen->format->Gshift/8;
		int bidx=screen->format->Bshift/8;
		f[2]=TCODSystem::getElapsedSeconds();
		for (int x=samplex; x < samplex + samplew; x ++ ) {
			uint8_t *p = (uint8_t *)screen->pixels + x * screen->format->BytesPerPixel + sampley * screen->pitch;
			f[0]=(float)(x)/samplew;
			for (int y=sampley; y < sampley + sampleh; y ++ ) {
				int ir=0,ig=0,ib=0;
				if ( (y-sampley)%8 == 0 ) {
					f[1]=(float)(y)/sampleh;
					n=noise->getFbm(f,3.0f);
				}
				int dec = (int)(3*(n+1.0f));
				int count=0;
				switch(dec) {
					case 4:
						count += 4;
						// get pixel at x,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p -= 2*screen->format->BytesPerPixel; // get pixel at x+2,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p -= 2*screen->pitch; // get pixel at x+2,y+2
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p+= 2*screen->format->BytesPerPixel; // get pixel at x,y+2
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p += 2*screen->pitch;
					case 3:
						count += 4;
						// get pixel at x,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p += 2*screen->format->BytesPerPixel; // get pixel at x+2,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p += 2*screen->pitch; // get pixel at x+2,y+2
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p-= 2*screen->format->BytesPerPixel; // get pixel at x,y+2
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p -= 2*screen->pitch;
					case 2:
						count += 4;
						// get pixel at x,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p -= screen->format->BytesPerPixel; // get pixel at x-1,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p -= screen->pitch; // get pixel at x-1,y-1
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p+= screen->format->BytesPerPixel; // get pixel at x,y-1
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p += screen->pitch;
					case 1:
						count += 4;
						// get pixel at x,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p += screen->format->BytesPerPixel; // get pixel at x+1,y
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p += screen->pitch; // get pixel at x+1,y+1
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p-= screen->format->BytesPerPixel; // get pixel at x,y+1
						ir += p[ridx];
						ig += p[gidx];
						ib += p[bidx];
						p -= screen->pitch;
						ir/=count;
						ig/=count;
						ib/=count;
						p[ridx]=ir;
						p[gidx]=ig;
						p[bidx]=ib;
					break;
					default:break;
				}
				p += screen->pitch;
			}
		}
	}
};


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
