#include "Tile.hpp"

#include "../libtcod.hpp"

/////////////////////////////////
///////////// TILE //////////////
/////////////////////////////////


Tile::Tile(bool blocked, bool block_sight) :
    _blocked(blocked), _block_sight(block_sight),
    _explored(false), _fg_symbol(' ')
{
}

Tile::~Tile()
{
}

bool Tile::blocked() const
{
    return _blocked;
}

bool Tile::block_sight() const
{
    return _block_sight;
}

bool Tile::explored() const
{
    return _explored;
}

int Tile::fg_symbol() const
{
    return _fg_symbol;
}

TCODColor Tile::fg_color() const
{
    return _fg_color;
}

TCODColor Tile::bg_color() const
{
    return _bg_color;
}

void Tile::render_at(TCODConsole * con, int x, int y, bool visible)
{

    // TODO use also 'visible' argument
    
    con->setCharBackground(x, y, bg_color());
    con->putChar(x, y, fg_symbol(), TCOD_BKGND_NONE);
    con->setCharForeground(x, y, fg_color());
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
