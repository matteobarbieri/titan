#ifndef ROGUE_20177_LEVEL
#define ROGUE_20177_LEVEL

#include "../nlohmann/json.hpp"

// Forward declaration
class Entity;

using json = nlohmann::json;

class Level
{

    public:

        Entity * owner;

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Level * from_json(json j);
};

#endif /* ifndef ROGUE_20177_LEVEL */
