#ifndef ROGUE_20177_EQUIPMENT_SLOTS
#define ROGUE_20177_EQUIPMENT_SLOTS
#include <iostream>
#include <string>
#include <vector>

#include <map>

/*
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
*/

/*
 * 
 */
enum class EquipmentSlot
{
    NONE = 0x0,
    MAIN_HAND = 0x1,
    OFF_HAND = 0x2,
    HEAD = 0x4,
    CHEST = 0x8
};

class SlotName
{

    private:
        SlotName();
        ~SlotName();

    public:

        std::map<EquipmentSlot, std::string> slot_names;

        /*! The static method to access the singleton */
        static SlotName & singleton();

};

EquipmentSlot operator | (EquipmentSlot lhs, EquipmentSlot rhs);
EquipmentSlot operator & (EquipmentSlot lhs, EquipmentSlot rhs);

#endif /* ifndef ROGUE_20177_EQUIPMENT_SLOTS */
