/*
 * Copyright (C) Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef endlesstunnel_common_hpp
#define endlesstunnel_common_hpp


extern "C" {
#   include <EGL/egl.h>
#   include <GLES2/gl2.h>
#   include <errno.h>
#   include <unistd.h>
#   include <stdlib.h>
#ifdef __ANDROID__
#   include <jni.h>
#   include <android/sensor.h>
#   include <android_native_app_glue.h>
#endif
}
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


#define BUFFER_OFFSET(i) ((char*)NULL + (i))

#include "our_key_codes.hpp"

#endif

