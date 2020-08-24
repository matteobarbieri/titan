#ifndef ROGUE_20177_ACTION_SKILLS
#define ROGUE_20177_ACTION_SKILLS

#include "Action.hpp"

// Forward declarations
class Outcome;
class Skill;

class UseSkillAction : public Action
{

    public:

        Skill * skill;

        UseSkillAction(Skill * skill);

        Outcome * _execute();

        /**
         * Also set context for the skill
         */
        void set_context(
            GameMap * game_map, Entity * player, TCODMap * fov_map,
            GameState * game_state);
};

class ResolveSkillAction : public Action
{

    public:

        //Skill * skill;
        int x, y;

        //ResolveSkillAction(Skill * skill);
        ResolveSkillAction(int x, int y);

        Outcome * _execute();

        /**
         * Also set context for the skill
         */
        void set_context(
            GameMap * game_map, Entity * player, TCODMap * fov_map,
            GameState * game_state);
};

#endif /* ifndef ROGUE_20177_ACTION_SKILLS */
