#include "gameMap.hpp"


bool CgameMap::readMap(list<string> & mapFolder)
{
    clearMap();
    if(!readStatus(mapFolder))
    {
        cout << "Missing map data" << endl;
        return false;
    }

    for(auto & it : mapFolder)
    {
        if(it.length() > c_map_width)
            c_map_width = it.length();
    }

    for(auto & it : mapFolder) 
    {
        while(it.length() < c_map_width)
            it.push_back(' ');
    }

    string leftOvers;

    vector<shared_ptr<CMapTiles> > tileLine;
    set<char> entrances;
    int i = -1;
    int j = -1;

    for(auto & line : mapFolder)
    {
        i++;
        j = -1;

        if(c_loadingSave && readAtributeType(line) == "END_OF_MAP" )
        {
            if(!readProperty(line,"END_OF_MAP",leftOvers) || !leftOvers.empty()) 
            {
                cout << "wrong syntax of end of map data" << endl;
                return false;
            }
            for(int k = 0; k < i;k++)
            {
                mapFolder.pop_front();
            }
            mapFolder.pop_front(); 
            break; 
        }
        
        for(auto it = line.begin(); it != line.end(); it++)
        {
            j++;

            if(*it == ' ') 
            {
                tileLine.push_back( shared_ptr<CMapTiles>( new Cempty_space(' ',true,i,j) )  );
                continue;
            }

            if(*it == '#')
            {
                tileLine.push_back(shared_ptr<CMapTiles>( new Cwall(*it,false,i,j) ) );
                continue;
            }

            if(*it == '<' || *it == '=' || ( isdigit(*it) && *it != '0' ))
            {

                if( (*it == '<' && ( it+1 == line.end() || *(it+1) != '=') ) 
                    || ( *it == '=' && (line.begin() == it || *(it-1) != '<' ) )
                    || ( isdigit(*it) && ( line.begin() == it || line.begin() == it-1 || *(it-2) != '<' || *(it-1) != '=' )) )
                {
                    cout << "Incorect exit/entrace" << endl;
                    clearMap();
                    return false;
                }

                if(isdigit(*it) && entrances.find(*it) != entrances.end())
                {
                    cout << "duplicate entrances" << endl;
                    clearMap();
                    return false;
                }

                if(isdigit(*it))
                    entrances.emplace(*it);
                

                if( (*it == '<' && it+1 != line.end() && it+2 != line.end() &&  isdigit(*(it+2)) && *(it+2) != '0' )  
                    || ( *it == '=' && it+1 != line.end() &&  isdigit(*(it+1)) && *(it+1) != '0' ) 
                    || isdigit(*it) )
                {
                    tileLine.push_back(shared_ptr<CMapTiles>( new Cwall(*it,false,i,j) ) );
                    if(*it=='<')
                        c_entraceCords.emplace(line[j+2],make_pair(i,j));
                }
                else
                {
                    if(*it=='=')
                    {
                        c_exitCords.emplace(i,j);
                        tileLine.push_back(shared_ptr<CMapTiles>( new Cempty_space(*it,true,i,j) ) );
                    }
                    else
                        tileLine.push_back(shared_ptr<CMapTiles>( new Cwall(*it,false,i,j) ) );
                }


                continue;
            }

            cout << "tile not avaiable: " << *it << endl;
            clearMap();
            return false;

        }
        c_gameMap.push_back( move(tileLine) );
        tileLine.clear();
    }

    int k = 0;
    for(auto & it : entrances)
    {
        if(++k != it-48)
        {
            cout << "wrong entrance sequence" << endl;
            clearMap();
            return false;
        }
    }

    if(c_exitCords.size() != 1)
    {
        cout << "Missing or too many exits" << endl;
        clearMap();
        return false;
    }


    if(!checkAccess() || entrances.empty())
        return false; 

    for(auto & line : c_gameMap)
    {
        for(auto & it : line)
        {
            if(it->c_symbol == ' ' && it->c_accessed)
                c_empty_spaces.push_back(it);
        }
    }


    for(auto & space : c_empty_spaces)
    {
        for (int i = -2; i < 3; i++) 
        {
            for (int j = -2; j < 3; j++) 
            {
                if(!(i == 0 && j == 0) && !(space.lock()->c_y+i < 0  || space.lock()->c_x+j < 0
                || space.lock()->c_y+i >= (int)(c_gameMap.size()-1) || space.lock()->c_x+j >= (int)(c_map_width-1)) 
                && c_gameMap[space.lock()->c_y+i][space.lock()->c_x+j]->c_accessed)
                {
                    space.lock()->c_range_access.push_back( c_gameMap[space.lock()->c_y+i][space.lock()->c_x+j] );
                }
            }
        }

    }

    
    c_number_of_entraces = c_entraceCords.size();



    return true;


}



