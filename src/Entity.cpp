#include <string>
#include <sstream>

#include "libtcod.hpp"

#include "Entity.hpp"
#include "RenderOrder.hpp"

#include "GameMessages.hpp"
#include "GameState.hpp"

#include "SaveGame.hpp"
#include "Uid.hpp"

// Components
#include "components/Fighter.hpp"
#include "components/Ai.hpp"
#include "components/Stairs.hpp"
#include "components/Item.hpp"
#include "components/Level.hpp"
#include "components/Equipment.hpp"
#include "components/Equippable.hpp"
#include "components/Usable.hpp"
#include "components/Inventory.hpp"
#include "components/Interactive.hpp"

// Misc
#include "buffs/Buff.hpp"

Entity::Entity(int x, int y, int symbol,
    TCODColor color, std::string name, RenderOrder render_order,
    bool blocks, bool blocks_sight, bool _fixed,
    unsigned long int id) :
    x(x), y(y), _render_order(render_order), name(name),
    symbol(symbol), _color(color),
    _blocks(blocks), _blocks_sight(blocks_sight),
    _fixed(_fixed), tag("")
{
    
        // Initialize pointers to NULL
        fighter = nullptr;
        ai = nullptr;
        stairs = nullptr;
        item = nullptr;
        level = nullptr;
        equipment = nullptr;
        equippable = nullptr;
        usable = nullptr;
        inventory = nullptr;
        interactive = nullptr;

        // If parameter id is default, initialize it with a progressive one
        if (id == 0)
            _id = Uid::singleton().next_id();
        else
            _id = id;

}

/*
bool Entity::operator < (const Entity & other) const
{
    std::cout << this->name << ": " << this->render_order() <<
        other.name << ": " << other.render_order() << std::endl;

    std::cout << "comparison: " << (this->render_order() < other.render_order()) << std::endl;

    return (this->render_order() < other.render_order());
}
*/

void Entity::apply_buff(Buff * buff)
{
    buffs.push_back(buff);
}


bool Entity::is_valid_target() const
{
    // TODO make more complicated, take factions into account!
    return fighter != nullptr;
}


bool Entity::is_disabled() const
{

    for (std::vector<Buff *>::const_iterator b = buffs.begin() ; b != buffs.end(); ++b)
    {
        if ((*b)->disables_entity())
        {
            return true;
        }
    }

    return false;
}

void Entity::interact_with(Entity * other, GameMap * game_map, GameState * game_state)

{

    // TODO must be more complex than this (i.e., take into account factions 
    // etc)!
    if (other->fighter != nullptr)
    {
        fighter->attack_melee(other);
        if (other->fighter->is_dead())
        {
            other->die();
        }
        game_state->game_phase = ENEMY_TURN;
    }
    else if (other->interactive != nullptr)
    {
        // Assume game phase is determined by the side effect of the specific
        // implementation of the interact method.
        other->interactive->interact(this, game_map, game_state);
    }

}

// GETTERS
RenderOrder Entity::render_order() const
{
    return _render_order;
}

TCODColor Entity::color() const
{
    return _color;
}

bool Entity::fixed() const
{
    return _fixed;
}

bool Entity::blocks() const
{
    return _blocks;
}

bool Entity::blocks_sight() const
{
    return _blocks_sight;
}

// SETTERS
void Entity::render_order(RenderOrder ro)
{
    _render_order = ro;
}

void Entity::color(TCODColor c)
{
    _color = c;
}

void Entity::fixed(bool f)
{
    _fixed = f;
}

void Entity::blocks(bool b)
{
    _blocks = b;
}

void Entity::blocks_sight(bool b)
{
    _blocks_sight = b;
}

