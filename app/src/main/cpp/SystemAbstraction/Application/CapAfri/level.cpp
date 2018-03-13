#include "level.hpp"
#include "system_log.hpp"
#include <errno.h>

#include "lib_config.hpp"
#include <sstream>

Level::Level()
{

}

Level::~Level()
{

}

int Level::loadLevelFromFile(string levelFilePath)
{
    FILE * mapFile = fopen(levelFilePath.c_str(), "ab+");
    if(mapFile == NULL)
    {
        int tmpErrno = errno;
        return tmpErrno;
    }


    fclose(mapFile);
    return 0;
}

int Level::saveLevelToFile(string levelFilePath)
{
    Config config;

    config.set_int32_t("ground_verticles.size()", (int32_t)ground_verticles.size());

    for(unsigned int i = 0; i < ground_verticles.size(); i++)
    {
        stringstream stream;
        stream << "ground_verticles.at(" << i << ")";
        string variable_name = stream.str() + ".x";
        config.set_float(variable_name, ground_verticles.at(i).x);
        config.set_float(stream.str() + ".y", ground_verticles.at(i).y);
        config.set_float(stream.str() + ".z", ground_verticles.at(i).z);
    }

    config.saveDataFromMemoryToFile(levelFilePath);

    return 0;
}


