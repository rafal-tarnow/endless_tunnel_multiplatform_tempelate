#pragma once


#ifdef __ANDROID__
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android/asset_manager.h>
#endif

#include <unordered_map>
#include <string>

//********** SINGLETON ********************
#include <cassert>

    template <typename T>
    class Singleton
    {
    private:
        static T*	ms_singleton;

    public:
        Singleton()
        {
            assert(!ms_singleton);
            ms_singleton = static_cast<T*>(this);
        }

        ~Singleton()
        {
            assert(ms_singleton);
            ms_singleton = 0;
        }

        static T& GetSingleton()		{ return *ms_singleton; }
        static T* GetSingletonPtr()	{ return ms_singleton; }
    };

    template <typename T> T* Singleton<T>::ms_singleton = 0;

//*******************************************

class AudioManager
        :	public Singleton<AudioManager>
{
public:
    typedef unsigned int		AudioHandle;
    static const AudioHandle	INVALID_HANDLE	= 0xFFFFFFFF;

private:
    #ifdef __ANDROID__
    SLObjectItf					m_engineObject;
    SLEngineItf					m_engine;
    SLObjectItf 				m_outputMixObject;

    AAssetManager*				m_pAssetManager;

    struct PlayInstance
    {
        SLObjectItf 		m_playerObject;
        SLPlayItf			m_playerPlay;
    };
    typedef std::unordered_map<AudioHandle, PlayInstance*>		PlayInstanceMap;
    typedef PlayInstanceMap::iterator								PlayInstanceMapIterator;

    PlayInstanceMap				m_playInstances;
#endif

public:
    explicit AudioManager(/*AAssetManager* pAssetManager*/);
    virtual ~AudioManager();

    bool	Initialize();
    void	Destroy();

    void empty();

    AudioHandle	CreateSFX(std::string& filename);
    void		PlaySFX(AudioHandle handle);
    void		DestroySFX(AudioHandle handle);
};
