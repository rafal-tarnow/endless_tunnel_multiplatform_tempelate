#include "system_billing.hpp"


#ifdef __ANDROID__
static android_app * androidApp;

void initPurchase(android_app * app)
{
    androidApp = app;
}

void purchase(const char* message) {
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

void purchase(const char* message) {

}

#endif
