#include "attacker.hpp"

Cattacker::Cattacker(char symbol, bool passable,int y, int x, string name, int HP,int damage,int speed,int cost,
                     Cdamage_type resistance, Cdamage_type dmg_type) : CUnit(symbol,passable,y,x,name,HP,damage,resistance,dmg_type),
                         c_speed(speed), c_cost(cost) {}

Cattacker::Cattacker(const Cattacker & src) : CUnit(src) 
{
    c_cost = src.c_cost;
    c_speed = src.c_speed;
}

void Cattacker::findPath(weak_ptr<CMapTiles> end_tile)
{

    c_pathToExit.clear();

    auto distance = [](weak_ptr<CMapTiles> a, weak_ptr<CMapTiles> b)
    {
        return sqrtf((a.lock()->c_x - b.lock()->c_x)*(a.lock()->c_x - b.lock()->c_x) + (a.lock()->c_y - b.lock()->c_y)*(a.lock()->c_y - b.lock()->c_y));
    };


    weak_ptr<CMapTiles> nodeCurrent = c_mapTile_ptr; 
    c_mapTile_ptr.lock()->c_fLocalGoal = 0.0f;
    c_mapTile_ptr.lock()->c_fGlobalGoal = distance(c_mapTile_ptr,end_tile);


    list<weak_ptr<CMapTiles>> listTileNotTested;
    listTileNotTested.push_back(c_mapTile_ptr);
    float fPossibleLowerGoal;


    while (!listTileNotTested.empty() && nodeCurrent.lock() != end_tile.lock())
    {
        listTileNotTested.sort([](const weak_ptr<CMapTiles> lhs, weak_ptr<CMapTiles> rhs){return lhs.lock()->c_fGlobalGoal < rhs.lock()->c_fGlobalGoal; } ) ;

        while(!listTileNotTested.empty() && listTileNotTested.front().lock()->c_path_visited)
            listTileNotTested.pop_front();

        if(listTileNotTested.empty())
            break;

        nodeCurrent = listTileNotTested.front();
        nodeCurrent.lock()->c_path_visited = true;

        for (auto it : nodeCurrent.lock()->c_Neighbours)
        {

            if(!it.lock()->c_path_visited && it.lock()->c_passable)
                listTileNotTested.push_back(it);
            
            if(it.lock()->Occupied_by_tower)
                fPossibleLowerGoal = nodeCurrent.lock()->c_fLocalGoal + distance(nodeCurrent,it) + 3 ;
            else
                fPossibleLowerGoal = nodeCurrent.lock()->c_fLocalGoal + distance(nodeCurrent,it);


            if(fPossibleLowerGoal < it.lock()->c_fLocalGoal)
            {
                it.lock()->c_parent = nodeCurrent;
                it.lock()->c_fLocalGoal = fPossibleLowerGoal;


                it.lock()->c_fGlobalGoal = it.lock()->c_fLocalGoal + distance(it,end_tile);
            }

        }
    }

    weak_ptr<CMapTiles> path_tile = end_tile;
    if (path_tile.lock()->c_parent.expired() )
    {
        return;
    }

    while(path_tile.lock() != c_mapTile_ptr.lock())
    {
        c_pathToExit.push_front(path_tile);
        path_tile.lock()->num_of_paths++;
        path_tile = path_tile.lock()->c_parent;
    }

}

void Cattacker::move_foward(list<shared_ptr<Cattacker>>::iterator it)
{

        c_mapTile_ptr.lock()->Occupied_by_unit = false;

        if(c_pathToExit.empty())
            return;

        auto nextTile = c_pathToExit.begin();

        if(nextTile->lock()->Occupied_by_tower)
        {
            attack_enemy();
            c_mapTile_ptr.lock()->Occupied_by_unit = true;
            return;
        }

        int i = 1;
        for(; i < c_speed; i++)
        {
            nextTile++;
            if(nextTile == c_pathToExit.end() || nextTile->lock()->Occupied_by_tower)
            {
                nextTile--;
                break;
            }
        }

        for(; i > 0;i--)
        {
            if(nextTile->lock()->c_unitTile_ptr.expired())
                break;

            if(nextTile == c_pathToExit.begin())
                return;
            
            nextTile--;
        }
        
        nextTile->lock()->c_unitTile_ptr= (*it); 

        c_mapTile_ptr.lock()->c_unitTile_ptr.reset(); 
        c_mapTile_ptr = *nextTile; 

        for(;i > 0; i--)
            c_pathToExit.pop_front();

        c_mapTile_ptr.lock()->Occupied_by_unit = true;

}


list<string> Cattacker::SaveUnitData(bool active)
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
    line = "COST: " + to_string(c_cost);
    DATA.push_back(line);
    line = "SPEED: " + to_string(c_speed);
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

void Cattacker::attack_enemy()
{
    auto target = c_pathToExit.begin()->lock()->c_unitTile_ptr;

    if(c_dmg_type == target.lock()->c_resistance)
        target.lock()->c_HP -= (c_damage/2);
    else    
       target.lock()->c_HP -= c_damage; 

    cout << target.lock()->c_HP << endl;
    if(target.lock()->c_HP <= 0)
    {
        c_pathToExit.begin()->lock()->Occupied_by_tower = false;
    }
}
