#ifndef MAPTILES_H
#define MAPTILES_H

#include <iostream>
#include "tiles.hpp"
#include "unit.hpp"

using namespace std;

/**
 * @brief Parent class for all map tiles
 * 
 */
class CMapTiles : public Ctiles
{
    public:
            CMapTiles(char symbol, bool passable, int y, int x);

            CMapTiles(const CMapTiles & src);
            
            bool c_accessed = false;
            bool c_path_visited = false;
            float c_fGlobalGoal;
            float c_fLocalGoal;
            weak_ptr<CMapTiles> c_parent; 


            int num_of_paths;
            int c_range_distance;
            bool in_range = false;

            vector <weak_ptr<CMapTiles>> c_Neighbours;
            vector <weak_ptr<CMapTiles>> c_range_access;

            weak_ptr<CUnit> c_unitTile_ptr;

            bool Occupied_by_unit = false;
            bool Occupied_by_tower = false;

            int num_of_paths_in_range;

            /**
             * @brief Pure virtual function for getting value position
             * 
             * @param AOE Value of AOE towers
             * @param ST Value of ST towers
             * @param BLOCK Value of obstacle towers
             * @return int 
             */
            virtual void getPosValue(int & AOE, int & ST, int & BLOCK) = 0;
};



#endif
