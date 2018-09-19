#pragma once


#ifdef __ANDROID__
#   include <android/log.h>
#else
#   include <cstdio>
#endif

#include <streambuf>

#define DEBUG_TAG "CapAfri"

#ifdef __ANDROID__
#   define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, __VA_ARGS__))
#   define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))
#   define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, DEBUG_TAG, __VA_ARGS__))
#   define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, __VA_ARGS__))
#else
#   define LOGD(...) (printf(__VA_ARGS__))
#   define LOGI(...) (printf(__VA_ARGS__))
#   define LOGW(...) (printf(__VA_ARGS__))
#   define LOGE(...) (printf(__VA_ARGS__))
#endif


#define ABORT_GAME { LOGE("*** GAME ABORTING."); *((volatile char*)0) = 'a'; }
#define DEBUG_BLIP LOGD("[ BLIP ]: %s:%d", __FILE__, __LINE__)

#define MY_ASSERT(cond) { if (!(cond)) { LOGE("ASSERTION FAILED: %s", #cond); \
    ABORT_GAME; } }



#ifdef __ANDROID__
class androidbuf: public std::streambuf {
public:
    enum { bufsize = 128 }; // ... or some other suitable buffer size
    androidbuf() { this->setp(buffer, buffer + bufsize - 1); }
private:
    int overflow(int c) {
        if (c == traits_type::eof()) {
            *this->pptr() = traits_type::to_char_type(c);
            this->sbumpc();
        }
        return this->sync()? traits_type::eof(): traits_type::not_eof(c);
    }
    int sync() {
        int rc = 0;
        if (this->pbase() != this->pptr()) {
            __android_log_print(ANDROID_LOG_INFO,
                               "Native",
                               "%s",
                               std::string(this->pbase(),
                                           this->pptr() - this->pbase()).c_str());
            rc = 0;
            this->setp(buffer, buffer + bufsize - 1);
        }
        return rc;
    }
    char buffer[bufsize];
};
#endif