void CgameMap::display_map() const  
{
    cout << setfill('~') << setw(80) << "~" << endl;
    cout << "HP REMANING: "  << setfill(' ') << setw(54) << left << c_EnemyHp   << "MONEY:  " << c_money << endl;
    cout << "ROUND: " << c_round << "/" << setw(57) << left << c_MaxRounds   << "INCOME: "<< c_passive_income << endl;
    cout << "SCORE: " <<  c_score << endl;
    cout << setfill('~') << setw(80) << "~" << endl;

    for(auto & line : c_gameMap)
        {
            for(auto & it : line)
            {
                if(!it->c_unitTile_ptr.expired())
                    cout << it->c_unitTile_ptr.lock()->c_symbol;
                else
                    cout << it->c_symbol;
            }
            cout << endl;
        }


}




bool CgameMap::checkAccess()
{
    queue<pair<int,int>> access_queue;

    for(auto it = c_entraceCords.begin(); it != c_entraceCords.end(); it++)
    {
        int x = it->second.second;
        int y = it->second.first;
        c_gameMap[y][x]->c_accessed=true;
        access_queue.push(make_pair(y,x));

        while(!access_queue.empty())
        {
            if(access_queue.front().first <= 0 || access_queue.front().first >= (int)(c_gameMap.size()-1) 
                || access_queue.front().second >= (int)(c_map_width-1) || access_queue.front().second <= 0  ) 
            {
                cout <<  "out of bounds" << endl;
                cout << "Coord: " << access_queue.front().first << " and " <<  access_queue.front().second << endl;
                cout << "sizes: " << c_gameMap.size() << " and "  << c_map_width<< endl;
                clearMap();
                return false;
            }

            for(int l = -1; l < 2;l++)
            {
                for(int m = -1; m < 2; m++)
                {
                    if(!c_gameMap[access_queue.front().first+l][access_queue.front().second+m]->c_accessed 
                        && c_gameMap[access_queue.front().first+l][access_queue.front().second+m]->c_passable)
                    {

                        access_queue.push(make_pair(access_queue.front().first+l,access_queue.front().second+m));
                    }
                    c_gameMap[access_queue.front().first+l][access_queue.front().second+m]->c_accessed=true; 

                    if(!(m == 0 && l==0))
                        c_gameMap[access_queue.front().first][access_queue.front().second]->c_Neighbours.push_back(c_gameMap[access_queue.front().first+l][access_queue.front().second+m]);
                    
                    
                }

            }
            access_queue.pop();

            if(access_queue.empty() && it == c_entraceCords.begin() 
                && !c_gameMap[c_exitCords.begin()->first][c_exitCords.begin()->second]->c_accessed)
            {
                cout << "Exit is unreachable by atleast 1 entrance" << endl;
                clearMap();
                return false;
            }

            if(!access_queue.empty() && it != c_entraceCords.begin())
            {
                cout << "Entrance has access to non-available spaces though whitch it cannot reach the exit or another entrance" << endl;
                clearMap();
                return false;
            }
        }
    }
    return true;
}

void CgameMap::clearMap()
{
    c_entraceCords.clear();
    c_exitCords.clear();
    c_gameMap.clear();
    c_EnemyHp = 0;
    c_MaxRounds = 0;
    c_score = 0;
    c_passive_income = 0;
    c_money = 0;
}

bool CgameMap::readStatus(list<string> & mapFolder)
{
    string statusName;
    if(readAtributeType(*(mapFolder.begin())) != "STATUS:" || !readProperty(*(mapFolder.begin()),"STATUS:",statusName) || !statusName.empty() )
    {
        cout << "incorrect declaration of map status" << endl;
        clearMap();
        return false;
    }
    mapFolder.pop_front();

    
    if(readAtributeType(*(mapFolder.begin())) != "HEALTH:" || !readProperty(*(mapFolder.begin()),"HEALTH:",c_EnemyHp) || c_EnemyHp <= 0)
    {
        cout << "incorect health value" << endl;
        clearMap();
        return false;
    }
    mapFolder.pop_front();

    if(readAtributeType(*(mapFolder.begin())) != "MAX_ROUNDS:" || !readProperty(*(mapFolder.begin()),"MAX_ROUNDS:",c_MaxRounds) || c_MaxRounds <= 0)
    {
        cout << "incorect max rounds value" <<  endl;
        clearMap();
        return false;
    }
    mapFolder.pop_front();

    if(readAtributeType(*(mapFolder.begin())) != "MONEY:" || !readProperty(*(mapFolder.begin()),"MONEY:",c_money) || c_money < 0)
    {
        cout << "incorect money value" <<  endl;
        clearMap();
        return false;
    }
    mapFolder.pop_front();

    if(readAtributeType(*(mapFolder.begin())) != "INCOME:" || !readProperty(*(mapFolder.begin()),"INCOME:",c_passive_income) || c_passive_income < 0)
    {
        cout << "incorect income value" <<  endl;
        clearMap();
        return false;
    }
    mapFolder.pop_front();

    return true;
}

