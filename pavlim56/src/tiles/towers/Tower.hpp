#ifndef TOWER_H
#define TOWER_H

#include <iostream>
#include "../Maptiles.hpp"
#include "../Unit.hpp"

using namespace std;

/**
 * @brief parent class for all tower types
 * 
 */
class Ctower : public CUnit
{
    public:
        /**
         * @brief Construct a new Ctower object
         * 
         * determines c_money_worth and c_score_worth though the value of the tower's health.
         * 
         */
        Ctower(char symbol, bool passable,int y, int x, string name, int HP,int damage, Cdamage_type resistance, Cdamage_type dmg_type,
             int cooldown, int timer,string type, int reach);

        /**
         * @brief Construct a new Ctower object
         * 
         * determines c_money_worth and c_score_worth though the value of the tower's health.
         * 
         */
        Ctower(const Ctower & src);

        vector <weak_ptr<CMapTiles>> c_reachVector;	
        weak_ptr<CMapTiles> c_mapTile_ptr; 
        int c_cooldown;
        int c_score_worth;
        int c_money_worth;
        int c_timer; 
        string c_type;
        int c_reach;

        /**
         * @brief Gets all reachable tiles
         * 
         * Function goes though it's neighbours and their neighbours and adds them
         * to the c_reachVector if those tiles are still within the tower's reach.
         * 
         */
        void get_reachable_tiles();

        
        /**
         * @brief outputs tower's data to save
         * 
         * Outputs all tower's data necessary to load it up later.
         * 
         * @param active Tower is active within the map
         * @return list<string> Outputed data of the tower
         */
        virtual list<string> SaveUnitData(bool active);

        /**
         * @brief Pure virtual function for attacking attackers
         * 
         */
        virtual void attack_enemy() = 0;

    protected:

};


#endif
