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
         * How often a skill can be used
         */
        int cooldown_max;

        /**
         * How much cooldown the skill currently has
         */
        int cooldown;

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
         * Reduces cooldown by one. Should be called at the end of each turn
         * (after monsters have taken their action, similar to debuff tick).
         */
        void tick_cooldown();

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

        Skill(const char * name, const char * icon_path, int max_cooldown);
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

        SkillBlink(const char * name, const char * icon_path, int cooldown_max, int range);
};

#endif