void Entity::die()
{

    // Build message
    std::ostringstream stringStream;

    stringStream << "The " << 
        name << " is dead!";

    // Add message to message log
    MessageLog::singleton().add_message(
        {stringStream.str(), TCODColor::yellow});

    // Update appearance
    symbol = '%';
    color(TCODColor::darkRed);
    render_order(CORPSE);
    blocks(false);

    // Name
    stringStream.str("");
    stringStream << "remains of " << name;
    name = stringStream.str();

    // Only monsters (that is, entities which are not the players) have the ai
    // component.
    if (ai != nullptr)
    {
        delete ai;
        ai = nullptr;

        delete fighter;
        fighter = nullptr;
    }

}


json Entity::to_json()
{
    // TODO to complete
    json json_data;

    // Unique Id
    json_data["_id"] = _id;

    // Base entity features (private)
    json_data["_color"] = tcodcolor_to_json(_color);
    json_data["_bg_color"] = tcodcolor_to_json(_bg_color);

    json_data["_blocks"] = _blocks;
    json_data["_blocks_sight"] = _blocks_sight;

    json_data["_fixed"] = _fixed;

    json_data["_render_order"] = _render_order;

    // Base entity features (public)

    // Current coordinates in the map
    json_data["x"] = x;
    json_data["y"] = y;

    json_data["name"] = name;
    json_data["symbol"] = symbol;
    json_data["tag"] = tag;

    // Components
    if (fighter != nullptr)
        json_data["fighter"] = fighter->to_json();
    else
        json_data["fighter"] = nullptr;

    if (ai != nullptr)
        json_data["ai"] = ai->to_json();
    else
        json_data["ai"] = nullptr;

    if (stairs != nullptr)
        json_data["stairs"] = stairs->to_json();
    else
        json_data["stairs"] = nullptr;

    if (item != nullptr)
        json_data["item"] = item->to_json();
    else
        json_data["item"] = nullptr;

    if (level != nullptr)
        json_data["level"] = level->to_json();
    else
        json_data["level"] = nullptr;

    if (equipment != nullptr)
        json_data["equipment"] = equipment->to_json();
    else
        json_data["equipment"] = nullptr;

    if (equippable != nullptr)
        json_data["equippable"] = equippable->to_json();
    else
        json_data["equippable"] = nullptr;

    if (usable != nullptr)
        json_data["usable"] = usable->to_json();
    else
        json_data["usable"] = nullptr;

    if (inventory != nullptr)
        json_data["inventory"] = inventory->to_json();
    else
        json_data["inventory"] = nullptr;

    if (interactive != nullptr)
        json_data["interactive"] = interactive->to_json();
    else
        json_data["interactive"] = nullptr;

    // Buffs currently applied to entity
    json j_buffs;
    for (int i=0; i<(int)buffs.size(); i++)
    {
        j_buffs.push_back(buffs[i]->to_json());
    }

    json_data["buffs"] = j_buffs;

    return json_data;
};

Entity * Entity::from_json(json j)
{

    //DEBUG("Reconstructing entity");

    // First create base object
    Entity * e = new Entity(j["x"], j["y"], j["symbol"],
           json_to_tcodcolor(j["_color"]),  j["name"],
           j["_render_order"],
           j["_blocks"], j["_blocks_sight"],
           j["_fixed"],
           j["_id"]);

    //DEBUG("Restoring " << j["name"]);

    e->tag = j["tag"];

    // Then reconstruct and assign all components
    if (j["fighter"] != nullptr)
    {
        e->fighter = Fighter::from_json(j["fighter"]);
        e->fighter->owner = e;
    }
    
    if (j["ai"] != nullptr)
    {
        e->ai = MonsterAi::from_json(j["ai"]);
        e->ai->owner = e;
    }
    
    if (j["stairs"] != nullptr)
    {
        e->stairs = Stairs::from_json(j["stairs"]);
        e->stairs->owner = e;
    }
    
    if (j["item"] != nullptr)
    {
        e->item = Item::from_json(j["item"]);
        e->item->owner = e;
    }
    
    if (j["level"] != nullptr)
    {
        e->level = Level::from_json(j["level"]);
        e->level->owner = e;
    }
    
    if (j["equipment"] != nullptr)
    {
        e->equipment = Equipment::from_json(j["equipment"]);
        e->equipment->owner = e;
    }
    
    if (j["equippable"] != nullptr)
    {
        e->equippable = Equippable::from_json(j["equippable"]);
        e->equippable->owner = e;
    }
    
    if (j["usable"] != nullptr)
    {
        e->usable = Usable::from_json(j["usable"]);
        e->usable->owner = e;
    }
    
    if (j["inventory"] != nullptr)
    {
        e->inventory = Inventory::from_json(j["inventory"]);
        e->inventory->owner = e;
    }
    
    if (j["interactive"] != nullptr)
    {
        e->interactive = Interactive::from_json(j["interactive"]);
        e->interactive->owner = e;
    }
    
    return e;
}

