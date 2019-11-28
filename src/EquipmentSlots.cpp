#include "EquipmentSlots.hpp"

// Enable bitwise comparison for EquipmentSlot
EquipmentSlot operator |(EquipmentSlot lhs, EquipmentSlot rhs)
{
    return static_cast<EquipmentSlot> (
        static_cast<std::underlying_type<EquipmentSlot>::type>(lhs) |
        static_cast<std::underlying_type<EquipmentSlot>::type>(rhs)
    );
}

EquipmentSlot operator &(EquipmentSlot lhs, EquipmentSlot rhs)
{
    return static_cast<EquipmentSlot> (
        static_cast<std::underlying_type<EquipmentSlot>::type>(lhs) &
        static_cast<std::underlying_type<EquipmentSlot>::type>(rhs)
    );
}

EquipmentSlot operator ^(EquipmentSlot lhs, EquipmentSlot rhs)
{
    return static_cast<EquipmentSlot> (
        static_cast<std::underlying_type<EquipmentSlot>::type>(lhs) ^
        static_cast<std::underlying_type<EquipmentSlot>::type>(rhs)
    );
}

EquipmentSlot operator ~(EquipmentSlot rhs)
{
    return static_cast<EquipmentSlot> (
        ~static_cast<std::underlying_type<EquipmentSlot>::type>(rhs)
    );
}

EquipmentSlot& operator &=(EquipmentSlot &lhs, EquipmentSlot rhs)
{
    lhs = static_cast<EquipmentSlot> (
        static_cast<std::underlying_type<EquipmentSlot>::type>(lhs) &
        static_cast<std::underlying_type<EquipmentSlot>::type>(rhs)
    );

    return lhs;
}

/*! The static method to access the singleton */
SlotName & SlotName::singleton()
{
    static SlotName instance;

    return instance;
}

SlotName::~SlotName()
{
}

SlotName::SlotName()
{
    slot_names[EquipmentSlot::MAIN_HAND] = "Main hand";
    slot_names[EquipmentSlot::OFF_HAND] = "Off hand";
    slot_names[EquipmentSlot::HEAD] = "Head";
    slot_names[EquipmentSlot::CHEST] = "Chest";
    slot_names[EquipmentSlot::HANDS] = "Hands";
    slot_names[EquipmentSlot::LEGS] = "Legs";
    slot_names[EquipmentSlot::FEET] = "Feet";
}


/*
const std::vector<std::string> EquipmentSlots::slot_names = {
    "Main hand",
    "Off hand",
    "Head",
    "Chest",
    "Hands",
    "Legs",
    "Feet"
    };
*/
