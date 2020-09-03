#ifndef ROGUE_20177_ENEMIES
#define ROGUE_20177_ENEMIES

// Forward declarations
class Entity;
class Room;
class MonsterAi;

Entity * make_orc(Room * room, MonsterAi * ai_component=nullptr);

// Security droid: melee fighter
Entity * make_security_droid(Room * room, MonsterAi * ai_component=nullptr);
Entity * make_security_droid(int x, int y, MonsterAi * ai_component=nullptr);

// Guard: equipped with a ranged weapon
Entity * make_guard(int x, int y, MonsterAi * ai_component=nullptr);

#endif /* ifndef ROGUE_20177_ENEMIES */
