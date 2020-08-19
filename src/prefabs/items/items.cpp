#include "../../libtcod.hpp"

#include "../../Symbols.h"

#include "../../Entity.hpp"

#include "../../components/Item.hpp"
#include "../../components/Usable.hpp"
//#include "../../components/WeaponAttack.hpp"
//#include "../../components/Equippable.hpp"

#include "../funcs/misc.hpp"

Entity * make_frag_grenade(int x, int y)
{

    // Create entity object
    Entity * grenade = new Entity(
        //-1, -1, // x, y
        x, y,
        '*', TCODColor::sky, "Frag grenade", ITEM,
        false, false);

    AOEUsable * usable_component = new AOEUsable();
    usable_component->radius = 4;
    usable_component->range = 10;

    grenade->usable = usable_component;
    usable_component->owner = grenade;

    Item * grenade_item_component = new Item(
        ItemType::CONSUMABLE, ItemSubtype::NONE);

    // Add damage effect
    DamageEnemiesInAreaEffect * damage_effect = new DamageEnemiesInAreaEffect(2, 100);
    grenade_item_component->effects.push_back(damage_effect);

    grenade->item = grenade_item_component;
    grenade_item_component->owner = grenade;

    return grenade;
}
