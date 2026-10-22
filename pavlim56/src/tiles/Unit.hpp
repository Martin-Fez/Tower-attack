#ifndef UNITS_H
#define UNITS_H

#include <iostream>
#include "tiles.hpp"

using namespace std;

/**
 * @brief Parent class for all units
 * 
 * Parent class for towers and attackers
 * 
 */
class CUnit : public Ctiles 
{
    public:
        CUnit(char symbol, bool passable,int y, int x, string name, int HP,int damage, Cdamage_type resistance, Cdamage_type dmg_type);

        CUnit(const CUnit & src);

        string c_tileName;
        int c_HP;
        int c_damage;
        Cdamage_type c_resistance;
        Cdamage_type c_dmg_type;


        /**
         * @brief Pure virtual function for SaveUnitData
         * 
         * @param active Whether we are saving a active unit or non-active
         * @return list<string> Data to be returned
         */
        virtual list<string> SaveUnitData(bool active) = 0;

};



#endif
