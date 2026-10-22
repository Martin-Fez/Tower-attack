#include "Maptiles.hpp"

CMapTiles::CMapTiles(const CMapTiles & src) : Ctiles(src)
{
    num_of_paths = src.num_of_paths;
    in_range = src.in_range;
}

CMapTiles::CMapTiles(char symbol, bool passable,int y, int x) : Ctiles(symbol,passable,y,x)
{
    num_of_paths = 0;
    in_range = false;
}