bool CgameMap::readAttacker(list <string> & AttackerFolder)
{
    string leftOvers;
    
    Cdamage_type resistance;
    Cdamage_type dmg_type;

    map<string,bool> mand_atributes = { {"NAME:",0} , {"SYMBOL:",0}, {"HEALTH:",0} , {"DAMAGE:",0}, 
                            {"SPEED:",0}, {"COST:",0}};

    map<string,string> s_names = { {"NAME:","" }, {"RESISTANCE:",""}, {"DAMAGE_TYPE:",""} };
    map<string,char> c_names = {{ "SYMBOL:",'0'}};
    map<string,int> i_names = {  {"HEALTH:",0} , {"DAMAGE:",0}, {"SPEED:",0}, {"COST:",0} };


    while(!AttackerFolder.empty())
    {
        auto line = AttackerFolder.begin();
        if(c_loadingSave && readAtributeType(*line) == "END_OF_UNITS")
        {
            if(!readProperty(*line,"END_OF_UNITS",leftOvers) || !leftOvers.empty())
            {
                cout << "wrong syntax of unit save data" << endl;
                return false;
            }
            AttackerFolder.pop_front();
            c_number_of_available_units = c_unit_tiles.size();
            return true;
        }

        if(readAtributeType(*line) == "UNIT:" )  
        {
            if(!readProperty(*line,"UNIT:",leftOvers) || !leftOvers.empty()) 
            {
                cout << "wrong syntax of unit data" << endl;
                return false;
            }
            line++;
            AttackerFolder.pop_front();

            if(!ReadAtributeData(line,AttackerFolder,resistance,dmg_type,mand_atributes,s_names,c_names,i_names) )
                return false;

            
            c_unit_tiles.emplace(s_names["NAME:"],shared_ptr<Cattacker>( 
                    new Cattacker(c_names["SYMBOL:"],true,0,0,s_names["NAME:"], i_names["HEALTH:"], i_names["DAMAGE:"], 
                                    i_names["SPEED:"], i_names["COST:"],resistance,dmg_type) ) );  
            resistance.clearElements();
            dmg_type.clearElements();
            continue;


        }
        if(!readAtributeType(*line).empty() )
        {
            cout << "invalid atribute" << endl;
            return false;
        }
        
        AttackerFolder.pop_front();

    }
    c_number_of_available_units = c_unit_tiles.size();

    return true;
}

bool CgameMap::ReadAtributeData(std::_List_iterator<std::string> & line,list <string> & UnitFolder, 
            Cdamage_type & resistance,Cdamage_type & dmg_type, map<string,bool>  & mand_atributes, 
            map<string,string> & s_names,map<string,char> & c_names, map<string,int> & i_names)
{
    string atribute_name;
    string s_temp;
    char c_temp = '0';
    int i_temp = 0;

    while(!UnitFolder.empty() && !(*line).empty() && !readAtributeType(*line).empty())
    {
        atribute_name = readAtributeType(*line); 

        if(s_names.find(atribute_name) != s_names.end())
        {
            if(!readProperty(*line,atribute_name,s_temp) || s_temp.empty()) 
            {
                cout << "wrong syntax of unit's " << atribute_name << endl;
                return false;
            }

            auto mand_it = mand_atributes.find(atribute_name);
            if(mand_it != mand_atributes.end() && mand_it->second)
            {
                cout << "'" << atribute_name << "'" << " is already present" << endl;
                return false;
            }

            s_names[atribute_name] = s_temp;

            if(mand_it != mand_atributes.end())
                mand_it->second=true;
            else
            {
                if(atribute_name == "RESISTANCE:")
                    if(!resistance.addElement(s_temp) )
                    {
                        cout << "duplicate resistances" << endl;
                        return false;
                    }

                if(atribute_name == "DAMAGE_TYPE:")
                    if(!dmg_type.addElement(s_temp))
                    {
                        cout << "duplicate damage types" << endl;
                        return false;
                    }
            }

            UnitFolder.pop_front();
            line =UnitFolder.begin();
            s_temp.clear();
            continue;
        }

        if(i_names.find(atribute_name) != i_names.end())
        {
            if(!readProperty(*line,atribute_name,i_temp) || i_temp < 0) 
            {
                cout << "wrong syntax of unit's " << atribute_name << endl;
                return false;
            }

            auto mand_it = mand_atributes.find(atribute_name);
            if(mand_it->second)
            {
                cout << "'" << atribute_name << "'" << " is already present" << endl;
                return false;
            }

            if(mand_it != mand_atributes.end())
                mand_it->second=true;

            i_names[atribute_name] = i_temp;
            UnitFolder.pop_front();
            line = UnitFolder.begin();
            i_temp = 0;
            continue;
        }

        if(c_names.find(atribute_name) != c_names.end())
        {
            if(!readProperty(*line,atribute_name,c_temp) || c_temp == '0') 
            {
                cout << "wrong syntax of unit's " << atribute_name << endl;
                return false;
            }
            auto mand_it = mand_atributes.find(atribute_name);

            if(mand_it->second)
            {
                cout << "'" << atribute_name << "'" << " is already present" << endl;
                return false;
            }

            if(mand_it != mand_atributes.end())
                mand_it->second=true;

            c_names[atribute_name] = c_temp;
            UnitFolder.pop_front();
            line = UnitFolder.begin();
            c_temp = '0';
            continue;
        }

        cout << "atribute name not present" << endl;
        return false;
    }

    for(auto it = mand_atributes.begin(); it != mand_atributes.end(); it++)
    {
        if(!it->second)
        {
            cout << "missing a mandatory atribute: " << it->first << endl;
            return false;
        }
        it->second=false;
    }
    return true;
}


