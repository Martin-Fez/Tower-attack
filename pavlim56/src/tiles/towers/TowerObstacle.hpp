#ifndef TOWEROBSTACLE_H
#define TOWEROBSTACLE_H

#include <iostream>
#include "Tower.hpp"

using namespace std;

/**
 * @brief Tile meant to function as a obstacle for attackers
 * 
 */
class CTowerObstacle : public Ctower
{
    public:

    CTowerObstacle(char symbol, bool passable,int y, int x, string name, int HP,int damage, Cdamage_type resistance, Cdamage_type dmg_type,
        int cooldown, int timer,string type, int reach);

    CTowerObstacle(const Ctower & src);

    /**
     * @brief Does not attack
     * 
     * Obstacle tower only blocks and cannot attack
     * 
     */
    void attack_enemy();
};

#endif
