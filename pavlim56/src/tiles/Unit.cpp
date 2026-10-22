#include "Unit.hpp"

CUnit::CUnit(const CUnit & src) : Ctiles(src)
{
    c_tileName = src.c_tileName;
    c_HP = src.c_HP;
    c_damage = src.c_damage;
    c_resistance = src.c_resistance;
    c_dmg_type = src.c_dmg_type;
}

CUnit::CUnit(char symbol, bool passable,int y, int x,string name, int HP, int damage, Cdamage_type resistance, Cdamage_type dmg_type) : Ctiles(symbol,passable,y,x),
                c_tileName(name), c_HP(HP), c_damage(damage) , c_resistance(resistance), c_dmg_type(dmg_type)
                {}