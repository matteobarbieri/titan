#ifndef ROGUE_20177_ARMOR_DEFENSE
#define ROGUE_20177_ARMOR_DEFENSE

#include <vector>

#include "../nlohmann/json.hpp"

// Forward declaration
class Entity;

using json = nlohmann::json;

// Represents an attack from a weapon
class ArmorDefense
{

    public:

        int armor_value;

        // Simple constructor which sets damage range.
        ArmorDefense(int);

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static ArmorDefense * from_json(json j);
};


#endif /* ifndef ROGUE_20177_ARMOR_DEFENSE */
