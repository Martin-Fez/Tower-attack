#include "TowerObstacle.hpp"

CTowerObstacle::CTowerObstacle(char symbol, bool passable,int y, int x,string name, int HP, int damage, Cdamage_type resistance, Cdamage_type dmg_type,
             int cooldown, int timer,string type,int reach) 
: Ctower(symbol, passable, y, x, name, HP, damage, resistance, dmg_type, cooldown, timer,type,reach) {}

CTowerObstacle::CTowerObstacle(const Ctower & src) : Ctower(src) {}


void CTowerObstacle::attack_enemy()
{
    return;
}