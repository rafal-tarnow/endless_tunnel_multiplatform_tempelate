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
#include "SystemAbstraction/Application/Tunnel/common.hpp"
#include "input_util.hpp"
#include "SystemAbstraction/Application/Tunnel/joystick-support.hpp"
#include "SystemAbstraction/Application/Tunnel/scene_manager.hpp"
#include "SystemAbstraction/Application/Tunnel/welcome_scene.hpp"
#include "native_engine.hpp"
#include "./SystemAbstraction/system_abstraction.hpp"
#include "./SystemAbstraction/system_log.hpp"
#include "./SystemAbstraction/system_paths.hpp"
#include "./SystemAbstraction/OpenSLWrap.hpp"
#include "./SystemAbstraction/Application/libs/library_opengles_2/Resources/Resources.hpp"

// verbose debug logs on?
#define VERBOSE_LOGGING 0

#if VERBOSE_LOGGING
    #define VLOGD LOGD
#else
    #define VLOGD
#endif

// max # of GL errors to print before giving up
#define MAX_GL_ERRORS 200


android_app * androidApp;
void showAlert(const char* message) {
    JNIEnv* jni;
    androidApp->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(androidApp->activity->clazz);

    // Get the ID of the method we want to call
    // This must match the name and signature from the Java side
    // Signature has to match java implementation (second string hints a t a java string parameter)
    jmethodID methodID = jni->GetMethodID(clazz, "showAlert", "(Ljava/lang/String;)V");

    // Strings passed to the function need to be converted to a java string object
    jstring jmessage = jni->NewStringUTF(message);

    jni->CallVoidMethod(androidApp->activity->clazz, methodID, jmessage);

    // Remember to clean up passed values
    jni->DeleteLocalRef(jmessage);

    androidApp->activity->vm->DetachCurrentThread();
}

static NativeEngine *_singleton = NULL;

NativeEngine::NativeEngine(struct android_app *app) {
    androidApp = app;
    LOGD("NativeEngine: initializing.");
    mApp = app;
    mHasFocus = mIsVisible = mHasWindow = false;
    mHasGLObjects = false;
    mEglDisplay = EGL_NO_DISPLAY;
    mEglSurface = EGL_NO_SURFACE;
    mEglContext = EGL_NO_CONTEXT;
    mEglConfig = 0;
    mSurfWidth = mSurfHeight = 0;
    mApiVersion = 0;
    mJniEnv = NULL;
    memset(&mState, 0, sizeof(mState));
    mIsFirstFrame = true;

    if (app->savedState != NULL) {
        // we are starting with previously saved state -- restore it
        mState = *(struct NativeEngineSavedState*) app->savedState;
    }

    // only one instance of NativeEngine may exist!
    MY_ASSERT(_singleton == NULL);
    _singleton = this;

    VLOGD("NativeEngine: querying API level.");
    LOGD("NativeEngine: API version %d.", mApiVersion);

    systemInput_initConfigPath(app->activity->internalDataPath);

    if(!AudioManager::GetSingletonPtr()) {
        new AudioManager(app->activity->assetManager);
    }
    Resource::initAndroid(app->activity->assetManager);
}

NativeEngine* NativeEngine::GetInstance() {
    MY_ASSERT(_singleton != NULL);
    return _singleton;
}

NativeEngine::~NativeEngine() {
    VLOGD("NativeEngine: destructor running");
    KillContext();
    if (mJniEnv) {
        LOGD("Detaching current thread from JNI.");
        mApp->activity->vm->DetachCurrentThread();
        LOGD("Current thread detached from JNI.");
        mJniEnv = NULL;
    }
    _singleton = NULL;
}

static void _handle_cmd_proxy(struct android_app* app, int32_t cmd) {
    NativeEngine *engine = (NativeEngine*) app->userData;
    engine->HandleCommand(cmd);
}

static int _handle_input_proxy(struct android_app* app, AInputEvent* event) {
    NativeEngine *engine = (NativeEngine*) app->userData;
    return engine->HandleInput(event) ? 1 : 0;
}

bool NativeEngine::IsAnimating() {
    return mHasFocus && mIsVisible && mHasWindow;
}

void NativeEngine::GameLoop() {
    mApp->userData = this;
    mApp->onAppCmd = _handle_cmd_proxy;
    mApp->onInputEvent = _handle_input_proxy;

    while (1) {
        int ident, events;
        struct android_poll_source* source;

        // If not animating, block until we get an event; if animating, don't block.
        while ((ident = ALooper_pollAll(IsAnimating() ? 0 : -1, NULL, &events, 
                (void**)&source)) >= 0) {

            // process event
            if (source != NULL) {
                source->process(mApp, source);
            }

            // are we exiting?
            if (mApp->destroyRequested) {
                return;
            }
        }

        if (IsAnimating()) {
            DoFrame();
        }
    }
}

