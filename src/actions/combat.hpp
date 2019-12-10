#ifndef ROGUE_20177_ACTION_COMBAT
#define ROGUE_20177_ACTION_COMBAT

#include "Action.hpp"
#include "../EquipmentSlots.hpp"

// Forward declarations
class Outcome;

class AttackAction : public Action
{

    public:

        Outcome * _execute();

};

class ReloadAction : public Action
{

    public:

        EquipmentSlot weapon_to_reload;
        ReloadAction(EquipmentSlot=EquipmentSlot::MAIN_HAND);

        Outcome * _execute();

};


#endif /* ifndef ROGUE_20177_ACTION_COMBAT */
