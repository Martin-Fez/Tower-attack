#ifndef READPROPERTYFUNC_H
#define READPROPERTYFUNC_H

#include <iostream>
#include <string>

using namespace std;

        /**
         * @brief Reads what comes after the requested atribute and returns it as a int
         * 
         * Function shortens the line using the first inputed word.
         * After that searches the line a positive numerical value.
         * If it finds more than one or a non digit it returns false.
         * 
         * @param line from which to read the value
         * @param atributeType atribute's name to ignore
         * @param value Value to be returned
         * @return true Success and returned a valid value
         * @return false Failed to read
         */
        bool readProperty(const string line,const string atributeType, int & value); 

        /**
         * @brief Reads what comes after the requested atribute and returns it as a char
         * 
         * Function shortens the line using the first inputed word.
         * After that searches the line a single character.
         * If it finds more than one non blank character it returns false.
         * 
         * @param line from which to read the character
         * @param atributeType atribute's name to ignore
         * @param symbol Character to be returned
         * @return true Success and returned a valid character
         * @return false Failed to read
         */
        bool readProperty(const string line,const string atributeType, char & symbol);

        /**
         * @brief Reads what comes after the requested atribute and returns it as a string
         * 
         * Function shortens the line using the first inputed word.
         * After that it searches for the first non blank character and after it reads the entire rest of the line as a string.
         * If the the string is longer than 100 character it returns false.
         * 
         * @param line from which to read the string
         * @param atributeType atribute's name to ignore
         * @param sequence String to be returned
         * @return true Success and returned a valid string
         * @return false Failed to read
         */
        bool readProperty(const string line,const string atributeType, string & sequence);

        /**
         * @brief reads first sequence of non blank characters
         * 
         * Reads the inputed line and finds the firs sequence of characters(in other words the first word)
         * 
         * @param line Inputed line
         * @return string first sequence of characters
         */
        string readAtributeType(const string line);




#endif
