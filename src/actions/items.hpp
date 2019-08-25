#ifndef ROGUE_20177_ACTION_ITEMS
#define ROGUE_20177_ACTION_ITEMS value

#include "Action.hpp"

// Forward declarations
class Outcome;

class PickupAction : public Action
{

    public:

        Outcome * _execute();

};

class ItemEquipToggleAction : public Action
{

    public:

        // Constructors
        //EquipItemAction();
        //~EquipItemAction();

        Outcome * _execute();

        Outcome * equip();
        Outcome * unequip();

};


/*
class DropItemAction : public Action
{

    public:

        // Constructors
        DropItemAction();
        ~DropItemAction();

        Outcome * _execute();

};

class UnequipItemAction : public Action
{

    public:

        // Constructors
        UnequipItemAction();
        ~UnequipItemAction();

        Outcome * _execute();

};

*/

#endif /* ifndef ROGUE_20177_ACTION_ITEMS */
