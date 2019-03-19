#include "system_billing.hpp"
#include <sstream>

using namespace std;


#ifdef __ANDROID__

#define JNI_ASSERT(jni, cond) { \
  if (!(cond)) {\
    std::stringstream ss; \
    ss << __FILE__ << ":" << __LINE__; \
    throw std::runtime_error(ss.str()); \
  } \
  if (jni->ExceptionCheck()) { \
    std::stringstream ss; \
    ss << __FILE__ << ":" << __LINE__; \
    throw std::runtime_error("Exception: " + ss.str()); \
  } \
}


static android_app * androidApp;

void initPurchase(android_app * app)
{
    androidApp = app;
}



int callJava()
{
    JNIEnv* jni;
    return 15;
}


string print_dpi() {
    JNIEnv* jni;
    int width;
    int height;
    android_app* app = androidApp;
    app->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass activityClass = jni->GetObjectClass(app->activity->clazz);
    //jclass activityClass = jni->FindClass("com/reyfel/sample/CapAfri/MainActivity");
    //jclass activityClass = jni->FindClass("android/app/NativeActivity");

    JNI_ASSERT(jni, activityClass);

    ////////////////////////////////////////

    jmethodID getWindowManager = jni->GetMethodID
            ( activityClass
                    , "getWindowManager"
                    , "()Landroid/view/WindowManager;");
    JNI_ASSERT(jni, getWindowManager);

    jobject wm = jni->CallObjectMethod(app->activity->clazz, getWindowManager);
    JNI_ASSERT(jni, wm);

    jclass windowManagerClass = jni->FindClass("android/view/WindowManager");
    JNI_ASSERT(jni, windowManagerClass);

    jmethodID getDefaultDisplay = jni->GetMethodID( windowManagerClass
            , "getDefaultDisplay"
            , "()Landroid/view/Display;");
    JNI_ASSERT(jni, getDefaultDisplay);

    jobject display = jni->CallObjectMethod(wm, getDefaultDisplay);
    JNI_ASSERT(jni, display);

    jclass displayClass = jni->FindClass("android/view/Display");
    JNI_ASSERT(jni, displayClass);

    // Check if everything is OK so far, it is, the values it prints
    // are sensible.
    {
        jmethodID getWidth = jni->GetMethodID(displayClass, "getWidth", "()I");
        JNI_ASSERT(jni, getWidth);

        jmethodID getHeight = jni->GetMethodID(displayClass, "getHeight", "()I");
        JNI_ASSERT(jni, getHeight);

        width = jni->CallIntMethod(display, getWidth);
        JNI_ASSERT(jni, true);
        //log("Width: ", width); // Width: 320

        height = jni->CallIntMethod(display, getHeight);
        JNI_ASSERT(jni, true);
        //log("Height: ", height); // Height: 480
    }

    jclass displayMetricsClass = jni->FindClass("android/util/DisplayMetrics");
    JNI_ASSERT(jni, displayMetricsClass);

    jmethodID displayMetricsConstructor = jni->GetMethodID( displayMetricsClass
            , "<init>", "()V");
    JNI_ASSERT(jni, displayMetricsConstructor);

    jobject displayMetrics = jni->NewObject( displayMetricsClass
            , displayMetricsConstructor);
    JNI_ASSERT(jni, displayMetrics);

    jmethodID getMetrics = jni->GetMethodID( displayClass
            , "getMetrics"
            , "(Landroid/util/DisplayMetrics;)V");
    JNI_ASSERT(jni, getMetrics);

    jni->CallVoidMethod(display, getMetrics, displayMetrics);
    JNI_ASSERT(jni, true);

    {
        jfieldID xdpi_id = jni->GetFieldID(displayMetricsClass, "xdpi", "F");
        JNI_ASSERT(jni, xdpi_id);

        float xdpi = jni->GetFloatField(displayMetrics, xdpi_id);
        JNI_ASSERT(jni, true);

        //log("XDPI: ", xdpi); // XDPI: 0
    }

    {
        jfieldID height_id = jni->GetFieldID( displayMetricsClass
                , "heightPixels", "I");
        JNI_ASSERT(jni, height_id);

        int height = jni->GetIntField(displayMetrics, height_id);
        JNI_ASSERT(jni, true);

        //log("Height: ", height); // Height: 0
    }


    //GET VALUE
    jmethodID getValue = jni->GetMethodID(activityClass, "getValue", "()I");
    JNI_ASSERT(jni, getValue);

    width = jni->CallIntMethod(app->activity->clazz, getValue);
    JNI_ASSERT(jni, true);

    //GET STRING
    jmethodID getString = jni->GetMethodID(activityClass, "getString", "()Ljava/lang/String;");
    JNI_ASSERT(jni, getString);

    jstring joStringPropVal  = (jstring)jni->CallObjectMethod(app->activity->clazz,getString);

    const char* jcVal = jni->GetStringUTFChars(joStringPropVal, nullptr);

    string napis;
    napis.assign(jcVal, strlen(jcVal));

    jni->ReleaseStringUTFChars(joStringPropVal, jcVal);

    //GET STRING ARRAY
    jmethodID getStringArray = jni->GetMethodID(activityClass, "getStringArray", "()[Ljava/lang/String;");
    JNI_ASSERT(jni, getStringArray);

    jobjectArray stringArraryObject = (jobjectArray)jni->CallObjectMethod(app->activity->clazz,getStringArray);
    JNI_ASSERT(jni, stringArraryObject);

    jsize stringArrarySize = jni->GetArrayLength(stringArraryObject);
    JNI_ASSERT(jni, true);

    string tekst;

    for (int i=0; i<stringArrarySize; i++)
    {
        jstring string = (jstring)jni->GetObjectArrayElement(stringArraryObject, i);
        JNI_ASSERT(jni, string);

        const char* text = jni->GetStringUTFChars(string, nullptr);

        tekst.append(text, strlen(text));
        tekst.append(" | ");

        jni->ReleaseStringUTFChars(string, text);
    }

    // TODO: Delete objects here.
    jni->DeleteLocalRef(displayMetrics);

    app->activity->vm->DetachCurrentThread();

    return tekst;
}


