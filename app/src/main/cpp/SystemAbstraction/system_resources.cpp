#include "system_resources.hpp"

#ifdef __ANDROID__
AAssetManager* Resource::m_pAssetManager;

void Resource::initAndroid(AAssetManager*	pAssetManager)
{
    m_pAssetManager = pAssetManager;
}
#endif

Resource::Resource(string path)
{
#ifdef __ANDROID__
    AAsset* asset=AAssetManager_open(m_pAssetManager,path.c_str(),AASSET_MODE_BUFFER);
    size = AAsset_getLength(asset);
    data = (unsigned char *)malloc(size);
    AAsset_read(asset, data, size);
    AAsset_close(asset);
#endif
}

Resource::~Resource()
{
    free(data);
    data = NULL;
    size = 0;
}

unsigned char * Resource::getData()
{
    return data;
}
uint32_t Resource::getSize()
{
    return size;
}
