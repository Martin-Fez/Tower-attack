#ifndef TOWERSINGLETG_H
#define TOWERSINGLETG_H

#include <iostream>
#include "Tower.hpp"

using namespace std;

/**
 * @brief Tile for towers that target a single attacker
 * 
 */
class CTowerSingleTG : public Ctower
{
    public:

    CTowerSingleTG(char symbol, bool passable,int y, int x, string name, int HP,int damage, Cdamage_type resistance, Cdamage_type dmg_type,
        int cooldown, int timer,string type, int reach);

    CTowerSingleTG(const Ctower & src);

    /**
     * @brief Attacks a single enemy
     * 
     * Attacks a single enemy within it's designated reach
     * HP of attackers is reduced by the amount of dmg ST tower has and whether they resitant or not.
     * 
     */
    void attack_enemy();
};

#endif
