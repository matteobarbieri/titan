#include <cstdlib>  // For srand() and rand()

#include "../../libtcod.hpp"

#include "../../Entity.hpp"
#include "../../RenderOrder.hpp"
#include "../../map/GameMap.hpp"

#include "../../components/Fighter.hpp"
#include "../../components/Ai.hpp"

Entity * make_orc(Room * room, MonsterAi * ai_component)
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

    Fighter * fighter_component = new Fighter(20);

    // Create the AI for the monster if it is not passed
    if (ai_component == nullptr)
        ai_component = new MonsterAi();

    Entity * orc = new Entity(
        x, y, 'o',  TCODColor::desaturatedGreen,
        "Orc", ACTOR, true);

    orc->fighter = fighter_component;
    orc->ai = ai_component;

    return orc;

}
