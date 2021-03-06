#ifndef ROGUE_20177_USABLE
#define ROGUE_20177_USABLE

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class Entity;
class Effect;
class Outcome;

class Usable
{

    public:

        Entity * owner;

        /**
         * The effects applied by the item once it's used
         */
        std::vector<Effect *> effects;

        // Whether the item is consumed on use
        bool is_consumable;

        Usable();
        ~Usable();

        /**
         * Creates a json representation of the component
         */
        virtual json to_json() = 0;

        virtual Outcome * _use() = 0;

        Outcome * use();

        static Usable * from_json(json j);
};

// TODO consider moving it somewhere else
class Targetable
{
    public:

        int radius;
        int range;

        Targetable(int, int);

        bool is_in_radius(int src_x, int src_y, int trg_x, int trg_y);
        bool is_in_range(int src_x, int src_y, int trg_x, int trg_y);

};

class AOEUsable : public Usable, public Targetable
{

    public:

        AOEUsable(bool, int, int);

        Outcome * _use() override;

        static AOEUsable * from_json(json j);
        json to_json() override;

        // TODO destructor also?
};

#endif /* ifndef ROGUE_20177_USABLE */
