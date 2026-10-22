#include "empty_space.hpp"

Cempty_space::Cempty_space(char symbol, bool passable,int y, int x) : CMapTiles(symbol,passable,y,x) {}

void Cempty_space::getPosValue(int & AOE, int & ST, int & BLOCK)
{
    AOE = 0;
    ST = 0;
    BLOCK = 0;

    for(auto & it : c_range_access)
    {
        if(it.lock()->num_of_paths > 0)
            ST-=2;

        if(it.lock()->Occupied_by_unit) 
            ST-=2;

        if(it.lock()->in_range)
            ST+=1;
    }

    for(auto & it : c_Neighbours)
    {
        if(it.lock()->num_of_paths > 0)
        {
            AOE-=2;
        }

        if(it.lock()->Occupied_by_unit) 
        {
            AOE-=2;  
        }

        if(it.lock()->in_range)
            AOE+=1;
    }

    bool next_to_wall = false;
    int xValue = 0;
    int yValue = 0;
    for(auto & it : c_range_access)
    {
        if(!it.lock()->c_passable && (it.lock()->c_y == c_y || it.lock()->c_x == c_x ))
        {
            next_to_wall = true;
            if(it.lock()->c_x == c_x)
            {
                xValue -=1;
                xValue *=4;
            }

            if(it.lock()->c_y == c_y)
            {
                yValue -=1;
                yValue *=4;
            }

            if(it.lock()->Occupied_by_tower)
            {
                BLOCK-=1;
            }
            
        }

        if(!it.lock()->c_passable && ( it.lock()->c_y != c_y && it.lock()->c_x != c_x ))
        {
            BLOCK+=5;
        }

    }

    if(!next_to_wall && num_of_paths)
    {
        BLOCK-=2;
        ST+=5;
        AOE+=3;
    }


    if(xValue < yValue)
    {
        BLOCK += xValue;
    }
    else
    {
        BLOCK += yValue;
    }

    return;

}
