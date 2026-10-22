#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <string>
#include <iomanip>

using namespace std;

enum dialogue {startMenu,gameMenu};


class TEXT
{
    public:


/**
 * @brief prints avaiable options
 * 
 * @param dialogue enum of dialogue we want to use
 */
void printOptions (const int dialogue);


/**
 * @brief function for reading player's numerical input
 * 
 * Reads input a checks that an a input is a number
 * and checks if it is within the allowed interval of
 * 0 - numOfOptions
 * The function keeps asking for a number till a valid one is given
 * 
 * @param numOfOptions maximum allowed number
 * @return int inputed number
 */
int getChoice (const int numOfOptions);

/**
 * @brief function for reading any string input
 * 
 * Function reads any string input up to 100 characters long
 * 
 * 
 * 
 * @return string read string from input
 */
string getName();

/**
 * @brief function for checking format of inputed string
 * 
 * Function cuts down blank spaces before first character
 * and returns an empty string if nothing but blank is inputed
 * 
 * @param buffer 
 */
void changeFormat(string & buffer);
};


#endif