/*


class Entity:
    """
    A generic object to represent players, enemies, items, etc.
    """

    def move(self, dx, dy):
        # Move the entity by a given amount
        self.x += dx
        self.y += dy

    def move_towards(self, target_x, target_y, game_map, entities):
        dx = target_x - self.x
        dy = target_y - self.y
        distance = math.sqrt(dx ** 2 + dy ** 2)

        dx = int(round(dx / distance))
        dy = int(round(dy / distance))

        if not (
            game_map.is_blocked(
                self.x +
                dx,
                self.y +
                dy) or get_blocking_entities_at_location(
                entities,
                self.x +
                dx,
                self.y +
                dy)):
            self.move(dx, dy)

    def move_astar(self, target, entities, game_map):
        # Create a FOV map that has the dimensions of the map
        fov = libtcod.map_new(game_map.width, game_map.height)

        # Scan the current map each turn and set all the walls as unwalkable
        for y1 in range(game_map.height):
            for x1 in range(game_map.width):
                libtcod.map_set_properties(
                    fov,
                    x1,
                    y1,
                    not game_map.tiles[x1][y1].block_sight,
                    not game_map.tiles[x1][y1].blocked)

        # Scan all the objects to see if there are objects that must be
        # navigated around. Check also that the object isn't self or the
        # target (so that the start and the end points are free).
        # The AI class handles the situation if self is next to the target so
        # it will not use this A* function anyway
        for entity in entities:
            if entity.blocks and entity != self and entity != target:
                # Set the tile as a wall so it must be navigated around
                libtcod.map_set_properties(
                    fov, entity.x, entity.y, True, False)

        # Allocate a A* path
        # The 1.41 is the normal diagonal cost of moving, it can be set as 0.0
        # if diagonal moves are prohibited
        my_path = libtcod.path_new_using_map(fov, 1.41)

        # Compute the path between self's coordinates and the target's
        # coordinates
        libtcod.path_compute(my_path, self.x, self.y, target.x, target.y)

        # Check if the path exists, and in this case, also if such path is
        # shorter than 25 tiles. The path size matters if you want the monster
        # to use alternative longer paths (for example through other rooms),
        # if for example the player is in a corridor.
        # It makes sense to keep path size relatively low to keep the monsters
        # from running around the map if there's an alternative path really far
        # away
        if not libtcod.path_is_empty(
                my_path) and libtcod.path_size(my_path) < 25:
            # Find the next coordinates in the computed full path
            x, y = libtcod.path_walk(my_path, True)
            if x or y:
                # Set self's coordinates to the next path tile
                self.x = x
                self.y = y
        else:
            # Keep the old move function as a backup so that if there are no
            # paths (for example another monster blocks a corridor), it will
            # still try to move towards the player (closer to the corridor
            # opening).
            self.move_towards(target.x, target.y, game_map, entities)

            # Delete the path to free memory
        libtcod.path_delete(my_path)

    def distance(self, x, y):
        return math.sqrt((x - self.x) ** 2 + (y - self.y) ** 2)

    def distance_to(self, other):
        dx = other.x - self.x
        dy = other.y - self.y
        return math.sqrt(dx ** 2 + dy ** 2)

*/
