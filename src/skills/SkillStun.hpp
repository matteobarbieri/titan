#ifndef R20177_SKILL_STUN
#define R20177_SKILL_STUN

#include <SDL2/SDL.h>
#include "Skill.hpp"

class SkillStun : public Skill
{

    protected:

        /**
         * The actual method which must be reimplemented by subclasses.
         */
        Outcome * _use();

    public:

        SkillStun(const char * name, const char * icon_path);
};

#endif

