#include "system_paths.hpp"
#include "system_log.hpp"

#ifdef __ANDROID__
#include <stdlib.h>
#endif

string getStandardCommonReadWriteDirecory()
{
#if   defined(__ANDROID__)
    string text = getenv("EXTERNAL_STORAGE");
    return text;
#else// defined(__linux__) //&& !defined(__ANDROID__)
    string text_home = getenv("HOME");
    //string text_user = getenv("USER");
    return "/" + text_home + "/.config";
#endif
}


string mConfigPath;

void systemInput_initConfigPath(string configPath)
{
    mConfigPath = configPath;
}

string getAppConfigFilePath()
{
    if(mConfigPath.empty())
    {
        LOGD("ERROR Config path must be inited by system before use!");
        exit(EXIT_FAILURE);
    } else
    {
        return mConfigPath;
    }
}
