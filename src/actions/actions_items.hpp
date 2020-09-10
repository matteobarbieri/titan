#ifndef ROGUE_20177_ACTION_ITEMS
#define ROGUE_20177_ACTION_ITEMS

#include "Action.hpp"

// Forward declarations
class Outcome;

class ItemUseAction : public Action
{

    public:

        Outcome * _execute();

};

class ItemResolveTargetingAction : public Action
{

    public:

        int x, y;

        // Since it's a targeted action, require target coordinates
        ItemResolveTargetingAction(int, int);

        Outcome * _execute();

};

class DropItemAction : public Action
{

    public:

        Outcome * _execute();

};

// Retrieve an item from a container
class TransferItemAction : public Action
{

    public:

        // The general method called, which will turn up either being a "store"
        // or "retrieve" based on where the item currently is.
        Outcome * _execute();

        Outcome * _execute_retrieve();
        Outcome * _execute_store();

};

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

class SelectContainerItemAction : public Action
{

    public:

        char item_letter;

        SelectContainerItemAction(char item_letter);

        Outcome * _execute();

};

#endif /* ifndef ROGUE_20177_ACTION_ITEMS */
