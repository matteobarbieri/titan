#ifndef ROGUE_20177_INVENTORY
#define ROGUE_20177_INVENTORY

#include <vector>

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

// Forward declarations
class Entity;
class GameMap;

class InventoryFullException : public std::exception
{
};

class Inventory
{

    private:

        // Keep track separately of items and their associated letters
        std::vector<Entity *> _items;
        std::vector<char> _item_letters;

        int _capacity;

    public:

        Inventory(int capacity);
        ~Inventory();

        std::vector<char> available_letters;

        // TODO return message?
        //void equip(Entity * item);
        
        // TODO return message?
        void pickup(Entity * item, GameMap * level);

        std::vector<Entity *> items();


        /**
         * Creates a json representation of the component
         */
        json to_json();

        static Inventory * from_json(json j);
};


#endif /* ifndef ROGUE_20177_INVENTORY */

/*
import tcod as libtcod

from game_messages import Message


class Inventory:

    def __init__(self, capacity):

        self.items = list()
        self.item_letters = list()


    def unequip(self, item):

        messages = self.owner.equipment.unequip(item)

        # Remove the item from the inventory
        # self.items.remove(item)
        # self.item_letters.remove(item.item_letter)

        # Return a feedback message
        return messages


    def drop(self, item, level):

        # Run the unequip procedure to remove object in a clean way
        if item.item.equipped:
            self.unequip(item)

        # TODO
        # For the time being, just drop it on the floor exactly where the
        # player is.
        item.x = self.owner.x
        item.y = self.owner.y

        # Remove the item from the inventory
        self.items.remove(item)
        self.item_letters.remove(item.item_letter)

        # Re-add the used letter to the list of available ones
        self.available_letters.append(item.item_letter)
        self.available_letters.sort()
        item.item_letter = None

        # Add it back to the map's entities
        level.entities.append(item)

        # Return a feedback message
        return Message("You drop a {} on the floor.".format(item),
                libtcod.white)



    def get_item_position_in_list(self, item):
        """
        """

        viable_list = [x for x in self.items
            if x.item.equipped == item.item.equipped]

        # return self.items.index(item)
        return viable_list.index(item)

    """

    def use(self, item_entity, **kwargs):
        results = []

        item_component = item_entity.item

        if item_component.use_function is None:
            equippable_component = item_entity.equippable

            if equippable_component:
                results.append({'equip': item_entity})
            else:
                results.append(
                    {
                        'message': Message(
                            'The {0} cannot be used'.format(
                                item_entity.name),
                            libtcod.yellow)
                    }
                )

        else:
            if item_component.targeting and not (kwargs.get('target_x') or kwargs.get('target_y')):
                results.append({'targeting': item_entity})
            else:
                kwargs = {**item_component.function_kwargs, **kwargs}
                item_use_results = item_component.use_function(self.owner, **kwargs)

                for item_use_result in item_use_results:
                    if item_use_result.get('consumed'):
                        self.remove_item(item_entity)

                results.extend(item_use_results)

        return results
        """

*/