bool CgameMap::readTowers(list <string> & towerFolder)
{
    string leftOvers;
    
    Cdamage_type resistance;
    Cdamage_type dmg_type;

    map<string,bool> mand_atributes = { {"NAME:",0} , {"SYMBOL:",0}, {"HEALTH:",0} , {"DAMAGE:",0}, 
                             {"COOLDOWN:",0},{"TIMER:",0}, {"TYPE:",0},{"DISTANCE:",0}}; 

    map<string,string> s_names = { {"NAME:","" }, {"RESISTANCE:",""}, {"DAMAGE_TYPE:",""},{"TYPE:",""} };
    map<string,char> c_names = {{ "SYMBOL:",'0'}};
    map<string,int> i_names = {  {"HEALTH:",0} , {"DAMAGE:",0}, {"COOLDOWN:",0},{"TIMER:",0},{"DISTANCE:",0} };

    while(!towerFolder.empty())
    {
        auto line = towerFolder.begin();

        if(c_loadingSave && readAtributeType(*line) == "END_OF_TOWERS")
        {
            if(!readProperty(*line,"END_OF_TOWERS",leftOvers) || !leftOvers.empty()) 
            {
                cout << "wrong syntax of tower save data" << endl;
                return false;
            }
            towerFolder.pop_front();
            return true;
        }

        if(readAtributeType(*line) == "TOWER:" )  
        {
            if(!readProperty(*line,"TOWER:",leftOvers) || !leftOvers.empty()) 
            {
                cout << "wrong syntax of unit data" << endl;
                return false;
            }
            line++;
            towerFolder.pop_front();

            if(!ReadAtributeData(line,towerFolder,resistance,dmg_type,mand_atributes,s_names,c_names,i_names) )
                return false;

            if(s_names["TYPE:"] == "AOE")
            {
                c_tower_tiles.emplace(s_names["NAME:"],shared_ptr<Ctower>( 
                    new CTowerAOE(c_names["SYMBOL:"],true,0,0,s_names["NAME:"], i_names["HEALTH:"], i_names["DAMAGE:"]
                    ,resistance,dmg_type,i_names["COOLDOWN:"],i_names["TIMER:"],s_names["TYPE:"],i_names["DISTANCE:"]) ) );
            }
            else 
                if(s_names["TYPE:"] == "ST")
                    c_tower_tiles.emplace(s_names["NAME:"],shared_ptr<Ctower>( 
                            new CTowerSingleTG(c_names["SYMBOL:"],true,0,0,s_names["NAME:"], i_names["HEALTH:"], i_names["DAMAGE:"]
                            ,resistance,dmg_type,i_names["COOLDOWN:"],i_names["TIMER:"],s_names["TYPE:"],i_names["DISTANCE:"]) ) ); 
                else
                    if(s_names["TYPE:"] == "BLOCK")
                    c_tower_tiles.emplace(s_names["NAME:"],shared_ptr<Ctower>( 
                            new CTowerObstacle(c_names["SYMBOL:"],true,0,0,s_names["NAME:"], i_names["HEALTH:"], i_names["DAMAGE:"]
                            ,resistance,dmg_type,i_names["COOLDOWN:"],i_names["TIMER:"],s_names["TYPE:"],i_names["DISTANCE:"]) ) ); 
                    else
                    {
                        cout << "Specified tower type does not exist" << endl;
                        return false;
                    }
            
            resistance.clearElements();
            dmg_type.clearElements();
            continue;

        }
        if(!readAtributeType(*line).empty() )
        {
            cout << "invalid atribute" << endl;
            return false;
        }
        
        towerFolder.pop_front();

    }


    return true;
}


