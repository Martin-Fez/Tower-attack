#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <memory>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <queue>


#include "tiles/Maptiles/empty_space.hpp"
#include "tiles/Maptiles/wall.hpp"
#include "tiles/attackers/attacker.hpp"
#include "tiles/towers/TowerObstacle.hpp"
#include "tiles/towers/TowerSingleTG.hpp"
#include "tiles/towers/TowerAOE.hpp"
#include "extra_func/readPropertyFunc.hpp"



using namespace std;

/**
 * @brief Class for managing the game Map and its units
 * 
 */
class CgameMap
{
    public:
        
        /**
         * @brief Function for loading inputed data as a game
         * 
         * Fucntion responsible for loading game data from a save. The fucntion uses the functions readTowers,
         * readUnits and readMap to load their respective data, then loads current score and round and
         * loads active units though loadActiveUnits
         * 
         * @param save Inputed data of the save
         * @return true succesfully laoded
         * @return false Failed to load
         */
        bool loadSave(list <string> & save);
        
        /**
         * @brief Function reads inputed tower data
         * 
         * Function reads tower data and creates template tower tiles based off from the given data
         * The function also creates specific tower types based off the given type
         * Function while reading checks whether input contains only data needed for creation of a tower.
         * Each tower is initialized with line containing "TOWER:"
         * 
         * @param towers Inputed data of the towers
         * @return true Success reading tower data
         * @return false Failure reading tower data
         */
        bool readTowers(list <string> & towerFolder);

        /**
         * @brief Function reads inputed attacker data
         * 
         * Function reads attacker data and creates template attacker tiles based off from the given data
         * The function also creates specific attacker types based off the given type
         * Function while reading checks whether input contains only data needed for creation of a attacker.
         * Each attacker is initialized with line containing "UNIT:"
         * 
         * @param AttackerFolder Inputed data of the attackers 
         * @return true Success reading attacker data
         * @return false Failure reading attacker data
         */
        bool readAttacker(list <string> & AttackerFolder);

        /**
         * @brief Function for reading ASCII and data of a map
         * 
         * Function reads inputed data and checks whether it is correct or not.
         * It checks the ASCII art of the map and checks for entrances, exits and invalid characters.
         * After it finishes reading the data, function further checks calls check access, 
         * checks whether the sequence of entrances is correct and that there is only one exit.
         * Finaly the function inputs pointers to all empty spaces into c_empty_spaces 
         *  
         * 
         * @param map Data containing data of the map and ASCII definining the map
         * @return true Success reading map data
         * @return false Failure reading attacker data
         */
        bool readMap(list <string> & map);

        /**
         * @brief Function for reading all active units
         * 
         * Function responsible for loading all of the active units from a save.
         * It determines whether it is a attacker or a tower, loads their respective parameters,
         * inputs them into their respective containers and using coordinates specified within
         * the data connects it with a tile on the loaded map.
         * 
         * @param save Data contaning all the units
         * @return true Success loading active units
         * @return false Failure loading active units
         */
        bool readActiveUnits(list <string> & save);



        /**
         * @brief Function for displaying current active map
         * 
         * Function for displaying current active map and all the units upon it.
         * 
         */
        void display_map() const;

        /**
         * @brief Function for displaying all the available units
         * 
         * Function that displays all of the units that were loaded and are available.
         * The parameters that are displayed are: name, HP, DMG and cost
         * 
         */
        void display_units() const;

        /**
         * @brief Function for loading a chosen number of unit
         * 
         * Function loads a chosen unit as a copy from c_unit_tiles and places it into a temporary container
         * 
         * @param chosen_unit_num Number coresponding to the chosen unit
         * @param numOfUnits Number of chosen units
         * @return true true Enough money is available and the unit can be placed
         * @return false  false Enough money is not available and the unit cannot be placed
         */
        bool choose_units(int const chosen_unit_num, int const numOfUnits);

        /**
         * @brief Function responsible for placing units on the map
         * 
         * Function places and connects the unit to the map at the specified entrance.
         * After which it calculates it's path to the exit
         * 
         * @param entrace entrances at which to place the unit
         */
        void place_units(int const entrace);

