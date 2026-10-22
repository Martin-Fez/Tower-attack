#include "wall.hpp"


Cwall::Cwall(char symbol, bool passable,int y, int x) : CMapTiles(symbol,passable,y,x) {}


void Cwall::getPosValue(int & AOE, int & ST, int & BLOCK)
{
    AOE =  numeric_limits<int>::max();
    ST = numeric_limits<int>::max();
    BLOCK = numeric_limits<int>::max();

    return;
}

