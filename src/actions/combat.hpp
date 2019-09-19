#ifndef ROGUE_20177_ACTION_COMBAT
#define ROGUE_20177_ACTION_COMBAT

#include "Action.hpp"

// Forward declarations
class Outcome;

class AttackAction : public Action
{

    private:
        int search_target_in_range(int range, Entity ** target);

    public:

        Outcome * _execute();

};

#endif /* ifndef ROGUE_20177_ACTION_COMBAT */
