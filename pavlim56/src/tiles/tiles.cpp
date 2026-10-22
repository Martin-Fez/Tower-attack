#include "tiles.hpp"

Ctiles::Ctiles(char symbol, bool passable,int y, int x) : c_symbol(symbol), c_passable(passable), c_y(y), c_x(x) 
{
}

Ctiles::Ctiles(const Ctiles & src) 
{
    c_symbol = src.c_symbol;
    c_x = src.c_x;
    c_y = src.c_y;
}
