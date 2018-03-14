#pragma once

#include <map>
#include <string>

using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Config
{
public:
    bool loadDataFromFileToMemory(string fileName);
    bool saveDataFromMemoryToFile(string fileName);

    void set_float(string name, float value);
    void set_int32_t(string name, int32_t value);
    void set_glm_vec3(string name, glm::vec3 & value);

    float get_float(string name);
    int32_t get_int32_t(string name);
    glm::vec3 & get_glm_vec3(string name);

private:
    map<string, float> map_float;
    map<string, int32_t> map_int32_t;
    map<string, glm::vec3> map_glm_vec3;
};
