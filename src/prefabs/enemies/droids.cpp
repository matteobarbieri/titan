#include <cstdlib>  // For srand() and rand()

#include "../../libtcod.hpp"

#include "../../Entity.hpp"
#include "../../RenderOrder.hpp"
#include "../../map/GameMap.hpp"

#include "../../EquipmentSlots.hpp"

#include "../../components/Fighter.hpp"
#include "../../components/Equipment.hpp"
#include "../../components/Ai.hpp"

#include "../weapons/melee.hpp"

Entity * make_security_droid(int x, int y, MonsterAi * ai_component)
{
    Entity * droid = new Entity(
        x, y, 'd',  TCODColor::lightYellow,
        "Security Droid", ACTOR, true);

    // Fighter
    Fighter * fighter_component = new Fighter(20, 30, 0);
    droid->fighter = fighter_component;
    fighter_component->owner = droid;

    // AI
    // Create the AI for the monster if it is not passed
    if (ai_component == nullptr)
    {
        ai_component = new ImmobileAi();
    }
    droid->ai = ai_component;
    ai_component->owner = droid;

    // Equipment component
    droid->equipment = new Equipment();
    droid->equipment->owner = droid;

    droid->equipment->slots[EquipmentSlot::MAIN_HAND] = make_baton();

    return droid;
}

Entity * make_security_droid(Room * room, MonsterAi * ai_component)
{
    // Use proxy variables
    int x1 = room->xy.x1;
    int x2 = room->xy.x2;
    int y1 = room->xy.y1;
    int y2 = room->xy.y2;

    // To produce random int in a given range
    int dx = x2 - 1 - x1; // -1 + 1
    int dy = y2 - 1 - y1; // -1 + 1

    // Choose starting coordinates
    int x = rand() % dx + x1 + 1;
    int y = rand() % dy + y1 + 1;

    return make_security_droid(x, y, ai_component);
}
