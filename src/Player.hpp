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
         * Given a char (the key pressed by the user), return the index
         * corresponding to the skill bound to that key (or -1 if no skill is
         * bound to that key).
         */
        int char_to_skill_index(char c);

        /**
         * Loadout of skills
         */
        std::vector<Skill *> skills;
};

#endif
