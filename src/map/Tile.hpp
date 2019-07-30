#ifndef ROGUE_20177_TILE
#define ROGUE_20177_TILE

/*

import random

import libtcodpy as libtcod

GRAY_PALETTE = [
    # libtcod.Color(242, 242, 242),
    libtcod.Color(204, 204, 204),
    libtcod.Color(165, 165, 165),
    libtcod.Color(127, 127, 127),
    libtcod.Color(89, 89, 89),
]

*/

#include <vector>

#include "../libtcod.hpp"

#include "../nlohmann/json.hpp"

using json = nlohmann::json;

json tcodcolor_to_json(TCODColor c);

/**
 * A tile on a map. It may or may not be blocked, and may or may not block
 * sight.
 */
class Tile
{

    protected:

        bool _blocked;
        bool _block_sight;
        bool _explored;

        TCODColor _bg_color;
        TCODColor _fg_color;

        // TODO check attribute name
        int _fg_symbol;

    public:

        Tile(bool blocked, bool block_sight);
        virtual ~Tile();

        // Private member getters
        virtual bool blocked() const;
        virtual bool block_sight() const;
        virtual bool explored() const;
        virtual int fg_symbol() const;
        
        // Set value
        void explored(bool v);

        TCODColor bg_color() const;
        TCODColor fg_color() const;

        virtual void render_at(TCODConsole * con, int x, int y, bool visible);
        virtual json to_json();
};

/**
 * A block representing traversable terrain
 */
class Floor : public Tile
{

    public:

        Floor(TCODColor bg_color = TCODColor(20, 20, 20),
              TCODColor fg_color = TCODColor(65, 65, 65),
              int fg_symbol=250);

};

/**
 * A block of wall
 */
class Wall : public Tile
{

    public:

        Wall(TCODColor bg_color,
              TCODColor fg_color=TCODColor::black,
              int fg_symbol='#');

        /**
         * Create a wall tile choosing the background color from a
         */
        static Wall * create_from_palette(std::vector<TCODColor> palette);

};

/*

class Door(Tile):
    """
    A door
    """

    def __init__(self, bg_color=libtcod.Color(139,69,19), 
                 fg_color=libtcod.orange, is_open=False):

        # Declare it as blocked
        super().__init__(False)

        self.bg_color = bg_color
        self.fg_color = fg_color

        self.is_open = is_open

    def open(self):
        self.is_open = True

    def close(self):
        self.is_open = False

    @property
    def fg_symbol(self):
        """
        Return a different symbol based on status
        """
        if self.is_open:
            return '-'
        else:
            return '+'

    @property
    def block_sight(self):
        return not self.is_open

*/

#endif /* ifndef ROGUE_20177_TILE */
