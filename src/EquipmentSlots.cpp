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
