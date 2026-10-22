#ifndef TOWERAOE_H
#define TOWERAOE_H

#include <iostream>
#include "Tower.hpp"

using namespace std;

/**
 * @brief Tile for towers that deal damage in area of effect
 * 
 */
class CTowerAOE : public Ctower
{
    public:

    CTowerAOE(char symbol, bool passable,int y, int x, string name, int HP,int damage, Cdamage_type resistance, Cdamage_type dmg_type,
        int cooldown, int timer,string type, int reach);

    CTowerAOE(const Ctower & src);

    /**
     * @brief attacks attackers
     * 
     * Attacks all all attackers within it's immediate distance.
     * HP of attackers is reduced by the amount of dmg AOE tower has and whether they resitant or not
     * 
     */
    void attack_enemy();
};


#endif
