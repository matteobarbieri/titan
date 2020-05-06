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
#include "components/Container.hpp"

// Misc
#include "buffs/Buff.hpp"

Entity::Entity(int x, int y, int symbol,
    TCODColor color, std::string name, RenderOrder render_order,
    bool blocks, bool blocks_sight, bool _fixed,
    unsigned long int id, unsigned long int group_id) :
    x(x), y(y), _render_order(render_order), name(name),
    symbol(symbol), _color(color),
    _blocks(blocks), _blocks_sight(blocks_sight),
    _fixed(_fixed), tag(""),
    group_id(group_id)
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
        container = nullptr;

        // If parameter id is default, initialize it with a progressive one
        if (id == 0)
            _id = Uid::singleton().next_id();
        else
            _id = id;

        // No need to assign a unique group ID if not needed
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
    buff->target = this;
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
        fighter->attack(other, game_map);
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

    // Group id
    json_data["group_id"] = group_id;

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

    if (container != nullptr)
        json_data["container"] = container->to_json();
    else
        json_data["container"] = nullptr;

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
           j["_id"],
           j["group_id"]);

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
    
    if (j["container"] != nullptr)
    {
        e->container = Container::from_json(j["container"]);
        e->container->owner = e;
    }

    // Buffs
    if (j["buffs"] != nullptr)
    {
        // Reapply buffs "manually"
        for (int i=0; i<(int)j["buffs"].size(); i++)
        {
            Buff * b = Buff::from_json(j["buffs"][i]);
            e->buffs.push_back(b);
            b->target = e;
        }
    }

    return e;
}
