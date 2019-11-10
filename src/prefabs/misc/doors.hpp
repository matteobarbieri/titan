#ifndef ROGUE_20177_PREFABS_MISC_DOORS
#define ROGUE_20177_PREFABS_MISC_DOORS

class Entity;

Entity * make_door(int x, int y, bool open=false, bool locked=false, unsigned int key_id=0);

#endif /* ifndef ROGUE_20177_PREFABS_MISC_DOORS */
