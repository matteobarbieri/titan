#ifndef ROGUE_20177_RELOADABLE
#define ROGUE_20177_RELOADABLE

#include "../nlohmann/json.hpp"

// Forward declaration
//class Entity;
//class WeaponAttack;
//class ArmorDefense;

using json = nlohmann::json;

class Reloadable
{

    public:

        // The max amount of ammo that the weapon can use
        int clip_size;

        // The number of ammunition currently loaded in the weapon
        int ammo;

        // How many ammunition are used up for a single shot
        int ammo_per_shot;

        // Wether the weapon actually need ammo to be present in Player's
        // inventory to be reloaded (i.e. for particularly powerful weapons).
        bool need_ammo;

        // Main constructor, sets all properties
        Reloadable (int, int, int, bool);

        // Simplified constructor, only requires clip_size and sets the
        // remaining properties using reasonable values.
        Reloadable (int);

        /**
         * Use a number of ammo (depending on ammo_per_shot property). Return
         * true if there is enough ammo in the weapon to fire a shot, false
         * otherwise.
         */
        bool consume_ammo();

        /**
         * Reload the weapon. Returns true if the weapon can be reloaded (i.e.
         * it is not full already and there are ammo in player's inventory, in
         * case it is a weapon that requires actual ammo in inventory), false
         * otherwise.
         */
        bool reload();

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Reloadable * from_json(json j);
};

#endif /* ifndef ROGUE_20177_RELOADABLE */
