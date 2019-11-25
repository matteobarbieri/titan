#ifndef ROGUE_20177_ITEM
#define ROGUE_20177_ITEM

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
//struct MenuOption;

class Entity;

#include "../ui/menus.hpp"

/*
 * The main type of the item.
 */
enum class ItemType
{
    NONE = 0x0,
    WEAPON = 0x1,
    ARMOR = 0x2,
    DEVICE = 0x4,
    CONSUMABLE = 0x8,
    QUEST = 0x16,
    MISC = 0x32
};


/*
 * Additional item properties.
 */
enum class ItemSubtype
{
    NONE = 0x0,
    MELEE = 0x1,
    RANGED = 0x2
};


/**
 * The item class represents items and their properties "while they are in the
 * backpack". Meaning that for instance the info about their properties as items
 * that can be equipped must be stored in an instance of the  `Equippable` class.
 */
class Item
{

    public:

        Entity * owner;

        int item_letter;

        // Base item properties
        ItemType item_type;
        ItemSubtype item_subtype;

        // Self explanatory equipped status
        bool equipped;

        // Used for key-like items, they control which doors/mechanisms they
        // unlock.
        unsigned int key_id;

        Item(ItemType, ItemSubtype, bool=false, unsigned int=0);

        /**
         * Returns the list of meaninful item inventory options for the item.
         * For instance, all of them can be dropped on the ground, but only
         * weapons can be equipped, items with a valid usable component can be
         * used etc.
         */
        std::vector<MenuOption> item_inventory_options();

        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Item * from_json(json j);
};

/*

from enum import Enum, auto

class Item:
    """
    Class describing an item
    """

    def __init__(
            self, item_types=[], item_subtypes=[],
            ):

        # Set item type[s] and subtype[s]
        self.item_types = item_types
        self.item_subtypes = item_subtypes

        # Is the item being equipped?
        self.equipped = False

    def get_inventory_options(self):
        """
        Return a list of possible actions for the item once it is selected
        from the inventory.
        """
        options = list()

        if self.is_equippable():
            if self.owner.item.equipped:
                # TODO change letter!
                options.append(('t', 'Unequip'))
            else:
                options.append(('e', 'Equip'))

        if self.is_consumable():
            options.append(('u', 'Use'))

        # Any item can be dropped
        options.append(('d', 'Drop'))

        return options

    def can_perform_action(self, action):

        if action == 'unequip':
            return self.is_equippable() and self.owner.item.equipped
        if action == 'equip':
            return self.is_equippable() and not self.owner.item.equipped
        if action == 'use':
            # TODO not only consumables can be used
            return self.is_consumable



    # def __init__(
            # self, item_types=[], item_subtypes=[],
            # use_function=None, targeting=False, targeting_message=None,
            # **kwargs):

        # # The function called when the item is used
        # self.use_function = use_function

        # # Other things that might be used by the use_function
        # self.function_kwargs = kwargs

        # # Set item type[s] and subtype[s]
        # self.item_types = item_types
        # self.item_subtypes = item_subtypes

        # # TODO these things must be probably reworked
        # self.targeting = targeting
        # self.targeting_message = targeting_message

    def is_armor(self):
        return ItemType.ARMOR in self.item_types

    def is_weapon(self):
        return ItemType.WEAPON in self.item_types

    def is_device(self):
        return ItemType.DEVICE in self.item_types

    def is_equippable(self):
        return self.is_armor() or self.is_weapon() or self.is_device()

    def is_consumable(self):
        return ItemType.CONSUMABLE in self.item_types

    def is_melee(self):
        return ItemSubtype.MELEE in self.item_subtypes

    def is_ranged(self):
        return ItemSubtype.RANGED in self.item_subtypes
*/

#endif /* ifndef ROGUE_20177_ITEM */
