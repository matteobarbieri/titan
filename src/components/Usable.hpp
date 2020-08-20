#ifndef ROGUE_20177_USABLE
#define ROGUE_20177_USABLE

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class Entity;
class Effect;

class Usable
{

    public:

        Entity * owner;

        /**
         * The effects applied by the item once it's used
         */
        std::vector<Effect *> effects;

        Usable();
        ~Usable();

        /**
         * Creates a json representation of the component
         */
        virtual json to_json() = 0;

        virtual void _use() = 0;

        void use();

        static Usable * from_json(json j);
};

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

        AOEUsable(int, int);

        void _use() override;

        static AOEUsable * from_json(json j);
        json to_json() override;

        // TODO destructor also?
};

#endif /* ifndef ROGUE_20177_USABLE */
