#pragma once

#ifdef __ANDROID__

#   include <android/log.h>
#   include <android/looper.h>
#   include <cstddef>

class Loop {
public:
    void init(bool verbose) {
        looper = ALooper_forThread();
        if (looper == NULL)
            looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    }

private:
    ALooper *looper = NULL;
};

#endif