void CgameMap::display_units() const
{
    int i = 0;
    for(auto & unit : c_unit_tiles)
    {
        cout << ++i << " - " <<setfill(' ') <<   setw(20) << left << unit.first  << "HP: "  <<setw(10) <<unit.second->c_HP  << "DMG: " 
          <<  setw(10)  << unit.second->c_damage   << "COST: " <<  setw(10) << unit.second->c_cost  << endl;
    }
}

bool CgameMap::choose_units(int const chosen_unit_num, int const numOfUnits)
{
    c_temp_active_units.clear();
    auto it = c_unit_tiles.begin();
    for (int i = 0; i < chosen_unit_num-1; i++)
    {
        it++;
    }

    if((it)->second->c_cost*numOfUnits > c_money)
    {
        cout << "You do not have enough money to place this unit" << endl;
        return false;
    }

    for(int i = 0; i < numOfUnits;i++)
        c_temp_active_units.push_back(shared_ptr<Cattacker>( new Cattacker( (*it->second )  ) ) );

    return true;
}

void CgameMap::place_units(int const entrace )
{
    for(auto & unit : c_temp_active_units)
    {
        c_active_units.push_back(move(unit));
        // c_active_units.push_back(move(*c_temp_active_units.begin()));

        auto it = c_entraceCords.begin();
        for (int i = 1; i < entrace; i++)
            it++;
        
        int y = it->second.first;
        int x = it->second.second;
        c_gameMap[y][x]->c_unitTile_ptr = (c_active_units.back());
        c_active_units.back()->c_mapTile_ptr = c_gameMap[y][x];

        c_money -= c_active_units.back()->c_cost;

        ResetPathCalc();
        c_active_units.back()->findPath(c_gameMap[c_exitCords.begin()->first][c_exitCords.begin()->second]);
    }
    

    
    c_temp_active_units.clear();
}


void CgameMap::advanceRound(const int Roundnum) 
{
    for(int i = 0; i < Roundnum; i++)
    {
        if(c_EnemyHp < 1)
        {
            c_score += (c_MaxRounds - c_round)*100;
            c_victory = true;
            return;
        }

        if(++c_round > c_MaxRounds)
        {
            c_defeat = true;
            return;
        }
        c_money += c_passive_income;

        placeTowers();

        for(auto tower : c_active_towers) 
        {
        tower->attack_enemy();
        }

        for(auto it = c_active_units.begin(); it != c_active_units.end();)
        {
            if((*it)->c_HP <= 0)
            {
                cout << "a unit has been killed" << endl;
                (*it)->c_mapTile_ptr.lock()->Occupied_by_unit = false;
                c_active_units.erase(it++);
                continue;
            }
            it++;
        }

        auto it = c_active_units.begin();
        while(it != c_active_units.end())
        {
            ResetPathCalc();
            (*it)->findPath(c_gameMap[c_exitCords.begin()->first][c_exitCords.begin()->second]);

            (*it)->move_foward(it);
            if((*it)->c_pathToExit.empty()) 
            {
                cout <<   "A unit has reached the end" << endl;
                c_money += (*it)->c_damage*5;
                c_score += (*it)->c_damage*10;
                c_EnemyHp -= (*it)->c_damage;
                (*it)->c_mapTile_ptr.lock()->Occupied_by_unit=false;
                c_active_units.erase(it++);
                continue;
            }
            it++;
        }

        for(auto it = c_active_towers.begin(); it != c_active_towers.end();)
        {
            c_money += ((*it)->c_money_worth - ((*it)->c_HP)); 
            c_score += ((*it)->c_score_worth - ((*it)->c_HP*10));
            (*it)->c_money_worth = (*it)->c_HP;
            (*it)->c_score_worth = (*it)->c_HP*10;
            if((*it)->c_HP <= 0)
            {
                cout << "a tower has been eliminated " << endl;
                (*it)->c_mapTile_ptr.lock()->Occupied_by_tower = false;
                c_active_towers.erase(it++);
                continue;
            }
            it++;
        }

    }

}


