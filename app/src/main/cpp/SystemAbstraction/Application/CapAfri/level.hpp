#pragma once

#include <vector>
#include <string>

using namespace std;

#include "./Bodies/coin_circle_body.hpp"
#include <lib_config.hpp>
#include "./Bodies/coin_circle_body.hpp"

class Level{

public:
    Level();
    ~Level();

    /* if error while procceding file function return file open() errno value */
    int loadLevelFromFile(string levelFilePath);
    int saveLevelToFile(string levelFileName);
    void clear();
    vector<CircleCoin *> coinsVector;

    vector<glm::vec3> ground_verticles;
    vector<CircleCoinRender *> coins_vector;
private:
    Config config;
};
