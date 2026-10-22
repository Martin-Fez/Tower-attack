#ifndef LOAD_H
#define LOAD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <filesystem>
#include "dialogueFunc.hpp"



namespace fs = std::filesystem;
using namespace std;

/**
 * @brief Class containing functions for interacting with files
 * 
 */
class LoadFile
{
    public:
    /**
     * @brief function for printing out available files and returning chosen files
     * 
     * Function prints out all available files in a given file and
     * asks the user to input which file they wish to use.
     * Depending on the choice the functions either returns back or
     * return the chosen name.
     * If the file is empty, fucntion returns an empty string
     * 
     * 
     * @param files vector of names of available files
     * @param goBack bool for returning from choice to menu
     * @return string chosen file's name
     */
    string chooseFile(vector<string> files,bool & goBack);

    /**
     * @brief fucntion for reading a regular file
     * 
     * Function is responsible for checking whether inputed file name is a regular file
     * and openable and then reads the contents of said file and line by line
     * inputs it into a list of strings(each string being a line)
     * 
     * @param fileName file's name
     * @return list<string> contents of the read file
     */
    list<string> loadFile(string fileName);

    /**
     * @brief function for reading contents of a directory
     * 
     * Function checks whether given input is a directory and
     * then inputs all of the file names within it into a vector of
     * strings
     * 
     * @param directoryName directory to be read from
     * @return vector<string> Names of files within a directory
     */
    vector<string> fileNames(string directoryName);



    /**
     * @brief Function for saving a game's data into a file
     * 
     * Functions checks whether given directory and name are valid
     * and then creates a file with given name and inputs all of
     * the contents of the list of string into the file(with one string being one line)
     * 
     * @param gameData all of a game's data within a list of strings
     * @param Savename name of the save to be created
     * @param directory directory where the save is to be created
     */
    void saveGame(list<string> gameData, string Savename,string directory);
};






#endif