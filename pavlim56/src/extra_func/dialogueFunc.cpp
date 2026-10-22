#include "dialogueFunc.hpp"

void TEXT::printOptions (int dialogue)
{

    switch(dialogue)
    {
    case 0:
        cout << "Do you wish to load a game or start a new?" << endl;
        cout << "Type the number to choose an option:" << endl;
        cout << "1 - Start a new game" << endl;
        cout << "2 - Load a save" << endl;
        cout << "3 - Quit the game" << endl;
        cout << setfill('-') << setw(80) << "-" << endl;
        break;

    case 1:
        cout << setfill('-') << setw(80) << "-" << endl;
        cout << "1 - Select units" << endl;
        cout << "2 - Advance the game by 1 round" << endl;
        cout << "3 - Advance the game by custom amount of rounds" << endl;
        cout << "4 - Save the game" << endl;
        cout << "5 - Load a saved game" << endl;
        cout << "6 - Turn on/off manual folder specificiation" << endl;
        cout << "7 - Quit the game" << endl;
        cout << setfill('-') << setw(80) << "-" << endl;
        break;

    default:
        cout << "dialogue error" << endl;
        return;
        
    } 

}

int TEXT::getChoice (int numOfOptions) 
{
    string buffer;
    int iResult = 0;
    bool dataRead = false;
    bool whiteSpacesOnly = true;

    while(true)
    {
        getline(cin,buffer);
        cout << setfill('-') << setw(80) << "-" << endl;
        for(auto it = buffer.begin(); it != buffer.end(); it++)
        {
            char temp = *it;
            
            if(isblank(temp) && iResult > 0)
                dataRead = true;

            if(isdigit(temp) && !dataRead) 
            {
                if(temp == 0 && iResult == 0)
                    dataRead=true;

                iResult = iResult*10 + *it - '0';
                continue;
            }

            if( !isblank(temp)  )
            {
                whiteSpacesOnly = false;
                break;
            }
        }

        if(!whiteSpacesOnly || iResult > numOfOptions || iResult < 0)
        {
            cout << "input correct number:" << endl;
            buffer.clear();
            iResult = 0;
            dataRead = false;
            whiteSpacesOnly = true;
            continue;
        }
        
        break;
    }

    return iResult;

}

string TEXT::getName()
{
    string buffer;
    while(1)
    {
        getline(cin,buffer);
        if(buffer.size() > 100)
        {
            cout << "file name too long" << endl;
            buffer.clear();
        }
        else    
            break;
    }
    
    changeFormat(buffer);
    return buffer;
}

void TEXT::changeFormat(string & buffer)
{
    size_t position = buffer.find_first_not_of(' ');
    if(position == string::npos )
    {
        cout << "given blank" << endl;
        buffer.clear();
        return;
    }

    buffer = buffer.substr(position);
    cout << buffer << endl;
}
