#include "lib_config.hpp"
#include <sstream>
#include <fstream>

#include <glm/gtx/string_cast.hpp>
#include <iostream>


bool LibConfig::loadDataFromMemoryToMemory(char * data, int size)
{


    stringstream stringStream;

    stringStream.write((const char*) data, size);

    loadFromInstream(stringStream);

    return true;
}

bool LibConfig::loadDataFromFileToMemory(string fileName)
{

    //OPEN FILE
    std::ifstream instream(fileName.c_str());
    if(instream.fail())
    {
        return false;
    }


    loadFromInstream(instream);

    //close stream
    instream.close();
    return true;
}

bool LibConfig::loadFromInstream(istream & instream)
{
    //CLEAR DATAS
    map_float.clear();
    map_int32_t.clear();
    map_uint32_t.clear();
    map_glm_vec3.clear();

    string type;
    string name;
    string equal;
    string value_1;
    string value_2;
    string value_3;
    string value_4;

#warning "DOROBIC OBSLUGE BLEDU PARSOWANIA PLIKU"

    //READ FILE LINE BY LINE

    while (instream >> type)
    {
        if(type == "float")
        {
            instream >> name >> equal >> value_1;

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
            instream >> name >> equal >> value_1;

            cout << "----------------------" << endl;
            cout << "type = " << type << endl;
            cout << "name = " << name << endl;
            cout << "equal = " << equal << endl;
            cout << "value_1 = " << value_1 << endl;
            cout << "value_2 = " << value_2 << endl;
            cout << "value_3 = " << value_3 << endl;

            map_int32_t[name] = stoi(value_1);
        }
        else if(type == "uint32_t")
        {
            instream >> name >> equal >> value_1;

            cout << "----------------------" << endl;
            cout << "type = " << type << endl;
            cout << "name = " << name << endl;
            cout << "equal = " << equal << endl;
            cout << "value_1 = " << value_1 << endl;
            cout << "value_2 = " << value_2 << endl;
            cout << "value_3 = " << value_3 << endl;

            map_uint32_t[name] = uint32_t(stoul(value_1));
        }
        else if(type == "bool")
        {
            instream >> name >> equal >> value_1;

            cout << "----------------------" << endl;
            cout << "type = " << type << endl;
            cout << "name = " << name << endl;
            cout << "equal = " << equal << endl;
            cout << "value_1 = " << value_1 << endl;
            cout << "value_2 = " << value_2 << endl;
            cout << "value_3 = " << value_3 << endl;

            map_bool[name] = bool(stoi(value_1));;
        }
        else if(type == "glm::vec3")
        {
            instream >> name >> equal >> value_1 >> value_2 >> value_3;

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
        else if(type == "glm::vec4")
        {
            instream >> name >> equal >> value_1 >> value_2 >> value_3 >> value_4;

            //            cout << "----------------------" << endl;
            //            cout << "type = " << type << endl;
            //            cout << "name = " << name << endl;
            //            cout << "equal = " << equal << endl;
            //            cout << "value_1 = " << value_1 << endl;
            //            cout << "value_2 = " << value_2 << endl;
            //            cout << "value_3 = " << value_3 << endl;

            glm::vec4 value(stof(value_1),stof(value_2),stof(value_3),stof(value_4));
            map_glm_vec4[name] = value;
        }
    }
    return true;
}

bool LibConfig::saveDataFromMemoryToFile(string fileName)
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

    //SAVE UINT32_T
    std::map<std::string, uint32_t>::iterator it_uint32_t = map_uint32_t.begin();
    while (it_uint32_t != map_uint32_t.end())
    {
        std::string name = it_uint32_t->first;
        uint32_t value = it_uint32_t->second;
        onfile << "uint32_t " << name << " = " << value << endl;
        it_uint32_t++;
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

    //SAVE GLM_VEC4
    std::map<std::string, glm::vec4>::iterator it_glm_vec4 = map_glm_vec4.begin();
    while (it_glm_vec4 != map_glm_vec4.end())
    {
        std::string name = it_glm_vec4->first;
        glm::vec4 value = it_glm_vec4->second;
        onfile << "glm::vec4 " << name << " = " << value.x << " " << value.y << " " << value.z << " " << value.w << endl;
        it_glm_vec4++;
    }

    //SAVE BOOL
    std::map<std::string, bool>::iterator it_bool = map_bool.begin();
    while (it_bool != map_bool.end())
    {
        std::string name = it_bool->first;
        bool value = it_bool->second;
        onfile << "bool " << name << " = " << value << endl;
        it_bool++;
    }

    //CLOSE FILE
    onfile.flush();
    onfile.close();

    return true;
}

void LibConfig::set_float(string name, float value)
{
    map_float[name] = value;
}

void LibConfig::set_int32_t(string name, int32_t value)
{
    map_int32_t[name] = value;
}
void LibConfig::set_uint32_t(string name, uint32_t value)
{
    map_uint32_t[name] = value;
}

void LibConfig::set_glm_vec3(string name, glm::vec3 & value)
{
    map_glm_vec3[name] = value;
}

void LibConfig::set_glm_vec4(string name, glm::vec4 & value)
{
    map_glm_vec4[name] = value;
}

void LibConfig::set_bool(string name, bool value)
{
    map_bool[name] = value;
}

bool LibConfig::get_float(string name, float &value)
{
    if(map_float.count(name))
    {
        value =  map_float[name];
        return true;
    }else{
        return false;
    }
}

bool LibConfig::get_glm_vec3(string name, glm::vec3 & value)
{
    if(map_glm_vec3.count(name))
    {
        value =  map_glm_vec3[name];
        return true;
    }else{
        return false;
    }
}

bool LibConfig::get_glm_vec4(string name, glm::vec4 & value)
{
    if(map_glm_vec4.count(name))
    {
        value =  map_glm_vec4[name];
        return true;
    }else{
        return false;
    }
}

bool LibConfig::get_int32_t(string name, int32_t & value)
{
    if(map_int32_t.count(name))
    {
        value =  map_int32_t[name];
        return true;
    }else{
        return false;
    }
}

bool LibConfig::get_uint32_t(string name, uint32_t & value)
{
    if(map_uint32_t.count(name))
    {
        value =  map_uint32_t[name];
        return true;
    }else{
        return false;
    }
}

bool LibConfig::get_bool(string name, bool & value)
{
    if(map_bool.count(name))
    {
        value =  map_bool[name];
        return true;
    }else{
        return false;
    }
}

