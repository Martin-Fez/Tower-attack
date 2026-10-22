#ifndef ATRIBUTES_H
#define ATRIBUTES_H

#include <iostream>
#include <set>
using namespace std;

/**
 * @brief Class for managing damage types
 * 
 */
class Cdamage_type
{
    public:

        Cdamage_type() = default;

        Cdamage_type(const Cdamage_type & src);

        Cdamage_type operator =(const Cdamage_type & src);

        /**
         * @brief Overload operator for ==
         * 
         * Checks whether if one of the names within each of the container matches the other.
         * If one match is found, they equel
         * 
         * @param src Source file
         * @return true Found match
         * @return false didn't find match
         */
        bool operator ==(const Cdamage_type & src);


        /**
         * @brief Checks for existing names and adds them to the set
         * 
         * Checks if the new element is not present and adds it to the container.
         * 
         * @param new_type New name to be added
         * @return true Added
         * @return false already present
         */
        bool addElement(string new_type);

        /**
         * @brief Clears the container with names
         * 
         */
        void clearElements();

        set<string> c_names;



};


#endif
