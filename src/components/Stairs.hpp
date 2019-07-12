#ifndef ROGUE_20177_STAIRS
#define ROGUE_20177_STAIRS

#include "Component.hpp"

class Stairs : public Component
{
    public:
        
        // Attributes
        int floor;

        Stairs(int floor);
};

#endif /* ifndef ROGUE_20177_STAIRS */
