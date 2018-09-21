#pragma once

#include <library_opengles_2/Debug/Debug.hpp>
#include <lib_config/lib_config.hpp>
#include <cstdint>


class GameConfig{
public:
    static GameConfig * create(string fullFilePath)
    {
        gameConfig = new GameConfig(fullFilePath);
        return gameConfig;
    }
    static GameConfig * getInstance()
    {
        if(gameConfig){
            return gameConfig;
        }
    }
    static void deleteInstance()
    {
        if(gameConfig){
            delete gameConfig;
            gameConfig = nullptr;
        }
    }

    void sync()
    {
          LibConfig config;

          config.set_float("dampingRatio", dampingRatio);
          config.set_float("frequencyHz", frequencyHz);
          config.set_float("friction", friction);
          config.set_float("maxMotorTorque", maxMotorTorque);
          config.set_float("simFPS", simFPS);
          config.set_uint32_t("coins", coins);
          config.set_uint32_t("currentMapIndex", currentMapIndex);
          config.set_glm_vec4("clearColour", clearColour);
          config.set_bool("debugDrawFlag", debugDrawFlag);
          config.set_bool("showFPS", showFPS);
          config.set_bool("skipBackgroundDraw", skipBackgroundDraw);
          config.set_bool("useAverageSimFPS", useAverageSimFPS);
          config.set_bool("useCurrentSimFPS", useCurrentSimFPS);
          config.set_bool("useFixedSimFPS", useFixedSimFPS);
          config.set_bool("waitForGlFinish", waitForGlFinish);

          config.saveDataFromMemoryToFile(mFullFilePath);
    }

    float dampingRatio = 0.7;
    float frequencyHz = 0.7;
    float friction = 100;
    float maxMotorTorque = 100;
    float simFPS = 60;
    uint32_t coins = 0;
    uint32_t currentMapIndex = 0;
    glm::vec4 clearColour = glm::vec4(0, 0, 0, 0);
    bool debugDrawFlag = false;
    bool showFPS = false;
    bool skipBackgroundDraw = false;
    bool useAverageSimFPS = false;
    bool useCurrentSimFPS = true;
    bool useFixedSimFPS = false;
    bool waitForGlFinish = false;

protected:
    GameConfig(string fullFilePath)
    {
        mFullFilePath = fullFilePath;
        LibConfig config;
        config.loadDataFromFileToMemory(mFullFilePath);

        dampingRatio = config.get_float("dampingRatio");
        frequencyHz = config.get_float("frequencyHz");
        friction = config.get_float("friction");
        maxMotorTorque = config.get_float("maxMotorTorque");
        simFPS = config.get_float("simFPS");
        coins = config.get_uint32_t("coins");
        currentMapIndex = config.get_uint32_t("currentMapIndex");
        clearColour = config.get_glm_vec4("clearColour");
        debugDrawFlag = config.get_bool("debugDrawFlag");
        showFPS = config.get_bool("showFPS");
        skipBackgroundDraw = config.get_bool("skipBackgroundDraw");
        useAverageSimFPS = config.get_bool("useAverageSimFPS");
        useCurrentSimFPS = config.get_bool("useCurrentSimFPS");
        useFixedSimFPS = config.get_bool("useFixedSimFPS");
        waitForGlFinish = config.get_bool("waitForGlFinish");

    }

    ~GameConfig()
    {
        sync();
    }

private:
    static GameConfig *gameConfig;
    string mFullFilePath;
    DBG_COUNT("GameConfig");
};
