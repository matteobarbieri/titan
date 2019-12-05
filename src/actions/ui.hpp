#ifndef ROGUE_20177_ACTION_UI
#define ROGUE_20177_ACTION_UI

#include "Action.hpp"

// Forward declarations
class Outcome;

class CycleTargetAction : public Action
{

    public:

        // Used to determine whether cycling forward or reverse.
        int direction;

        Outcome * _execute();

        CycleTargetAction(int direction=1);

};

#endif /* ifndef ROGUE_20177_ACTION_UI */
