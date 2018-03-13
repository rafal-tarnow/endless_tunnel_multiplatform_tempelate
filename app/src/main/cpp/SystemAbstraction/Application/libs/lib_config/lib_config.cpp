#include "lib_config.hpp"
#include <sstream>
#include <fstream>

bool Config::loadDataFromFileToMemory(string fileName)
{
    //CLEAR DATAS
    map_float.clear();
    map_int32_t.clear();

    //OPEN FILE
    std::ifstream infile(fileName.c_str());

    string type;
    string name;
    string equal;
    string value;

    //READ FILE LINE BY LINE
    while (infile >> type >> name >> equal >> value)
    {
        if(type == "float")
        {
            map_float[name] = stof(value);
        }
        else if(type == "int32_t")
        {
            map_int32_t[name] = stoi(value);
        }
    }
}

bool Config::saveDataFromMemoryToFile(string fileName)
{
    //OPEN FILE
    std::ofstream onfile(fileName.c_str());

    //SAVE FLOATS
    std::map<std::string, float>::iterator it_float = map_float.begin();
    while (it_float != map_float.end())
    {
        std::string name = it_float->first;
        float value = it_float->second;
        onfile << "float " << name << " = " << value << endl;
        it_float++;
    }

    //SAVE INT32_T
    std::map<std::string, int32_t>::iterator it_int32_t = map_int32_t.begin();
    while (it_int32_t != map_int32_t.end())
    {
        std::string name = it_int32_t->first;
        int32_t value = it_int32_t->second;
        onfile << "int32_t " << name << " = " << value << endl;
        it_int32_t++;
    }

    //CLOSE FILE
    onfile.close();
}

void Config::set_float(string name, float value)
{
    map_float[name] = value;
}

void Config::set_int32_t(string name, int32_t value)
{
    map_int32_t[name] = value;
}

float Config::get_float(string name)
{
    return map_float[name];
}

int32_t Config::get_int32_t(string name)
{
    return map_int32_t[name];
}

