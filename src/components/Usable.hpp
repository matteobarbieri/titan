#ifndef ROGUE_20177_USABLE
#define ROGUE_20177_USABLE

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class Entity;

class Usable
{

    public:

        Entity * owner;

        Usable();
        ~Usable();

        /**
         * Creates a json representation of the component
         */
        json to_json();

        virtual void _use() = 0;

        void use();

        static Usable * from_json(json j);
};

class Targetable
{
    public:

        int radius;
        int range;

        bool is_in_radius(int src_x, int src_y, int trg_x, int trg_y);
        bool is_in_range(int src_x, int src_y, int trg_x, int trg_y);


};

class AOEUsable : public Usable, public Targetable
{

    void _use() override;

};

#endif /* ifndef ROGUE_20177_USABLE */
