#ifndef R20177_SKILL
#define R20177_SKILL

#include <SDL2/SDL.h>

class Skill
{
    private:

        /**
         * The path to the skill's icon
         */
        const char * icon_path;

        /**
         * The preloaded SDL texture representing the skill's icon
         */
        SDL_Texture * icon_texture;

    public:

        /**
         * The skill's name
         */
        const char * name;

        /**
         * Preload texture
         */
        void load_texture(SDL_Renderer * renderer);

        /**
         * Returns the icon texture. Loads it in case it is not already loaded
         */
        SDL_Texture * get_icon_texture();

        Skill(const char * name, const char * icon_path);
        ~Skill();
};

#endif