list<string> CgameMap::saveData()
{
    list<string> dataToSave;
    for(auto & tower : c_tower_tiles)
    {
        dataToSave.push_back("TOWER:");
        dataToSave.splice(dataToSave.end(),tower.second->SaveUnitData(false));
        
    }

    dataToSave.push_back("END_OF_TOWERS");
    dataToSave.push_back(" ");

    for(auto & unit : c_unit_tiles)
    {
        dataToSave.push_back("UNIT:");
        dataToSave.splice(dataToSave.end(),unit.second->SaveUnitData(false));
        
    }

    dataToSave.push_back("END_OF_UNITS");

    dataToSave.push_back("STATUS:");
    dataToSave.push_back("HEALTH: " + to_string(c_EnemyHp));
    dataToSave.push_back("MAX_ROUNDS: " + to_string(c_MaxRounds));
    dataToSave.push_back("MONEY: " + to_string(c_money));
    dataToSave.push_back("INCOME: " + to_string(c_passive_income));

    string line;
    for(auto & vec_line  : c_gameMap)
    {
        for(auto & tile : vec_line)
        {
            line += string(1,tile->c_symbol);
        }
        dataToSave.push_back(line);
        line.clear();
    }
    dataToSave.push_back("END_OF_MAP");
    dataToSave.push_back("ROUND: " + to_string(c_round));
    dataToSave.push_back("SCORE: " + to_string(c_score));

    for(auto & tower : c_active_towers)
    {
        dataToSave.push_back("ACTIVE_TOWER:");
        dataToSave.splice(dataToSave.end(),tower->SaveUnitData(true));
    }

    for(auto & unit : c_active_units)
    {
        dataToSave.push_back("ACTIVE_UNIT:");
        dataToSave.splice(dataToSave.end(),unit->SaveUnitData(true));
    }

    return dataToSave;
}

bool CgameMap::loadSave(list <string> & save)
{
    c_loadingSave = true;

    if(!readTowers(save))
    {
        cout << "failed to read tower data in save file" << endl;
        return false;
    }
    if(!readAttacker(save))
    {
        cout << "failed to read unit data in save file" << endl;
        return false;
    }
    if(!readMap(save))
    {
        cout << "failed to read map data in save file" << endl;
        return false;
    }
    c_round = 0; 
    if(readAtributeType(*save.begin()) == "ROUND:")
    {
        if(!readProperty(*save.begin(),"ROUND:",c_round) || c_round < 0) 
        {
            cout << "wrong syntax of round data" << endl;
            return false;
        }
    }
    save.pop_front();

    if(readAtributeType(*save.begin()) == "SCORE:")
    {
        if(!readProperty(*save.begin(),"SCORE:",c_score) || c_score < 0) 
        {
            cout << "wrong syntax of score data" << endl;
            return false;
        }
    }
    save.pop_front();

    if(!readActiveUnits(save))
    {
        cout << "failed to load active units" << endl;
    }

    

    return true;
}


void CgameMap::ResetPathCalc()
{
    for(auto & line : c_gameMap)
    {
        for(auto & it : line)
        {
            it->c_path_visited=false;
            it->c_fGlobalGoal= numeric_limits<float>::max();
            it->c_fLocalGoal= numeric_limits<float>::max();
            it->c_parent.lock() = nullptr;
            it->c_range_distance = numeric_limits<int>::max();
            it->c_path_visited = 0;

        }
    }
}

void CgameMap::resetGame()
{
    clearMap();
    c_unit_tiles.clear();
    c_tower_tiles.clear();
    c_active_towers.clear();
    c_active_units.clear();
    c_empty_spaces.clear();
}



