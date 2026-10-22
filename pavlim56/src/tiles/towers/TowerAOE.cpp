#include "TowerAOE.hpp"

CTowerAOE::CTowerAOE(char symbol, bool passable,int y, int x,string name, int HP, int damage, Cdamage_type resistance, Cdamage_type dmg_type,
         int cooldown, int timer,string type, int reach) 
: Ctower(symbol, passable, y, x, name, HP, damage, resistance, dmg_type, cooldown, timer,type,reach) {}

CTowerAOE::CTowerAOE(const Ctower & src) : Ctower(src) {}

void CTowerAOE::attack_enemy()
{

    for(auto & target : c_reachVector) 
    {

        if(target.lock()->Occupied_by_unit)
        {
            if(c_dmg_type == target.lock()->c_unitTile_ptr.lock()->c_resistance)
                target.lock()->c_unitTile_ptr.lock()->c_HP -= c_damage/2;
            else
                target.lock()->c_unitTile_ptr.lock()->c_HP -= c_damage;

            if(target.lock()->c_unitTile_ptr.lock()->c_HP < 0)
            {
                target.lock()->Occupied_by_unit = false;
            }
        }
    }

}