#ifndef ROGUE_20177_ACTION_MOVE
#define ROGUE_20177_ACTION_MOVE

#include "Action.hpp"

// Forward declarations
class Outcome;

class WaitAction : public Action
{

    public:

        // Constructors

        // The two integers determining direction

        Outcome * _execute();

};

class MoveAction : public Action
{

    public:

        // Constructors
        MoveAction(int dx, int dy);
        ~MoveAction();

        // The two integers determining direction
        int dx;
        int dy;

        Outcome * _execute();

};

#endif /* ifndef ROGUE_20177_ACTION_MOVE */
