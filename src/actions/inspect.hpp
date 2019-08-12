#ifndef ROGUE_20177_ACTION_INSPECT
#define ROGUE_20177_ACTION_INSPECT

#include "Action.hpp"
#include "Outcome.hpp"

class InspectAction : public Action
{
    public:

        // Constructors
        InspectAction(int x, int y);

        // The two integers determining direction
        int x;
        int y;

        Outcome * _execute();

};

#endif /* ifndef ROGUE_20177_ACTION_INSPECT */
