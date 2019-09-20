#ifndef R20177_PLAYER
#define R20177_PLAYER

#include <vector>

#include <SDL2/SDL_render.h>

// Forward declarations
class Entity;
class Skill;

class Player
{

    private:
        Player();
        ~Player();

    public:

        /*! The static method to access the singleton */
        static Player & singleton();

        /**
         * Pointer to the corresponding entity in the game
         */
        Entity * entity;

        void preload_skill_textures(SDL_Renderer * renderer);

        /**
         * Loadout of skills
         */
        std::vector<Skill *> skills;
};

#endif