        /**
         * @brief Function responsible for advancing the round
         * 
         * Function for Roundnum rounds advances the game.
         * Per each round the function first checks whether one the game ending conditions have been met
         * After that it places all avaiable towers that can be placed and attacks with the towers.
         * All of the killed attackers are removed. After this each attacker recalculates their path moves forward along it.
         * If an attacker reaches the end it is removed from the game after dealing damage to the enemy's HP and the player is awarded score and money.
         * Finaly all of the towers that have been killed by the attacker are removed and additional money for the player is calculated from the HP diffrence
         * 
         * @param Roundnum Number of rounds the user wishes to advance
         */
        void advanceRound(const int Roundnum);

        /**
         * @brief Function for saving all of the active game's data into a list of strings
         * 
         * Function takes all of the game's towers, attackers, it's map, status and active units
         * and inputs in a format that can be read by other functions into a list of strings
         * 
         * @return list<string> Active game's data
         */
        list<string> saveData();

        /**
         * @brief Function for reseting all the game data within the class
         * 
         */
        void resetGame();

        /**
         * @brief Function for placing a tower
         * 
         * Function calculates best spots for respective tower types and then checks if any of the towers can be placed.
         * If yes, it will place it into the calculated spot on the map and connect it.
         * 
         */
        void placeTowers();




        map<char,pair <int,int> > c_entraceCords;
        set<pair <int,int> > c_exitCords;
        int c_number_of_available_units;
        int c_number_of_entraces;

        bool c_victory = false;
        bool c_defeat = false;
        int c_score = 0;

        

    private:

        /**
         * @brief Function for checking validity and access of an inputed map
         * 
         * Function checks using floodfill whether it can reach from every entrace the exit of the map.
         * Return false if it cannot or if it can go out of bounds.
         * Further more for each tile it accesses it creates a connection with it's neighbours.
         * 
         * @return true Valid map layout
         * @return false Invalid map layout
         */
        bool checkAccess();

        /**
         * @brief clears the map's data
         * 
         */
        void clearMap();

        /**
         * @brief Reads map's status
         * 
         * Function for reading the status of the map
         * 
         * @param mapFolder Map's data
         * @return true Successfully read data
         * @return false Failed to read data
         */
        bool readStatus(list<string> & mapFolder);

        /**
         * @brief Function for reading the data of a unit
         * 
         * Function goes though all of unit's data and assigns them to their respective containers.
         * It checks whether the data is in a correct format and if it's declaration is correct.
         * It also checks for requested mandatory atributes and if not all of them are read the function
         * returns false.
         * It also adds damage types and resistances and checks if there any duplicites.
         * 
         * @param line Iterator where pointing to the current line
         * @param UnitFolder Folder containing data of the unit
         * @param resistance unit's resistances
         * @param dmg_type unit's damage type
         * @param mand_atributes unit's mandatory atributes that must be read once
         * @param s_names string parameters
         * @param c_names char parameters
         * @param i_names int parameters
         * @return true Succesfully read unit's data
         * @return false Failed to read unit's data
         */
        bool ReadAtributeData(std::_List_iterator<std::string> & line,list <string> & UnitFolder, 
            Cdamage_type & resistance,Cdamage_type & dmg_type, map<string,bool> & mand_atributes, 
            map<string,string> & s_names,map<string,char> & c_names, map<string,int> & i_names);


        /**
         * @brief Function for reseting path calculation
         * 
         * Function for reseting each tile's values for path calculation
         * It also doubles as a way to reset the calculation of a tower's distance
         * 
         */
        void ResetPathCalc();





        vector<vector<shared_ptr<CMapTiles> > > c_gameMap;
        vector <weak_ptr<CMapTiles>> c_empty_spaces; 

        map<string,shared_ptr<Cattacker>> c_unit_tiles;
        map<string,shared_ptr<Ctower>> c_tower_tiles;

        list <shared_ptr<Ctower>> c_active_towers;
        list <shared_ptr<Cattacker>> c_active_units;
        list <shared_ptr<Cattacker>> c_temp_active_units;

        multimap <int,pair<int,int>> c_best_towerSpotAOE;
        multimap <int,pair<int,int>> c_best_towerSpotST;
        multimap <int,pair<int,int>> c_best_towerSpotBLOCK;
        
        size_t c_map_width = 0;
        bool c_loadingSave = false;
        int c_EnemyHp = 0;
        int c_passive_income = 0;
        int c_money = 0;
        int c_round = 1;
        int c_MaxRounds = 0;


};



#endif
