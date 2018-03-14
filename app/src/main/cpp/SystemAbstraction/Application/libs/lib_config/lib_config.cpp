#include "lib_config.hpp"
#include <sstream>
#include <fstream>

#include <glm/gtx/string_cast.hpp>
#include <iostream>


bool Config::loadDataFromFileToMemory(string fileName)
{
    //CLEAR DATAS
    map_float.clear();
    map_int32_t.clear();
    map_glm_vec3.clear();

    //OPEN FILE
    std::ifstream infile(fileName.c_str());

    string type;
    string name;
    string equal;
    string value_1;
    string value_2;
    string value_3;

#warning "DOROBIC OBSLUGE BLEDU PARSOWANIA PLIKU"

    //READ FILE LINE BY LINE
    while (infile >> type)
    {
        if(type == "float")
        {
            infile >> name >> equal >> value_1;

            cout << "----------------------" << endl;
            cout << "type = " << type << endl;
            cout << "name = " << name << endl;
            cout << "equal = " << equal << endl;
            cout << "value_1 = " << value_1 << endl;
            cout << "value_2 = " << value_2 << endl;
            cout << "value_3 = " << value_3 << endl;

            map_float[name] = stof(value_1);
        }
        else if(type == "int32_t")
        {
            infile >> name >> equal >> value_1;

            cout << "----------------------" << endl;
            cout << "type = " << type << endl;
            cout << "name = " << name << endl;
            cout << "equal = " << equal << endl;
            cout << "value_1 = " << value_1 << endl;
            cout << "value_2 = " << value_2 << endl;
            cout << "value_3 = " << value_3 << endl;

            map_int32_t[name] = stoi(value_1);
        }
        else if(type == "glm::vec3")
        {
            infile >> name >> equal >> value_1 >> value_2 >> value_3;

//            cout << "----------------------" << endl;
//            cout << "type = " << type << endl;
//            cout << "name = " << name << endl;
//            cout << "equal = " << equal << endl;
//            cout << "value_1 = " << value_1 << endl;
//            cout << "value_2 = " << value_2 << endl;
//            cout << "value_3 = " << value_3 << endl;

            glm::vec3 value(stof(value_1),stof(value_2),stof(value_3));
            map_glm_vec3[name] = value;
        }
    }
}

bool Config::saveDataFromMemoryToFile(string fileName)
{
    //OPEN FILE
    std::ofstream onfile(fileName.c_str());
    onfile.precision(100);

    //SAVE FLOATS
    std::map<std::string, float>::iterator it_float = map_float.begin();
    while (it_float != map_float.end())
    {
        std::string name = it_float->first;
        float value = it_float->second;
        onfile << "float " << name << " = " << value << endl;
        it_float++;
    }

    //SAVE INT32_T
    std::map<std::string, int32_t>::iterator it_int32_t = map_int32_t.begin();
    while (it_int32_t != map_int32_t.end())
    {
        std::string name = it_int32_t->first;
        int32_t value = it_int32_t->second;
        onfile << "int32_t " << name << " = " << value << endl;
        it_int32_t++;
    }

    //SAVE GLM_VEC3
    std::map<std::string, glm::vec3>::iterator it_glm_vec3 = map_glm_vec3.begin();
    while (it_glm_vec3 != map_glm_vec3.end())
    {
        std::string name = it_glm_vec3->first;
        glm::vec3 value = it_glm_vec3->second;
        onfile << "glm::vec3 " << name << " = " << value.x << " " << value.y << " " << value.z << endl;
        it_glm_vec3++;
    }

    //CLOSE FILE
    onfile.close();
}

void Config::set_float(string name, float value)
{
    map_float[name] = value;
}

void Config::set_int32_t(string name, int32_t value)
{
    map_int32_t[name] = value;
}

void Config::set_glm_vec3(string name, glm::vec3 & value)
{
    map_glm_vec3[name] = value;
}

float Config::get_float(string name)
{
    return map_float[name];
}

glm::vec3 & Config::get_glm_vec3(string name)
{
    return map_glm_vec3[name];
}

int32_t Config::get_int32_t(string name)
{
    return map_int32_t[name];
}

