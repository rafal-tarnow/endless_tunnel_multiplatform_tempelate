#pragma once

#include <vector>
#include <string>

using namespace std;

#include "./Bodies/coin_circle_body.hpp"
#include <lib_config.hpp>
#include "./Bodies/coin_circle_body.hpp"
#include "./Bodies/mushroom.hpp"
#include "./Bodies/meta_body.hpp"


class Level{

public:
    Level();
    ~Level();

    /* if error while procceding file function return file open() errno value */
    int loadLevelFromMemory(unsigned char * data, int size);
    int loadLevelFromFile(string levelFilePath);
    int saveLevelToFile(string levelFileName);
    void clear();

    vector<glm::vec3> ground_verticles;
    vector<CircleCoinRender *> coins_vector;
    vector<MushroomRenderer *> mushroom_vector;
    MetaRenderer * meta = nullptr;
    uint32_t background_image_index = 0;
    uint32_t ground_image_index = 0;
    glm::vec3 cameraOffset = glm::vec3(0,0,0);
    glm::vec4 groundContourColor = glm::vec4(0.9686, 0.7137, 0.0941, 1.0);
private:
    int loadLevel(Config & config);
};
