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
   clear();
}

int Level::loadLevelFromFile(string levelFilePath)
{
    this->clear();

    Config config;
    config.loadDataFromFileToMemory(levelFilePath);

    int32_t map_size = config.get_int32_t("ground_verticles.size()");
    for(int i = 0; i < map_size; i++)
    {
        stringstream stream;
        stream << "ground_verticles.at(" << i << ")";

        ground_verticles.push_back(config.get_glm_vec3(stream.str()));
    }

    int32_t vector_size = config.get_int32_t("coins_vector.size()");

    for(int i = 0; i < vector_size; i++)
    {
        stringstream stream;
        stream << "coins_vector.at(" << i << ")";
        glm::vec3 coin_position = config.get_glm_vec3(stream.str());

        coins_vector.push_back(new CircleCoinRender(coin_position.x, coin_position.y, -2.0f, 0.25f));
    }

    int32_t mushroom_vector_size = config.get_int32_t("mushroom_vector.size()");
    for(int i = 0; i < mushroom_vector_size; i++)
    {
        stringstream stream;
        stream << "mushroom_vector.at(" << i << ")";
        glm::vec3 mushroom_position = config.get_glm_vec3(stream.str());
        glm::vec2 dimm(2.0, 2.0);
        mushroom_vector.push_back(new MushroomRenderer(mushroom_position));
    }

    glm::vec3 metaPosition = config.get_glm_vec3("metaPosition");
    meta = new MetaRenderer(metaPosition, 1.5);

    background_image_index = config.get_uint32_t("background_image_index");

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

    config.set_int32_t("coins_vector.size()",(int32_t)coins_vector.size());
    for(unsigned int i = 0; i < coins_vector.size(); i++)
    {
        stringstream stream;
        stream << "coins_vector.at(" << i << ")";

        config.set_glm_vec3(stream.str(), coins_vector.at(i)->getPosition());
    }

    config.set_int32_t("mushroom_vector.size()", (int32_t)mushroom_vector.size());
    for(unsigned int i = 0; i < mushroom_vector.size(); i++)
    {
        stringstream stream;
        stream << "mushroom_vector.at(" << i << ")";

        config.set_glm_vec3(stream.str(), mushroom_vector.at(i)->getPosition());
    }

    config.set_uint32_t("background_image_index", background_image_index);

    config.set_glm_vec3("metaPosition", meta->getPosition());

    config.saveDataFromMemoryToFile(levelFilePath);

    return 0;
}

void Level::clear()
{
    ground_verticles.clear();

    for (auto &coin : coins_vector) // access by reference to avoid copying
    {
        delete coin;
    }
    coins_vector.clear();

    for(auto & mushroom : mushroom_vector)
    {
        delete mushroom;
    }
    mushroom_vector.clear();

    if(meta != nullptr)
        delete meta;
}