JNIEnv* NativeEngine::GetJniEnv() {
    if (!mJniEnv) {
        LOGD("Attaching current thread to JNI.");
        if (0 != mApp->activity->vm->AttachCurrentThread(&mJniEnv, NULL)) {
            LOGE("*** FATAL ERROR: Failed to attach thread to JNI.");
            ABORT_GAME;
        }
        MY_ASSERT(mJniEnv != NULL);
        LOGD("Attached current thread to JNI, %p", mJniEnv);
    }

    return mJniEnv;
}


void NativeEngine::HandleCommand(int32_t cmd) {

    VLOGD("NativeEngine: handling command %d.", cmd);
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.
            VLOGD("NativeEngine: APP_CMD_SAVE_STATE");
            mApp->savedState = malloc(sizeof(mState));
            *((NativeEngineSavedState*)mApp->savedState) = mState;
            mApp->savedStateSize = sizeof(mState);
            break;
        case APP_CMD_INIT_WINDOW:
            // We have a window!
            VLOGD("NativeEngine: APP_CMD_INIT_WINDOW");
            if (mApp->window != NULL) {
                mHasWindow = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is going away -- kill the surface
            VLOGD("NativeEngine: APP_CMD_TERM_WINDOW");
            KillSurface();
            mHasWindow = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            VLOGD("NativeEngine: APP_CMD_GAINED_FOCUS");
            mHasFocus = true;
            break;
        case APP_CMD_LOST_FOCUS:
            VLOGD("NativeEngine: APP_CMD_LOST_FOCUS");
            mHasFocus = false;
            break;
        case APP_CMD_PAUSE:
            VLOGD("NativeEngine: APP_CMD_PAUSE");
            SystemAbstraction::onPause();
            break;
        case APP_CMD_RESUME:
            VLOGD("NativeEngine: APP_CMD_RESUME");
            SystemAbstraction::onResume();
            break;
        case APP_CMD_STOP:
            VLOGD("NativeEngine: APP_CMD_STOP");
            mIsVisible = false;
            break;
        case APP_CMD_START:
            VLOGD("NativeEngine: APP_CMD_START");
            mIsVisible = true;
            break;
        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONFIG_CHANGED:
            VLOGD("NativeEngine: %s", cmd == APP_CMD_WINDOW_RESIZED ?
                    "APP_CMD_WINDOW_RESIZED" : "APP_CMD_CONFIG_CHANGED");
            // Window was resized or some other configuration changed.
            // Note: we don't handle this event because we check the surface dimensions
            // every frame, so that's how we know it was resized. If you are NOT doing that,
            // then you need to handle this event!
            break;
        case APP_CMD_LOW_MEMORY:
            VLOGD("NativeEngine: APP_CMD_LOW_MEMORY");
            // system told us we have low memory. So if we are not visible, let's
            // cooperate by deallocating all of our graphic resources.
            if (!mHasWindow) {
                VLOGD("NativeEngine: trimming memory footprint (deleting GL objects).");
                KillGLObjects();
            }
            break;
        default:
            VLOGD("NativeEngine: (unknown command).");
            break;
    }

    VLOGD("NativeEngine: STATUS: F%d, V%d, W%d, EGL: D %p, S %p, CTX %p, CFG %p",
            mHasFocus, mIsVisible, mHasWindow, mEglDisplay, mEglSurface, mEglContext, mEglConfig);
}

static bool _cooked_event_callback(struct CookedEvent *event) {
    PointerCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.x = event->motionX;
    coords.y = event->motionY;
    coords.minX = event->motionMinX;
    coords.maxX = event->motionMaxX;
    coords.minY = event->motionMinY;
    coords.maxY = event->motionMaxY;
    coords.isScreen = event->motionIsOnScreen;

    switch (event->type) {
        case COOKED_EVENT_TYPE_JOY:
            SystemAbstraction::onJoyUpdate(event->joyX, event->joyY);
            return true;
        case COOKED_EVENT_TYPE_POINTER_DOWN:
            SystemAbstraction::onPointerDown(event->motionPointerId, &coords);
            showAlert("Pointer down!");
            return true;
        case COOKED_EVENT_TYPE_POINTER_UP:
            SystemAbstraction::onPointerUp(event->motionPointerId, &coords);
            return true;
        case COOKED_EVENT_TYPE_POINTER_MOVE:
            LOGD("case COOKED_EVENT_TYPE_POINTER_MOVE:");
            SystemAbstraction::onPointerMove(event->motionPointerId, &coords);
            return true;
        case COOKED_EVENT_TYPE_KEY_DOWN:
            SystemAbstraction::onKey(SystemAbstraction::EVENT_DOWN, (SystemAbstraction::Key)event->keyCode,SystemAbstraction::MOD_NONE,0,0);
            return true;
        case COOKED_EVENT_TYPE_KEY_UP:
            SystemAbstraction::onKey(SystemAbstraction::EVENT_UP, (SystemAbstraction::Key)event->keyCode,SystemAbstraction::MOD_NONE, 0,0);
            return true;
        case COOKED_EVENT_TYPE_BACK:
            return SystemAbstraction::onBackKeyPressed();
    }

    return false;
}

