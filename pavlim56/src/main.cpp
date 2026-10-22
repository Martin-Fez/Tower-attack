#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <cstring>

#include "extra_func/dialogueFunc.hpp"
#include "extra_func/loadFileFunc.hpp"
#include "gameMap.hpp"

using namespace std;

int main()
{
    CgameMap current_game;

    CgameMap loadTest; // instance of CgameMap meant for testing whether loaded data is correct or not before loading in the proper one
    //meant as a way of allowing to keep the correct loaded data in current game without messing them up with new wrong ones

    LoadFile load;
    TEXT text;
    

    list<string> fileData;
    string mapFile;
    string unitFile;
    string saveFile;
    string towerFile;
    string newFileName;
    string directory_name;
    int choice;
    int choice2;
    bool goBack = false;
    bool specify;
    bool loop = true;

    while(loop)
    {
        text.printOptions(startMenu);
        specify = true;

        switch(text.getChoice(3))
        {
            case 1:
                cout << "do you wish use the default folders or to specify them manualy" << endl;
                cout << "0 - specify" << endl;
                cout << "1 - default" << endl;
                cout << setfill('-') << setw(80) << "-" << endl;
                if(text.getChoice(1))
                    specify = false;

                cout << "Choose a map:" << endl;

                if(!specify)
                    directory_name = "assets/maps";
                else
                {
                    cout << "type in directory from which you wish load files" << endl;
                    directory_name = text.getName();
                }

                while(1) 
                {
                    mapFile = load.chooseFile(load.fileNames(directory_name),goBack);
                    if(goBack) 
                        break;
                    
                    if(mapFile.empty())
                    {
                        cout << "problem opening the file,please fix the specified problem" << endl;
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }
                    
                    fileData = load.loadFile(mapFile);

                    if(fileData.empty())
                    {
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }

                    if( !current_game.readMap(fileData) )
                    {
                        cout << "map could not be loaded, choose a diffrent map:" << endl;
                        mapFile.clear();
                        continue;
                    }
                    break;
                }

                if(goBack) 
                {
                    goBack = false;
                    current_game.resetGame();
                    break;
                }

                fileData.clear();
                cout << "Choose units:" << endl;

                if(!specify)
                    directory_name = "assets/units";
                else
                {
                    cout << "type in directory from which you wish load files" << endl;
                    directory_name = text.getName();
                }

                while(1)
                {
                    unitFile = load.chooseFile(load.fileNames(directory_name),goBack);
                    if(goBack) 
                        break;

                    if(unitFile.empty())
                    {
                        cout << "problem opening the file,please fix the specified problem" << endl;
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }

                    fileData = load.loadFile(unitFile);

                    if(fileData.empty())
                    {
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }

                    if( !loadTest.readAttacker(fileData) )
                    {
                        cout << "units could not be loaded, choose a diffrent ones:" << endl;
                        loadTest.resetGame();
                        continue;
                    }

                    fileData = load.loadFile(unitFile);
                    current_game.readAttacker(fileData);

                    cout << "do you wish to add more units or to continue" << endl;
                    cout << "0 - add more" << endl;
                    cout << "1 - continue" << endl;
                    cout << setfill('-') << setw(80) << "-" << endl;
                    if(text.getChoice(1))
                        break;
                }
                
                if(goBack)
                {
                    goBack = false;
                    current_game.resetGame();
                    break;
                }

                fileData.clear();
                cout << setfill('-') << setw(80) << "-" << endl;
                cout << "Choose towers:" << endl;

                if(!specify)
                    directory_name = "assets/towers";
                else
                {
                    cout << "type in directory from which you wish load files" << endl;
                    directory_name = text.getName();
                }

                while(1)
                {
                    towerFile = load.chooseFile(load.fileNames(directory_name),goBack);
                    if(goBack)
                        break;

                    if(towerFile.empty())
                    {
                        cout << "problem opening the file,please fix the specified problem" << endl;
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }

                    fileData = load.loadFile(towerFile);

                    if(fileData.empty())
                    {
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }

                    if( !loadTest.readTowers(fileData) )
                    {
                        cout << "towers could not be loaded, choose a diffrent ones:" << endl;
                        loadTest.resetGame();
                        continue;
                    }

                    fileData = load.loadFile(towerFile);
                    current_game.readTowers(fileData);

                    cout << "do you wish to add more towers or to continue" << endl;
                    cout << "0 - add more" << endl;
                    cout << "1 - continue" << endl;
                    cout << setfill('-') << setw(80) << "-" << endl;

                    if(text.getChoice(1))
                        break;
                }

                if(goBack) 
                {
                    goBack = false;
                    current_game.resetGame();
                    break;
                }

                fileData.clear();
                loop = false;
                break;

            case 2:

                cout << "do you wish use the default folders or to specify them manualy" << endl;
                cout << "0 - specify" << endl;
                cout << "1 - default" << endl;
                cout << setfill('-') << setw(80) << "-" << endl;
                if(text.getChoice(1))
                    specify = false;

                cout << "Choose a save:" << endl;

                if(!specify)
                    directory_name = "assets/saves";
                else
                {
                    cout << "type in directory from which you wish load files" << endl;
                    directory_name = text.getName();
                }

                while(1)
                {
                    saveFile = load.chooseFile(load.fileNames(directory_name),goBack);
                    if(goBack)
                        break;

                    if(saveFile.empty())
                    {
                        cout << "problem opening the file,please fix the specified problem" << endl;
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }

                    fileData = load.loadFile(saveFile);

                    if(fileData.empty())
                    {
                        cout << setfill('-') << setw(80) << "-" << endl;
                        goBack = true;
                        break;
                    }

                    if( !loadTest.loadSave(fileData) )
                    {
                        cout << "save could not be loaded, choose a diffrent one:" << endl;
                        loadTest.resetGame();
                        continue;
                    }

                    fileData = load.loadFile(saveFile);
                    current_game.loadSave(fileData);
                    break;
                }
                
                if(goBack) 
                {
                    goBack = false;
                    current_game.resetGame();
                    break;
                }

                fileData.clear();
                loop = false;
                break;

            case 3:
                cout << "Quiting the game" << endl;
                return 0;
                break;

            default:
                cout << "reseting menu:" << endl;
                cout << setfill('-') << setw(80) << "-" << endl;
        }
    }

    


    while(true)
    {
        if(!(current_game.c_victory || current_game.c_defeat))
        current_game.display_map();
        
        if(current_game.c_victory)
        {
            cout << setfill('-') << setw(80) << "-" << endl;
            cout << setfill('-') << setw(80) << "-" << endl;
            cout << "You win !" << endl;
            cout << "SCORE : " << current_game.c_score << endl;
            cout << setfill('-') << setw(80) << "-" << endl;
            cout << setfill('-') << setw(80) << "-" << endl;
        }

        if(current_game.c_defeat)
        {
            cout << setfill('-') << setw(80) << "-" << endl;
            cout << setfill('-') << setw(80) << "-" << endl;
            cout << "You lose..." << endl;
            cout << "SCORE : " << current_game.c_score << endl;
            cout << setfill('-') << setw(80) << "-" << endl;
            cout << setfill('-') << setw(80) << "-" << endl;
        }
        
        
        
        text.printOptions(gameMenu);

        switch(text.getChoice(7))
        {
        case 1:
            if(current_game.c_victory || current_game.c_defeat)
            {
                cout << "game is over, please exit or load a another save" << endl;
                break;
            }
            cout << "choose one of the units" << endl;
            cout << "0 - Go back" << endl;
            current_game.display_units();
            choice = text.getChoice(current_game.c_number_of_available_units);
            if(choice == 0)
                break;

            cout << "how many: (max: 100)" << endl;
            cout << "0 - Go back" << endl;
            choice2 = text.getChoice(100);
            if(choice2 == 0)
                break;
            
            if(!current_game.choose_units(choice,choice2))
                break;


            cout << "choose entrance to place chosen unit: " << "1 - " << current_game.c_number_of_entraces << endl;
            cout << "type 0 to go back" << endl;
            choice = text.getChoice(current_game.c_number_of_entraces);
            if(choice == 0)
                break;
            current_game.place_units(choice);
            break;

        case 2:
            if(current_game.c_victory || current_game.c_defeat)
            {
                cout << "game is over, please exit or load a another save" << endl;
                break;
            }
            current_game.advanceRound(1); 
            break;

        case 3:
            if(current_game.c_victory || current_game.c_defeat)
            {
                cout << "game is over, please exit or load a another save" << endl;
                break;
            }
            cout << "type in how many rounds you wish to advance(max 100)" << endl;
            cout << "type 0 to go back" << endl;
            choice = text.getChoice(100);
            if(choice == 0)
                break;
            current_game.advanceRound(choice);
            break;
        
        case 4:
            if(current_game.c_victory || current_game.c_defeat)
            {
                cout << "game is over, please exit or load a another save" << endl;
                break;
            }
            cout << "type in new file's name:" << endl;
            newFileName = text.getName();
            cout << setfill('-') << setw(80) << "-" << endl;
            if(!specify)
                    directory_name = "assets/saves";
                else
                {
                    cout << "type in directory where you wish to save" << endl;
                    directory_name = text.getName();
                }
            load.saveGame(current_game.saveData(),newFileName,directory_name);
            break;

        case 5:
            cout << "Choose a save" << endl; 

            loadTest.resetGame();

            if(!specify)
                    directory_name = "assets/saves";
                else
                {
                    cout << "type in directory from which you wish load files" << endl;
                    directory_name = text.getName();
                }

            
            saveFile = load.chooseFile(load.fileNames(directory_name),goBack);
            if(goBack) 
            {
                goBack = false;
                break;
            }

            if(saveFile.empty())
            {
                cout << "problem opening the file,please fix the specified problem" << endl;
                cout << setfill('-') << setw(80) << "-" << endl;
                break;
            }

            fileData = load.loadFile(saveFile);

            if(fileData.empty())
            {
                cout << setfill('-') << setw(80) << "-" << endl;
                break;
            }

            if(!loadTest.loadSave(fileData))
            {
                cout << "cannot load save, aborting to current" << endl;
                break;
            }
            current_game.resetGame();
            fileData.clear();
            fileData = load.loadFile(saveFile);

            current_game.loadSave(fileData);
            fileData.clear();
            current_game.c_victory = false;
            current_game.c_defeat = false;
            break;
        
        case 6:
            if(current_game.c_victory || current_game.c_defeat)
            {
                cout << "game is over, please exit or load a another save" << endl;
                break;
            }
            if(specify)
            {
                specify = false;
                cout << "turned off" << endl;
            }
            else
            {
                specify = true;
                cout << "turned on" << endl;
            }

            break;

        case 7:
            cout << "Quiting the game" << endl; 
            return 0;

        default:
            cout << "reseting menu:" << endl;
        } 

    }

    return 0;
}