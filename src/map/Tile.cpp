#include "Tile.hpp"

#include "../libtcod.hpp"

/////////////////////////////////
///////////// TILE //////////////
/////////////////////////////////

Tile::Tile(bool blocked, bool block_sight) : _blocked(blocked), _block_sight(block_sight)
{
}

Tile::~Tile()
{
}

bool Tile::blocked()
{
    return _blocked;
}

bool Tile::block_sight()
{
    return _block_sight;
}

void Tile::render_at(TCODConsole * con, int x, int y, bool visible)
{
}

bool Tile::explored()
{
    return _explored;
}

void Tile::explored(bool v)
{
    _explored = v;
}

/////////////////////////////////
///////////// FLOOR /////////////
/////////////////////////////////


/*

   // TODO add the remaining code!
    def __init__(self, bg_color=libtcod.Color(20, 20, 20), fg_symbol=250,
                 alternate_fg_symbols=['[', ']', '{', '}', '*', '%'],
                 alternate_symbol_chance=0.1,
                 # fg_color=libtcod.Color(70, 70, 70)):
                 fg_color=libtcod.Color(65, 65, 65)):

        # self.bg_color = libtcod.black
        # self.bg_color = libtcod.Color(10, 10, 10)
        # self.bg_color = libtcod.Color(32, 32, 32)
        # self.bg_color = libtcod.Color(16, 16, 16)
        self.bg_color = bg_color
        self.fg_color = fg_color

        # Choose one of the available symbols every once in a while
        if random.random() < alternate_symbol_chance:
            # The alternate symbol
            self._fg_symbol = random.choice(alternate_fg_symbols)
        else:
            # The default symbol
            self._fg_symbol = fg_symbol




*/

Floor::Floor(TCODColor bg_color, TCODColor fg_color, int fg_symbol) : 
    Tile(false, false) 
{

    _bg_color = bg_color;
    _fg_color = fg_color;

    _fg_symbol = fg_symbol;

}



/////////////////////////////////
///////////// WALL //////////////
/////////////////////////////////

/*

   def __init__(self, bg_color, fg_symbol='#', fg_color=libtcod.black):

        # Declare it as blocked
        super().__init__(True)

        self.bg_color = bg_color
        self.fg_color = fg_color
        self._fg_symbol = fg_symbol

*/

Wall::Wall(TCODColor bg_color, TCODColor fg_color, int fg_symbol) : Tile(true, true)
{

    _bg_color = bg_color;
    _fg_color = fg_color;

    _fg_symbol = fg_symbol;
}

Wall * Wall::create_from_palette(std::vector<TCODColor> palette)
{

    // TODO choose a random bg color from palette
    
    Wall * wall = new Wall(palette[0]);

    return wall;
}

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


class Tile:
    """
    A tile on a map. It may or may not be blocked, and may or may not block
    sight.
    """

    def __init__(self, blocked, block_sight=None):

        self._blocked = blocked

        # By default, if a tile is blocked, it also blocks sight
        if block_sight is None:
            block_sight = blocked

        self._block_sight = block_sight

        self._fg_symbol = ' '
        self.explored = False

    @property
    def fg_symbol(self):
        return self._fg_symbol

    @property
    def blocked(self):
        return self._blocked

    @property
    def block_sight(self):
        return self._block_sight

    def render_at(self, con, x, y, visible):
        """
        Render a tile at position x, y
        """
        # Set color for background

        if type(self) == Tile:
            return

        libtcod.console_set_char_background(
            con, x, y, self.bg_color, libtcod.BKGND_SET)

        if self.fg_symbol is not None:

            # Draw symbol on foreground
            libtcod.console_put_char(
                con, x, y, self.fg_symbol, libtcod.BKGND_NONE)

            # Set color for foreground symbol
            libtcod.console_set_char_foreground(con, x, y, self.fg_color)


class Floor(Tile):
    """
    A block representing traversable terrain
    """


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


class Wall(Tile):
    """
    A block of wall
    """


    def create_from_palette(palette=GRAY_PALETTE):
        """
        palette: list
            Each element is a libtcod.Color object
        """

        return Wall(random.choice(palette))

#     def create(base_color=libtcod.Color(159, 89, 66), color_variance=20):

        # # Extract colors
        # b, g, r = base_color.b, base_color.g, base_color.r

        # # Slightly alter them
        # b += random.randint(-color_variance, color_variance)
        # b = max(0, b)
        # b = min(255, b)

        # g += random.randint(-color_variance, color_variance)
        # g = max(0, g)
        # g = min(255, g)

        # r += random.randint(-color_variance, color_variance)
        # r = max(0, r)
        # r = min(255, r)

        # return Wall(libtcod.Color(b, g, r))
*/