bool NativeEngine::HandleInput(AInputEvent *event) {
    return CookEvent(event, _cooked_event_callback) ? 1 : 0;
}

bool NativeEngine::InitDisplay() {
    if (mEglDisplay != EGL_NO_DISPLAY) {
        // nothing to do
        LOGD("NativeEngine: no need to init display (already had one).");
        return true;
    }

    LOGD("NativeEngine: initializing display.");
    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_FALSE == eglInitialize(mEglDisplay, 0, 0)) {
        LOGE("NativeEngine: failed to init display, error %d", eglGetError());
        return false;
    }
    return true;
}

bool NativeEngine::InitSurface() {
    // need a display
    MY_ASSERT(mEglDisplay != EGL_NO_DISPLAY);

    if (mEglSurface != EGL_NO_SURFACE) {
        // nothing to do
        LOGD("NativeEngine: no need to init surface (already had one).");
        return true;
    }
        
    LOGD("NativeEngine: initializing surface.");
    
    EGLint numConfigs;

    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // request OpenGL ES 2.0
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };

    // since this is a simple sample, we have a trivial selection process. We pick
    // the first EGLConfig that matches:
    eglChooseConfig(mEglDisplay, attribs, &mEglConfig, 1, &numConfigs);

    // create EGL surface
    mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, mApp->window, NULL);
    if (mEglSurface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface, EGL error %d", eglGetError());
        return false;
    }

    LOGD("NativeEngine: successfully initialized surface.");
    return true;
}

bool NativeEngine::InitContext() {
    // need a display
    MY_ASSERT(mEglDisplay != EGL_NO_DISPLAY);

    EGLint attribList[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE }; // OpenGL 2.0

    if (mEglContext != EGL_NO_CONTEXT) {
        // nothing to do
        LOGD("NativeEngine: no need to init context (already had one).");
        return true;
    }
        
    LOGD("NativeEngine: initializing context.");

    // create EGL context
    mEglContext = eglCreateContext(mEglDisplay, mEglConfig, NULL, attribList);
    if (mEglContext == EGL_NO_CONTEXT) {
        LOGE("Failed to create EGL context, EGL error %d", eglGetError());
        return false;
    }

    LOGD("NativeEngine: successfull initialized context.");

    return true;
}

void NativeEngine::ConfigureOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}


bool NativeEngine::PrepareToRender() {
    do {
        // if we're missing a surface, context, or display, create them
        if (mEglDisplay == EGL_NO_DISPLAY || mEglSurface == EGL_NO_SURFACE || 
                mEglContext == EGL_NO_CONTEXT) {

            // create display if needed
            if (!InitDisplay()) {
                LOGE("NativeEngine: failed to create display.");
                return false;
            }

            // create surface if needed
            if (!InitSurface()) {
                LOGE("NativeEngine: failed to create surface.");
                return false;
            }

            // create context if needed
            if (!InitContext()) {
                LOGE("NativeEngine: failed to create context.");
                return false;
            }

            LOGD("NativeEngine: binding surface and context (display %p, surface %p, context %p)", 
                    mEglDisplay, mEglSurface, mEglContext);

            // bind them
            if (EGL_FALSE == eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
                LOGE("NativeEngine: eglMakeCurrent failed, EGL error %d", eglGetError());
                HandleEglError(eglGetError());
            }

            // configure our global OpenGL settings
            ConfigureOpenGL();
        }

        // now that we're sure we have a context and all, if we don't have the OpenGL 
        // objects ready, create them.
        if (!mHasGLObjects) {
            LOGD("NativeEngine: creating OpenGL objects.");
            if (!InitGLObjects()) {
                LOGE("NativeEngine: unable to initialize OpenGL objects.");
                return false;
            }
        }
    } while(0);

    // ready to render
    return true;
}

void NativeEngine::KillGLObjects() {
    if (mHasGLObjects) {
        SystemAbstraction::onUninit();
        mHasGLObjects = false;
    }
}

void NativeEngine::KillSurface() {
    LOGD("NativeEngine: killing surface.");
    eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (mEglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(mEglDisplay, mEglSurface);
        mEglSurface = EGL_NO_SURFACE;
    }
    LOGD("NativeEngine: Surface killed successfully.");
}

