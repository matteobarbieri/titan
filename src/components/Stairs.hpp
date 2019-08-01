#ifndef ROGUE_20177_STAIRS
#define ROGUE_20177_STAIRS

#include "Component.hpp"

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class Stairs : public Component
{
    public:
        
        // Attributes
        int floor;

        Stairs(int floor);

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Stairs * from_json(json j);
};

#endif /* ifndef ROGUE_20177_STAIRS */
