#include "level.hpp"
#include "system_log.hpp"
#include <errno.h>

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
    else
    {
        fclose(mapFile);
    }
    return 0;
}

void Level::saveLevelToFile(string levelFileName)
{

}


