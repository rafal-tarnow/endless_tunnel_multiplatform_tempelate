#pragma once

#include <map>
#include <string>

using namespace std;

class Config
{
public:
    bool loadDataFromFileToMemory(string fileName);
    bool saveDataFromMemoryToFile(string fileName);

    void set_float(string name, float value);
    void set_int32_t(string name, int32_t value);

    float get_float(string name);
    int32_t get_int32_t(string name);

private:
    map<string, float> map_float;
    map<string, int32_t> map_int32_t;
};
