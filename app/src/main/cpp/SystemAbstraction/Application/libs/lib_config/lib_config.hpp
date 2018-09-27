#pragma once

#include <map>
#include <string>
#include <istream>

using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <library_opengles_2/Debug/Debug.hpp>

class LibConfig
{
public:
    bool loadDataFromMemoryToMemory(char * data, int size);
    bool loadDataFromFileToMemory(string fileName);
    bool saveDataFromMemoryToFile(string fileName);

    void set_float(string name, float value);
    void set_int32_t(string name, int32_t value);
    void set_uint32_t(string name, uint32_t value);
    void set_glm_vec3(string name, glm::vec3 & value);
    void set_glm_vec4(string name, glm::vec4 & value);
    void set_bool(string name, bool value);

    bool get_float(string name, float &value);
    bool get_int32_t(string name, int32_t &value);
    bool get_uint32_t(string name, uint32_t &value);
    bool get_glm_vec3(string name, glm::vec3 & value);
    bool get_glm_vec4(string name, glm::vec4 & value);
    bool get_bool(string name, bool & value);

private:
    bool loadFromInstream(istream & stream);
    map<string, float> map_float;
    map<string, int32_t> map_int32_t;
    map<string, uint32_t> map_uint32_t;
    map<string, glm::vec3> map_glm_vec3;
    map<string, glm::vec4> map_glm_vec4;
    map<string, bool> map_bool;
    DBG_COUNT("Config");
};
