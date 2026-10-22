#ifndef WALL_H
#define WALL_H

#include <iostream>
#include "../Maptiles.hpp"

using namespace std;

/**
 * @brief Tile representing a wall
 * 
 */
class Cwall : public CMapTiles
{
    public:
        Cwall(char symbol, bool passable,int y, int x);

        /**
         * @brief Returns the placement value for towers
         * 
         * Due to being a wall all the returned values are the highest maximum possible
         * 
         * @param AOE 
         * @param ST 
         * @param BLOCK 
         */
        void getPosValue(int & AOE, int & ST, int & BLOCK);   
};



#endif
