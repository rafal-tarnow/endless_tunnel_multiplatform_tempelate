#pragma once

#include <string>

using namespace std;

typedef enum{
    OK,
    NO_EXTERNAL_TORAGE
}Errors;


//On android return sdcard path or if sdcard is remove return internal memory path
//On PC return user home path
string getStandardCommonReadWriteDirecory();


void systemInput_initConfigPath(string configPath);
string getAppConfigFilePath();