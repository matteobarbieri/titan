#include <SDL2/SDL.h>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../Constants.h"
#include "../libtcod.hpp"

#include "text.hpp"

#include "custom_renderer.hpp"

#define SAMPLE_SCREEN_X 50
#define SAMPLE_SCREEN_Y 10
#define SAMPLE_SCREEN_W 20
#define SAMPLE_SCREEN_H 20

//Rendered texture
LTexture gTextTexture;

bool init()
{
	//Initialization flag
	bool success = true;

    /*
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
    */

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

	gFont = TTF_OpenFont( "DobkinPlain.ttf", 10 );
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


SampleRenderer::SampleRenderer() : effectNum(0), delay(3.0f) {
    noise=new TCODNoise(3);
    renderer = NULL;
}

SampleRenderer::~SampleRenderer() {
    delete noise;
}

void SampleRenderer::render_square(int samplex, int sampley, int samplew, int sampleh)
{
    SDL_Rect sdl_rect;

    sdl_rect.x = samplex + samplew/2;
    sdl_rect.y = sampley + sampleh/2;
    //sdl_rect.w = samplew;
    //sdl_rect.h = sampleh;
    sdl_rect.w = 20;
    sdl_rect.h = 20;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 10);
    SDL_RenderFillRect(renderer, &sdl_rect);

    loadMedia(renderer);
    gTextTexture.render( 300, 300 , renderer);
}

void SampleRenderer::render(void *sdlSurface) {

    SDL_Surface *screen = (SDL_Surface *)sdlSurface;

    // Initialize/create renderer the first time
    if (renderer == NULL)
    {
        DEBUG("Initilizing renderer!");
        init();
        renderer = SDL_CreateSoftwareRenderer(screen);
    }

    //DEBUG("Here!");
    // now we have almighty access to the screen's precious pixels !!
    // get the font character size
    int charw,charh;
    TCODSystem::getCharSize(&charw,&charh);

    // compute the sample console position in pixels
    int samplex = SAMPLE_SCREEN_X * charw;
    int sampley = SAMPLE_SCREEN_Y * charh;

    delay -= TCODSystem::getLastFrameLength();
    if ( delay < 0.0f ) {
        delay = 3.0f;
        effectNum = (effectNum + 1) % 3;
    }
    switch(effectNum) {
        //case 0 : blur(screen,samplex,sampley,SAMPLE_SCREEN_W * charw, SAMPLE_SCREEN_H * charh); break;
        //case 1 : explode(screen,samplex,sampley, SAMPLE_SCREEN_W * charw, SAMPLE_SCREEN_H* charh); break;
        case 0 : render_square(samplex,sampley, SAMPLE_SCREEN_W * charw, SAMPLE_SCREEN_H* charh); break;
        case 1 : render_square(samplex,sampley, SAMPLE_SCREEN_W * charw, SAMPLE_SCREEN_H* charh); break;
        case 2 : render_square(samplex,sampley, SAMPLE_SCREEN_W * charw, SAMPLE_SCREEN_H* charh); break;
        //case 2 : burn(screen,samplex,sampley, SAMPLE_SCREEN_W * charw, SAMPLE_SCREEN_H* charh); break;
        //case 2 : explode(screen,samplex,sampley, SAMPLE_SCREEN_W * charw, SAMPLE_SCREEN_H* charh); break;
    }
}

void SampleRenderer::burn(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh) {
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

void SampleRenderer::explode(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh) {
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

void SampleRenderer::blur(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh) {
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
