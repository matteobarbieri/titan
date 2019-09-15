#include "../libtcod.hpp"

#include <SDL2/SDL.h>

class SampleRenderer : public ITCODSDLRenderer {
    public:

        SampleRenderer();
        ~SampleRenderer();
        void render(void *sdlSurface); 

    protected :

        TCODNoise *noise;
        int effectNum;
        float delay;

        SDL_Renderer * renderer;

        void burn(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh); 
        void explode(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh);
        void blur(SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh);

};
