#include "Tower.hpp"

Ctower::Ctower(char symbol, bool passable,int y, int x,string name, int HP, int damage, Cdamage_type resistance, Cdamage_type dmg_type
            , int cooldown, int timer,string type, int reach) 
                : CUnit(symbol,passable,y,x,name,HP,damage,resistance,dmg_type) , c_cooldown(cooldown), c_timer(timer), c_type(type), c_reach(reach)
                {
                    c_score_worth = HP*10;
                    c_money_worth = HP;
                }

Ctower::Ctower(const Ctower & src) : CUnit(src)
{
    c_cooldown = src.c_cooldown;
    c_timer = src.c_timer; 
    c_score_worth = src.c_HP*10;
    c_money_worth = src.c_HP;
    c_type = src.c_type;
    c_reach = src.c_reach;
}

list<string> Ctower::SaveUnitData(bool active)
{
   list <string> DATA;
   string line;

    line = "NAME: " + c_tileName;
    DATA.push_back(line);
    line = "SYMBOL: " + string(1,c_symbol);
    DATA.push_back(line);
    line = "HEALTH: " + to_string(c_HP);
    DATA.push_back(line);
    line = "DAMAGE: " + to_string(c_damage);
    DATA.push_back(line);
    line = "COOLDOWN: " + to_string(c_cooldown);
    DATA.push_back(line);
    line = "TIMER: " + to_string(c_timer);
    DATA.push_back(line);
    line = "TYPE: " + c_type;
    DATA.push_back(line);
    line = "DISTANCE: " +to_string(c_reach);
    DATA.push_back(line);

    for(auto & name : c_resistance.c_names)
    {
        line = "RESISTANCE: " + name;
        DATA.push_back(line);
    }

    for(auto & name : c_dmg_type.c_names)
    {
        line = "DAMAGE_TYPE: " + name;
        DATA.push_back(line);
    }

    if(active)
    {
        line = "X: " + to_string(c_mapTile_ptr.lock()->c_x);
        DATA.push_back(line);
        line = "Y: " + to_string(c_mapTile_ptr.lock()->c_y);
        DATA.push_back(line);
    }

    line = " ";
    DATA.push_back(line);

    return DATA;
}

void Ctower::get_reachable_tiles()
{
    queue<weak_ptr<CMapTiles>> accessed_tiles;
    c_mapTile_ptr.lock()->c_range_distance = 0;
    accessed_tiles.push(c_mapTile_ptr);
    while(!accessed_tiles.empty())
    {
        auto current = accessed_tiles.front();
        current.lock()->in_range = true;
        for(auto neigbour : current.lock()->c_Neighbours)
        {
            if(neigbour.lock()->c_range_distance > current.lock()->c_range_distance)
            {
                neigbour.lock()->c_range_distance = current.lock()->c_range_distance+1;
            }

            if(neigbour.lock()->c_range_distance <= c_reach && !neigbour.lock()->c_path_visited)
            {
                accessed_tiles.push(neigbour);
                c_reachVector.push_back(neigbour);
                neigbour.lock()->c_path_visited = true;
            }

        }
        accessed_tiles.pop();
    }
    
    
}