void CgameMap::placeTowers()
{ 
    c_best_towerSpotST.clear();
    c_best_towerSpotAOE.clear();
    c_best_towerSpotBLOCK.clear();
    int AOE;
    int ST;
    int BLOCK;
    int x;
    int y;

    for(auto & space : c_empty_spaces) 
    {
        if(space.lock()->c_unitTile_ptr.expired() )
        {
            space.lock()->getPosValue(AOE,ST,BLOCK);
            c_best_towerSpotST.emplace(ST,make_pair(space.lock()->c_y,space.lock()->c_x));
            c_best_towerSpotAOE.emplace(AOE,make_pair(space.lock()->c_y,space.lock()->c_x));
            c_best_towerSpotBLOCK.emplace(BLOCK,make_pair(space.lock()->c_y,space.lock()->c_x));
        } 
    }
    auto spotST = c_best_towerSpotST.begin();
    auto spotAOE = c_best_towerSpotAOE.begin();
    auto spotBLOCK = c_best_towerSpotBLOCK.begin();


    for(auto it = c_tower_tiles.begin(); it != c_tower_tiles.end(); it++ )
    {
        (*it).second->c_timer--;
        if((*it).second->c_timer <= 0)
        {
            
            if(c_best_towerSpotST.empty() || c_best_towerSpotAOE.empty() || c_best_towerSpotBLOCK.empty() )
            {
                cout << "No more free spots to place towers" << endl;
                return;
            }

            if(it->second->c_type  == "AOE")
            {
                c_active_towers.push_back(shared_ptr<Ctower>( new CTowerAOE( (*it->second ))  ) );
                x = spotAOE->second.second;
                y = spotAOE->second.first;
                spotAOE++;
            }
            else
            {
                if(it->second->c_type  == "ST")
                {
                    c_active_towers.push_back(shared_ptr<Ctower>( new CTowerSingleTG( (*it->second ))  ) );
                    x = spotST->second.second;
                    y = spotST->second.first;
                    spotST++;
                }
                else
                {
                    if(it->second->c_type  == "BLOCK")
                    {
                        c_active_towers.push_back(shared_ptr<Ctower>( new CTowerObstacle( (*it->second ))  ) );
                        x = spotBLOCK->second.second;
                        y = spotBLOCK->second.first;
                        spotBLOCK++;
                    }
                    else
                    {
                        cout << "Issue placing tower, nonexistant type" << endl;
                        return;
                    }
                }
            } 

            c_gameMap[y][x]->c_unitTile_ptr = c_active_towers.back();
            c_active_towers.back()->c_mapTile_ptr = c_gameMap[y][x];
            c_gameMap[y][x]->Occupied_by_tower = true;
            (*it).second->c_timer = (*it).second->c_cooldown;

            ResetPathCalc();
            c_active_towers.back()->c_reach = 3;
            c_active_towers.back()->get_reachable_tiles();

            x = spotAOE->second.second;
            y = spotAOE->second.first;
            while(spotAOE != c_best_towerSpotAOE.end() && c_gameMap[y][x]->Occupied_by_tower )
            {
                spotAOE++;
            }

            x = spotST->second.second;
            y = spotST->second.first;
            while( spotST != c_best_towerSpotST.end() && c_gameMap[y][x]->Occupied_by_tower )
            {
                spotST++;
            }

            x = spotBLOCK->second.second;
            y = spotBLOCK->second.first;
            while(  spotBLOCK != c_best_towerSpotBLOCK.end() && c_gameMap[y][x]->Occupied_by_tower)
            {
                spotBLOCK++;
            }
            
            if(spotAOE == c_best_towerSpotAOE.end() || spotST == c_best_towerSpotST.end() ||  spotBLOCK == c_best_towerSpotBLOCK.end())
            {
                return;
            }
        }
    }
}

