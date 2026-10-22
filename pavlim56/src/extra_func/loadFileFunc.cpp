#include "loadFileFunc.hpp"


list<string> LoadFile::loadFile(string fileName)
{
    string line;
    list<string> Vfile;
    ifstream myfile (fileName);

    if(!fs::is_regular_file(fileName))
    {
        cout << "Is not a regular file" << endl;
        return Vfile;
    }

    if (myfile.is_open())
    {
        while( getline(myfile,line) )
        {
            if(!line.empty() && isblank(line.back()))
            {
                line.pop_back();
            }

            Vfile.push_back(line);  
        }
        myfile.close();
    }
    else 
        cout << "error opening file" << endl;

    return Vfile;
}


vector<string> LoadFile::fileNames(const string directoryName) 
{
    vector<string> files;
    string filePath;
    if(!fs::is_directory(directoryName))
    {
        cout << "directory does not exist" << endl;
        cout << setfill('-') << setw(80) << "-" << endl;
        return files;
    }

    for (const auto & entry : fs::directory_iterator(directoryName))
    {
        filePath = entry.path().string();
        files.push_back(filePath);
    }

    return files;
}

string LoadFile::chooseFile(vector<string> files, bool & goBack) 
{
    TEXT text;
    string dummy;
    int i = 0;
    size_t found;
    for(auto & it : files)
    {
        found = it.find_last_of("/");
        cout << ++i << " - " << it.substr(found+1) << endl;
    }

    if(i == 0)
    {
        cout << "There are no files available" << endl;
        cout << setfill('-') << setw(80) << "-" << endl;
        return dummy;
    }

    cout << "0 - return back" << endl;
    int j = text.getChoice(i);

    if(j == 0)
    {
        goBack = true;
        return dummy;
    }

    cout << files[j-1] << endl;
    cout << setfill('-') << setw(80) << "-" << endl;

    return files[j-1]; 

}

void LoadFile::saveGame(list<string> gameData, string Savename,string directory)
{
    string fullDirectory = directory + "/";

    if(!fs::is_directory(directory))
    {
        cout << "specified directory does not exist" << endl;
        return;
    }

    for (const auto & entry : fs::directory_iterator(directory))
    {
        if(entry.path().string() == (fullDirectory + Savename))
        {
            cout << "file name already present, could not save" << endl;
            return;
        }
    }

    ofstream outfile;
    outfile.open(fullDirectory + Savename);

    for(auto & line : gameData)
    {
        outfile << line << endl;
    }

    outfile.close();

    return;
}