void NativeEngine::KillContext() {
    LOGD("NativeEngine: killing context.");

    // since the context is going away, we have to kill the GL objects
    KillGLObjects();

    eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (mEglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(mEglDisplay, mEglContext);
        mEglContext = EGL_NO_CONTEXT;
    }
    LOGD("NativeEngine: Context killed successfully.");
}

void NativeEngine::KillDisplay() {
    // causes context and surface to go away too, if they are there
    LOGD("NativeEngine: killing display.");
    KillContext();
    KillSurface();

    if (mEglDisplay != EGL_NO_DISPLAY) {
        LOGD("NativeEngine: terminating display now.");
        eglTerminate(mEglDisplay);
        mEglDisplay = EGL_NO_DISPLAY;
    }
    LOGD("NativeEngine: display killed successfully.");
}

bool NativeEngine::HandleEglError(EGLint error) {
    switch (error) {
        case EGL_SUCCESS:
            // nothing to do
            return true;
        case EGL_CONTEXT_LOST:
            LOGW("NativeEngine: egl error: EGL_CONTEXT_LOST. Recreating context.");
            KillContext();
            return true;
        case EGL_BAD_CONTEXT:
            LOGW("NativeEngine: egl error: EGL_BAD_CONTEXT. Recreating context.");
            KillContext();
            return true;
        case EGL_BAD_DISPLAY:
            LOGW("NativeEngine: egl error: EGL_BAD_DISPLAY. Recreating display.");
            KillDisplay();
            return true;
        case EGL_BAD_SURFACE:
            LOGW("NativeEngine: egl error: EGL_BAD_SURFACE. Recreating display.");
            KillSurface();
            return true;
        default:
            LOGW("NativeEngine: unknown egl error: %d", error);
            return false;
    }
}

static void _log_opengl_error(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:
            LOGE("*** OpenGL error: GL_NO_ERROR");
            break;
        case GL_INVALID_ENUM:
            LOGE("*** OpenGL error: GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            LOGE("*** OpenGL error: GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
            LOGE("*** OpenGL error: GL_INVALID_OPERATION");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            LOGE("*** OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION");
            break;
        case GL_OUT_OF_MEMORY:
            LOGE("*** OpenGL error: GL_OUT_OF_MEMORY");
            break;
        default:
            LOGE("*** OpenGL error: error %d", err);
            break;
    }
}


void NativeEngine::DoFrame() {
    // prepare to render (create context, surfaces, etc, if needed)
    if (!PrepareToRender()) {
        // not ready
        VLOGD("NativeEngine: preparation to render failed.");
        return;
    }


    // how big is the surface? We query every frame because it's cheap, and some
    // strange devices out there change the surface size without calling any callbacks...
    int width, height;
    eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &width);
    eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &height);

    if (width != mSurfWidth || height != mSurfHeight) {
        // notify scene manager that the surface has changed size
        LOGD("NativeEngine: surface changed size %dx%d --> %dx%d", mSurfWidth, mSurfHeight, 
                width, height);
        mSurfWidth = width;
        mSurfHeight = height;
        SystemAbstraction::onFramebufferResize(mSurfWidth, mSurfHeight);

    }

    // if this is the first frame, install the welcome scene
    if (mIsFirstFrame) {
        mIsFirstFrame = false;
        SystemAbstraction::onRenderFirstFrame();
    }
    
    // render!
    SystemAbstraction::onRenderFrame();

    // swap buffers
    if (EGL_FALSE == eglSwapBuffers(mEglDisplay, mEglSurface)) {
        // failed to swap buffers... 
        LOGW("NativeEngine: eglSwapBuffers failed, EGL error %d", eglGetError());
        HandleEglError(eglGetError());
    }

    // print out GL errors, if any
    GLenum e;
    static int errorsPrinted = 0;
    while ((e = glGetError()) != GL_NO_ERROR) {
        if (errorsPrinted < MAX_GL_ERRORS) {
            _log_opengl_error(e);
            ++errorsPrinted;
            if (errorsPrinted >= MAX_GL_ERRORS) {
                LOGE("*** NativeEngine: TOO MANY OPENGL ERRORS. NO LONGER PRINTING.");
            }
        }
    }
}

android_app* NativeEngine::GetAndroidApp() {
    return mApp;
}

bool NativeEngine::InitGLObjects()
{
    if (!mHasGLObjects)
    {
        int width, height;
        width = height = 0;

        if (mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE)
        {
            eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &width);
            eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &height);

            if (width != mSurfWidth || height != mSurfHeight)
            {
                mSurfWidth = width;
                mSurfHeight = height;
            }
        }

        SystemAbstraction::onInit(mSurfWidth, mSurfHeight);
        _log_opengl_error(glGetError());
        mHasGLObjects = true;
    }
    return true;
}

