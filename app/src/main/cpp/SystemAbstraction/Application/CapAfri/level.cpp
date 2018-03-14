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
    Config config;
    config.loadDataFromFileToMemory(levelFilePath);

    int32_t map_size = config.get_int32_t("ground_verticles.size()");

    for(unsigned int i = 0; i < map_size; i++)
    {
        stringstream stream;
        stream << "ground_verticles.at(" << i << ")";

        ground_verticles.push_back(config.get_glm_vec3(stream.str()));
    }

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

        config.set_glm_vec3(stream.str(), ground_verticles.at(i));
    }

    config.saveDataFromMemoryToFile(levelFilePath);

    return 0;
}

void Level::clear()
{
    ground_verticles.clear();
}


