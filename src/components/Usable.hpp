#ifndef ROGUE_20177_USABLE
#define ROGUE_20177_USABLE

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class Usable
{


    public:

        //Usable();
        //~Usable();

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Usable * from_json(json j);
};

#endif /* ifndef ROGUE_20177_USABLE */
