#include <algorithm>

#include "Inventory.hpp"

#include "../Entity.hpp"
#include "../map/GameMap.hpp"

#include "Item.hpp"

Inventory::Inventory(int capacity, bool init_available_letters) : _capacity(capacity)
{

        // The list of all letters available for an item
        // TODO limit to the maximum number of items a player can have on him,
        // between equipped and in inventory
        
        if (init_available_letters)
        {
            // Skip the 'i'
            for (int i='a'; i<'h'; i++)
            {
                available_letters.push_back(i);
            }
            
            for (int i='j'; i<='z'; i++)
            {
                available_letters.push_back(i);
            }
        }

}

int Inventory::get_item_position_in_list(Entity * item)
{

    // TODO must change, doesn't work well because of equipped status
    for (int i=0; i<(int)items.size(); i++)
    {
        if (items[i] == item)
        {
            return i;
        }
    }

    DEBUG("Inventory::get_item_position_in_list should not arrive here...");

    // Should really never arrive here
    throw new std::exception();

}

void Inventory::drop(Entity * item, GameMap * level, Entity * player)
{

    // TODO obtain coordinates by map (get closest free tile
    item->x = player->x;
    item->y = player->y;

    // Add item letter back to the pool of available letters
    available_letters.push_back(item->item->item_letter);

    // Reset item letter assigned to item
    item->item->item_letter = -1;

    // Sort available item letters
    std::sort(available_letters.begin(), available_letters.end());
    
    // Mark item as unequipped
    item->item->equipped = false;

    // Add to the vector of items
    remove_item(item);

    level->add_entity(item);
}

void Inventory::remove_item(Entity * item)
{
    // Solution taken from
    // https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
    items.erase(
        std::remove(items.begin(), items.end(), item),
        items.end());
}

void Inventory::pickup(Entity * item, GameMap * level)
{
    // If at full capacity, throw exception
    if ((int)items.size() == _capacity)
    {
        throw InventoryFullException();
    }

    // Set item's coordinates to invalid values, since they do not make any
    // sense while the item is in player's inventory
    item->x = -1;
    item->y = -1;

    // Actually add the item to the inventory, Associating it with the
    // first available letter

    // "Manual" pop(0)
    char item_letter = available_letters[0];
    available_letters.erase(available_letters.begin());

    // Mark item as unequipped
    item->item->equipped = false;

    // Assign item letter to item component
    item->item->item_letter = item_letter;

    //DEBUG("Assigned letter " << item_letter);

    // Add to the vector of items
    items.push_back(item);

    // Remove entity from the level's list of entities
    level->remove_entity(item);
}

json Inventory::to_json()
{
    json j;

    j["_capacity"] = _capacity;

    // Items
    json j_items;

    for (int i=0; i<(int)items.size(); i++)
    {
        j_items.push_back(items[i]->to_json());
    }

    j["items"] = j_items;

    // Available letters
    json j_available_letters;

    for (int i=0; i<(int)available_letters.size(); i++)
    {
        j_available_letters.push_back(available_letters[i]);
    }

    j["available_letters"] = j_available_letters;

    return j;
}

Inventory * Inventory::from_json(json j)
{

    // Initialize inventory component WITHOUT also initializing the list of
    // available letters (must be restored from json object).
    Inventory * c = new Inventory(j["_capacity"], false);

    // Items
    for (int i=0; i<(int)j["items"].size(); i++)
    {
        c->items.push_back(Entity::from_json(j["items"][i]));
    }
    
    // Items letters
    for (int i=0; i<(int)j["available_letters"].size(); i++)
    {
        c->available_letters.push_back((int)j["available_letters"][i]);
    }

    return c;
}



/*
from game_messages import Message



class Inventory:


    def unequip(self, item):

        messages = self.owner.equipment.unequip(item)

        # Remove the item from the inventory
        # self.items.remove(item)
        # self.item_letters.remove(item.item_letter)

        # Return a feedback message
        return messages

    def equip(self, item):

        messages = self.owner.equipment.equip(item)

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
