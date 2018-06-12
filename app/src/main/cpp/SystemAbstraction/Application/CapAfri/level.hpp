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
    int loadLevelFromFile(string levelFilePath);
    int saveLevelToFile(string levelFileName);
    void clear();

    vector<glm::vec3> ground_verticles;
    vector<CircleCoinRender *> coins_vector;
    vector<MushroomRenderer *> mushroom_vector;
    MetaRenderer * meta = nullptr;
private:
    Config config;
};
