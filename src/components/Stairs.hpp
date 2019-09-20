#ifndef ROGUE_20177_STAIRS
#define ROGUE_20177_STAIRS

#include "../nlohmann/json.hpp"

// Forward declaration
class Entity;

using json = nlohmann::json;

class Stairs
{
    public:
        
        Entity * owner;

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
