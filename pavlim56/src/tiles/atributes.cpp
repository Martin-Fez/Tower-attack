#include "atributes.hpp"


Cdamage_type::Cdamage_type(const Cdamage_type & src) 
{
    c_names = src.c_names;
}


Cdamage_type Cdamage_type::operator =(const Cdamage_type & src)
{
    c_names = src.c_names;
    return *this;
}

bool Cdamage_type::operator ==(const Cdamage_type & src)
{
    for(auto & it : src.c_names)
    {
        if(c_names.find(it) != c_names.end())
        {
            return true;
        }
    }

    return false;
}


bool Cdamage_type::addElement(string new_type)
{
    if(c_names.find(new_type) != c_names.end())
    {
        return false;
    }

    c_names.emplace(new_type);

    return true;
    
}

void Cdamage_type::clearElements()
{
    c_names.clear();
}

