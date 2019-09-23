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

};

#endif /* ifndef ROGUE_20177_ACTION_SKILLS */