bool CgameMap::readActiveUnits(list <string> & save)
{
    string leftOversTower;
    string leftOversUnit;
    string tile_name;
    Cdamage_type resistance;
    Cdamage_type dmg_type;
    map<string,bool> mand_atributesTower = { {"NAME:",0} , {"SYMBOL:",0}, {"HEALTH:",0} , {"DAMAGE:",0}, 
                             {"COOLDOWN:",0}, {"X:",0}, {"Y:",0},{"TIMER:",0} , {"TYPE:",0},{"DISTANCE:",0}}; 
    map<string,string> s_namesTower = { {"NAME:","" }, {"RESISTANCE:",""}, {"DAMAGE_TYPE:",""} , {"TYPE:",""} };
    map<string,char> c_namesTower = {{ "SYMBOL:",'0'}};
    map<string,int> i_namesTower = {  {"HEALTH:",0} , {"DAMAGE:",0}, {"COOLDOWN:",0}, {"X:",0}, {"Y:",0} ,{"TIMER:",0},{"DISTANCE:",0}};

    map<string,bool> mand_atributesUnit = { {"NAME:",0} , {"SYMBOL:",0}, {"HEALTH:",0} , {"DAMAGE:",0}, 
                            {"SPEED:",0}, {"COST:",0}, {"X:",0}, {"Y:",0}}; 
    map<string,string> s_namesUnit = { {"NAME:","" }, {"RESISTANCE:",""}, {"DAMAGE_TYPE:",""} };
    map<string,char> c_namesUnit = {{ "SYMBOL:",'0'}};
    map<string,int> i_namesUnit = {  {"HEALTH:",0} , {"DAMAGE:",0}, {"SPEED:",0}, {"COST:",0} , {"X:",0}, {"Y:",0}};

    int x;
    int y;

    while(!save.empty()) 
    {
        auto line = save.begin();
        tile_name = readAtributeType(*line);
        if(tile_name == "ACTIVE_TOWER:" || tile_name == "ACTIVE_UNIT:") 
        {
            if((!readProperty(*line,"ACTIVE_TOWER:",leftOversTower) && !readProperty(*line,"ACTIVE_UNIT:",leftOversUnit)) 
                || (!leftOversTower.empty() && !leftOversUnit.empty())) 
            {
                cout << "wrong syntax of unit data" << endl;
                return false;
            }
            line++;
            save.pop_front();

            if(tile_name == "ACTIVE_TOWER:") 
                if(!ReadAtributeData(line,save,resistance,dmg_type,mand_atributesTower,s_namesTower,c_namesTower,i_namesTower) )
                    return false;
                else
                {
                    if(readAtributeType(s_namesTower["TYPE:"]) == "AOE" && readProperty(s_namesTower["TYPE:"],"AOE",leftOversTower) &&  leftOversTower.empty())
                    {
                        c_active_towers.push_back(shared_ptr<Ctower>( 
                            new CTowerAOE(c_namesTower["SYMBOL:"],true,0,0,s_namesTower["NAME:"], i_namesTower["HEALTH:"], i_namesTower["DAMAGE:"]
                            ,resistance,dmg_type,i_namesTower["COOLDOWN:"],i_namesTower["TIMER:"],s_namesTower["TYPE:"],i_namesTower["DISTANCE:"]) ) );
                    }
                    else 
                        if(readAtributeType(s_namesTower["TYPE:"]) == "ST" && readProperty(s_namesTower["TYPE:"],"ST",leftOversTower) &&  leftOversTower.empty())

                            c_active_towers.push_back(shared_ptr<Ctower>( 
                                    new CTowerSingleTG(c_namesTower["SYMBOL:"],true,0,0,s_namesTower["NAME:"], i_namesTower["HEALTH:"], i_namesTower["DAMAGE:"]
                                    ,resistance,dmg_type,i_namesTower["COOLDOWN:"],i_namesTower["TIMER:"],s_namesTower["TYPE:"],i_namesTower["DISTANCE:"]) ) ); 
                        else
                            if(readAtributeType(s_namesTower["TYPE:"]) == "BLOCK" && readProperty(s_namesTower["TYPE:"],"BLOCK",leftOversTower) &&  leftOversTower.empty())
                            c_active_towers.push_back(shared_ptr<Ctower>( 
                                    new CTowerObstacle(c_namesTower["SYMBOL:"],true,0,0,s_namesTower["NAME:"], i_namesTower["HEALTH:"], i_namesTower["DAMAGE:"]
                                    ,resistance,dmg_type,i_namesTower["COOLDOWN:"],i_namesTower["TIMER:"],s_namesTower["TYPE:"],i_namesTower["DISTANCE:"]) ) ); 
                            else
                            {
                                cout << "Specified tower type does not exist" << endl;
                                return false;
                            }

                    x = i_namesTower["X:"];
                    y = i_namesTower["Y:"];         
                    if(!c_gameMap[y][x]->c_unitTile_ptr.expired())
                    {
                        cout << "cannot place tower, space is already occupied" << endl;
                        return false;
                    }
                    c_gameMap[y][x]->c_unitTile_ptr = c_active_towers.back();
                    c_active_towers.back()->c_mapTile_ptr = c_gameMap[y][x]; 
                    c_gameMap[y][x]->Occupied_by_tower = true;
                }

            else 
                if(!ReadAtributeData(line,save,resistance,dmg_type,mand_atributesUnit,s_namesUnit,c_namesUnit,i_namesUnit) )
                    return false;
                else
                {
                    c_active_units.push_back(shared_ptr<Cattacker>( new Cattacker( c_namesUnit["SYMBOL:"],true,0,0,s_namesUnit["NAME:"],
                                    i_namesUnit["HEALTH:"], i_namesUnit["DAMAGE:"],i_namesUnit["SPEED:"], i_namesUnit["COST:"],resistance,dmg_type) ) );
                    
                    x = i_namesUnit["X:"];
                    y = i_namesUnit["Y:"];
                    c_gameMap[y][x]->c_unitTile_ptr = c_active_units.back();
                    c_active_units.back()->c_mapTile_ptr = c_gameMap[y][x];

                    ResetPathCalc();

                    c_active_units.back()->findPath(c_gameMap[c_exitCords.begin()->first][c_exitCords.begin()->second]);
                    c_gameMap[y][x]->Occupied_by_unit = true;

                }

            resistance.clearElements();
            dmg_type.clearElements();
            continue;

        }
        if(!readAtributeType(*line).empty() )
        {
            cout << *line << endl;
            // cout << tile_name << endl;
            cout << "invalid atribute" << endl;
            return false;
        }
        save.pop_front();
    }
    return true;
}