//int print_dpi() {
//    JNIEnv* jni;
//    int width;
//    int height;
//    android_app* app = androidApp;
//    app->activity->vm->AttachCurrentThread(&jni, NULL);
//
//    jclass activityClass = jni->GetObjectClass(app->activity->clazz);
//    //jclass activityClass = jni->FindClass("com/reyfel/sample/CapAfri/MainActivity");
//    //jclass activityClass = jni->FindClass("android/app/NativeActivity");
//
//    JNI_ASSERT(jni, activityClass);
//
//    jmethodID getWindowManager = jni->GetMethodID
//            ( activityClass
//                    , "getWindowManager"
//                    , "()Landroid/view/WindowManager;");
//    JNI_ASSERT(jni, getWindowManager);
//
//    jobject wm = jni->CallObjectMethod(app->activity->clazz, getWindowManager);
//    JNI_ASSERT(jni, wm);
//
//    jclass windowManagerClass = jni->FindClass("android/view/WindowManager");
//    JNI_ASSERT(jni, windowManagerClass);
//
//    jmethodID getDefaultDisplay = jni->GetMethodID( windowManagerClass
//            , "getDefaultDisplay"
//            , "()Landroid/view/Display;");
//    JNI_ASSERT(jni, getDefaultDisplay);
//
//    jobject display = jni->CallObjectMethod(wm, getDefaultDisplay);
//    JNI_ASSERT(jni, display);
//
//    jclass displayClass = jni->FindClass("android/view/Display");
//    JNI_ASSERT(jni, displayClass);
//
//    // Check if everything is OK so far, it is, the values it prints
//    // are sensible.
//    {
//        jmethodID getWidth = jni->GetMethodID(displayClass, "getWidth", "()I");
//        JNI_ASSERT(jni, getWidth);
//
//        jmethodID getHeight = jni->GetMethodID(displayClass, "getHeight", "()I");
//        JNI_ASSERT(jni, getHeight);
//
//        width = jni->CallIntMethod(display, getWidth);
//        JNI_ASSERT(jni, true);
//        //log("Width: ", width); // Width: 320
//
//        height = jni->CallIntMethod(display, getHeight);
//        JNI_ASSERT(jni, true);
//        //log("Height: ", height); // Height: 480
//    }
//
//    jclass displayMetricsClass = jni->FindClass("android/util/DisplayMetrics");
//    JNI_ASSERT(jni, displayMetricsClass);
//
//    jmethodID displayMetricsConstructor = jni->GetMethodID( displayMetricsClass
//            , "<init>", "()V");
//    JNI_ASSERT(jni, displayMetricsConstructor);
//
//    jobject displayMetrics = jni->NewObject( displayMetricsClass
//            , displayMetricsConstructor);
//    JNI_ASSERT(jni, displayMetrics);
//
//    jmethodID getMetrics = jni->GetMethodID( displayClass
//            , "getMetrics"
//            , "(Landroid/util/DisplayMetrics;)V");
//    JNI_ASSERT(jni, getMetrics);
//
//    jni->CallVoidMethod(display, getMetrics, displayMetrics);
//    JNI_ASSERT(jni, true);
//
//    {
//        jfieldID xdpi_id = jni->GetFieldID(displayMetricsClass, "xdpi", "F");
//        JNI_ASSERT(jni, xdpi_id);
//
//        float xdpi = jni->GetFloatField(displayMetrics, xdpi_id);
//        JNI_ASSERT(jni, true);
//
//        //log("XDPI: ", xdpi); // XDPI: 0
//    }
//
//    {
//        jfieldID height_id = jni->GetFieldID( displayMetricsClass
//                , "heightPixels", "I");
//        JNI_ASSERT(jni, height_id);
//
//        int height = jni->GetIntField(displayMetrics, height_id);
//        JNI_ASSERT(jni, true);
//
//        //log("Height: ", height); // Height: 0
//    }
//    // TODO: Delete objects here.
//    jni->DeleteLocalRef(displayMetrics);
//    app->activity->vm->DetachCurrentThread();
//
//    return width;
//}

void printToast(const char *message) {
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
#else

void purchase(const char* message)
{

}

void callJava()
{

}

#endif
