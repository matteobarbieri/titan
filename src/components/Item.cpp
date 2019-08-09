#include "Item.hpp"

//////////////////////////////////////
/////// ITEM TYPE OPERATORS //////////
//////////////////////////////////////

// Enable bitwise comparison for ItemType
ItemType operator |(ItemType lhs, ItemType rhs)
{
    return static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) |
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType operator &(ItemType lhs, ItemType rhs)
{
    return static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) &
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType operator ^(ItemType lhs, ItemType rhs)
{
    return static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) ^
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType operator ~(ItemType rhs)
{
    return static_cast<ItemType> (
        ~static_cast<std::underlying_type<ItemType>::type>(rhs)
    );
}

ItemType& operator &=(ItemType &lhs, ItemType rhs)
{
    lhs = static_cast<ItemType> (
        static_cast<std::underlying_type<ItemType>::type>(lhs) &
        static_cast<std::underlying_type<ItemType>::type>(rhs)
    );

    return lhs;
}

//////////////////////////////////////
////// ITEM SUBTYPE OPERATORS ////////
//////////////////////////////////////

// Enable bitwise comparison for ItemSubtype
ItemSubtype operator |(ItemSubtype lhs, ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) |
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype operator &(ItemSubtype lhs, ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) &
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype operator ^(ItemSubtype lhs, ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) ^
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype operator ~(ItemSubtype rhs)
{
    return static_cast<ItemSubtype> (
        ~static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );
}

ItemSubtype& operator &=(ItemSubtype &lhs, ItemSubtype rhs)
{
    lhs = static_cast<ItemSubtype> (
        static_cast<std::underlying_type<ItemSubtype>::type>(lhs) &
        static_cast<std::underlying_type<ItemSubtype>::type>(rhs)
    );

    return lhs;
}

//////////////////////////////////////
//////// ITEM CLASS METHODS //////////
//////////////////////////////////////

Item::Item(ItemType item_type, ItemSubtype item_subtype) :
    item_type(item_type), item_subtype(item_subtype)
{
}

Item::~Item()
{
}

bool Item::equipped()
{
    return _equipped;
}

json Item::to_json()
{
    json j;

    j["item_letter"] = item_letter;
    j["_equipped"] = _equipped;
    j["item_subtype"] = item_subtype;
    j["item_type"] = item_type;

    return j;
}

Item * Item::from_json(json j)
{
    Item * c = new Item(j["item_type"], j["item_subtype"]);

    // TODO complete with letter and equipped status

    return c;
}


/*

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
