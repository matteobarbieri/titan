#ifndef R20177_SKILL
#define R20177_SKILL

#include <SDL2/SDL.h>

#include "../components/Usable.hpp"

// Forward declarations
class GameMap;
class Entity;
class GameState;

class Outcome;

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

    protected:

        /**
         * Context for the skill
         */
        GameMap * game_map;
        Entity * player;
        GameState * game_state;

        /**
         * The actual method which must be reimplemented by subclasses.
         */
        virtual Outcome * _use() = 0;

        /**
         * The actual method which must be reimplemented by subclasses.
         */
        virtual Outcome * _resolve(int, int);

        int range;

    public:

        /**
         * The skill's name
         */
        const char * name;

        /**
         * Add context required to use the skill.
         */
        virtual void set_context(
            GameMap * game_map, Entity * player,
            GameState * game_state);

        /**
         * Preload texture
         */
        void load_texture(SDL_Renderer * renderer);

        /**
         * Superclass method which is called to actually use a skill.
         */
        Outcome * use();

        /**
         * Superclass method which is called to actually use a skill.
         */
        Outcome * resolve(int, int);

        /**
         * Returns the icon texture. Loads it in case it is not already loaded
         */
        SDL_Texture * get_icon_texture();

        Skill(const char * name, const char * icon_path);
        ~Skill();
};

class SkillBlink : public Skill, public Targetable
{

    protected:

        /**
         * The actual method which must be reimplemented by subclasses.
         */
        Outcome * _use();

        Outcome * _resolve(int, int);

    public:

        SkillBlink(const char * name, const char * icon_path, int range);
};

#endif
