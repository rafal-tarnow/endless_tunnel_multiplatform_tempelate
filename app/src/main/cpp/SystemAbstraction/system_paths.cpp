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
#elif defined(__linux__) //&& !defined(__ANDROID__)
    string text = getenv("HOME");
    return text;
#endif
}
