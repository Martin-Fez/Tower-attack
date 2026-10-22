#ifndef ATTACKER_H
#define ATTACKER_H

#include <iostream>
#include "../Unit.hpp"
#include "../Maptiles.hpp"

using namespace std;

/**
 * @brief Tile representing the unit attacker
 * 
 */
class Cattacker : public CUnit
{
    public:
        Cattacker(char symbol, bool passable,int y, int x,string name, int HP, int damage,int speed,int cost, Cdamage_type resistance, 
            Cdamage_type dmg_type);

        Cattacker(const Cattacker & src);


        /**
         * @brief Function for finding the shortest path to the exit using the A* algorithm
         * 
         * Function uses the A* algorithm to determine the shortest path to the exit.
         * Each tile is visited and given a distance value of how long it takes to get from said tile to the end.
         * The tile's distance is artificialy inflated if it has a tower on it.
         * The algorithm returns the path from exit to unit so the function reverses it and marks each tile as being a path tile.
         * 
         * 
         * @param end_tile Tile to reach
         */
        void findPath(weak_ptr<CMapTiles> end_tile); 

        /**
         * @brief Moves the attacker speed amount of tiles
         * 
         * Attempts to move attacker up to the amount of tiles it's speed atribute allows it to.
         * If must always end on a unoccupied tile. If it runs into a tower, it attacks it.
         * If it runs into a another attacker and it is faster than it, it can pass this attacker. 
         * 
         * @param it iterator to the unit's pointer within the container it is located in
         */
        void move_foward(list<shared_ptr<Cattacker>>::iterator it);

        /**
         * @brief Function to get a unit's data to be saved
         * 
         * Fucntion which outputs all the neccesary for unit to be later loaded into a list of strings.
         * 
         * @param active boolean to determine whether the tile is active on the map or not
         * @return list<string> Data of the attacker
         */
        virtual list<string> SaveUnitData(bool active); 

        /**
         * @brief function for attacking a tower
         * 
         * Attacker attacks the tower and decreases it's hp in accordance to it's resistances and the attacker's dmg.
         * The target is the unit within's it's path
         * 
         */
        void attack_enemy();

        int c_speed;
        int c_cost;
        list<weak_ptr<CMapTiles>> c_pathToExit; 
        weak_ptr<CMapTiles> c_mapTile_ptr; 

};


#endif
