#ifndef ROGUE_20177_ACTION_COMBAT
#define ROGUE_20177_ACTION_COMBAT

#include "Action.hpp"

// Forward declarations
class Outcome;

class AttackAction : public Action
{

    public:

        Outcome * _execute();

};

#endif /* ifndef ROGUE_20177_ACTION_COMBAT */
