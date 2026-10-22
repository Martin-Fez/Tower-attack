#ifndef EMPTY_SPACE_H
#define EMPTY_SPACE_H

#include <iostream>
#include "../Maptiles.hpp"

using namespace std;

/**
 * @brief tile representing an empty space
 * 
 */
class Cempty_space : public CMapTiles
{
    public:

        Cempty_space(char symbol, bool passable,int y, int x);

        /**
         * @brief Function for getting an empty space's value
         * 
         * Function for determining tile's value for placement of a tower with lower being more valueable.
         * 
         * ST - It is lowered by how many paths and unit it has in range and increased
         * if it is in range of a another tower or on the attacker's path
         * 
         * AOE - It is decreased by by the same values as ST but it has only the reach of it's 
         * neighbours
         * 
         * BLOCK - Block is decreased if it has another tower or wall next it whether horizontaly or verticaly.
         * It further decreses if on unit's path.
         * 
         * @param AOE Value for an AOE tower
         * @param ST Value for an ST tower
         * @param BLOCK Value for a obstacle tower
         */
        void getPosValue(int & AOE, int & ST, int & BLOCK);   
};


#endif
