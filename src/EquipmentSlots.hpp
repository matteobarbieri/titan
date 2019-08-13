#ifndef ROGUE_20177_EQUIPMENT_SLOTS
#define ROGUE_20177_EQUIPMENT_SLOTS
#include <iostream>
#include <string>
#include <vector>

/*
 * The main type of the item.
 */
//enum class EquipmentSlots
class EquipmentSlots
{
    public:

        static const int MAIN_HAND = 0;
        static const int OFF_HAND = 1;
        static const int HEAD = 2;
        static const int CHEST = 3;
        static const int HANDS = 4;
        static const int LEGS = 5;
        static const int FEET = 6;
        
        static const std::vector<std::string> slot_names;

};

#endif /* ifndef ROGUE_20177_EQUIPMENT_SLOTS */
