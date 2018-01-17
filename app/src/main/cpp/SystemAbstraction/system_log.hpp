#pragma once

extern "C" {
#ifdef __ANDROID__
#   include <android/log.h>
#else
#   include <stdio.h>
#endif
}

#define DEBUG_TAG "EndlessTunnel:Native"

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
