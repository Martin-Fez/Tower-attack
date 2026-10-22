#ifndef CTILES_H
#define CTILES_H

#include <vector>
#include <iostream>
#include <memory>
#include <list>
#include <algorithm>
#include <string>
#include <math.h>
#include <limits>
#include <queue>




#include "atributes.hpp"

using namespace std;

/**
 * @brief Parent class for tiles
 * 
 */
class Ctiles
{
    public:
        Ctiles(char symbol, bool passable, int y, int x);
        virtual ~Ctiles() = default;

        Ctiles(const Ctiles & src);


        bool c_mapspace = true; 

        char c_symbol;
        bool c_passable;
        int c_y;
        int c_x;

};



#endif
