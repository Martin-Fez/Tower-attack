#include "readPropertyFunc.hpp"


//will have to come back to this for when I work on loading saved games



bool readProperty(const string line,const string atributeType, int & value)
{

    string data = line.substr(line.find(atributeType)+atributeType.size());
    bool dataRead =false;
    int tempValue = 0;

    for(auto it = data.begin(); it != data.end(); it++) 
    {

        if(isblank(*it) && tempValue > 0)
            dataRead = true;

        if(isdigit(*it) && !dataRead )
        {
            if(*it == 0 && tempValue == 0)
                    dataRead=true;
            
            tempValue = tempValue*10 + *it - '0';
            continue;
        }

        if( !isblank(*it)  ) 
        {
            value = -1;
            return false;
        }   
    }

    value = tempValue;
    return true;
}

bool readProperty(const string line,const string atributeType, char & symbol)
{

    string data = line.substr(line.find(atributeType)+atributeType.size());
    bool dataRead =false;

    for(auto it = data.begin(); it != data.end(); it++) 
    {
        if(!isdigit(*it) && !isblank(*it) && !dataRead )
        {
            dataRead = true;
            symbol = *it;
            continue;
        }

        if( isdigit(*it) || (dataRead && !isblank(*it))  ) 
        {
            cout << "invalid symbol or extra characters:" << endl;
            return false;
        }   
    }
    return true;
}

bool readProperty(const string line,const string atributeType, string & sequence)
{

    string data = line.substr(line.find(atributeType)+atributeType.size());
    bool dataRead =false;

    for(auto it = data.begin(); it != data.end(); it++)
    {
        if(sequence.size() > 100)
        {
            cout << "string sequence too big" << endl;
            return false;
        }

        if(!isblank(*it) || dataRead)
        {
            dataRead = true;
            sequence.push_back(*it);
        } 

    }
    return true;
}

string readAtributeType(const string line)
{
    string name;
    for(auto it = line.begin(); it != line.end(); it++) 
    {
        if(!isblank(*it))
        {
            name.push_back(*it);
            continue;
        }

        if(!name.empty())
            break;
    }

    return name;
}