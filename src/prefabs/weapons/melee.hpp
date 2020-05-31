#ifndef ROGUE_20177_PREFABS_WEAPONS_MELEE
#define ROGUE_20177_PREFABS_WEAPONS_MELEE

// Forward declaration
class Entity;

// A dagger
Entity * make_dagger(int, int);
Entity * make_dagger();

// A stun baton
Entity * make_baton(int, int, int, int);
Entity * make_baton(int, int);
Entity * make_baton();

#endif /* ifndef ROGUE_20177_PREFABS_WEAPONS_RANGED */
