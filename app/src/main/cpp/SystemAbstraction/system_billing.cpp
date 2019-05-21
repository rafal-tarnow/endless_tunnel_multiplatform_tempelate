#include "system_billing.hpp"
#include <sstream>
#include <library_api/cunixdatagramsocket.h>
#include <SystemAbstraction/system_log.hpp>

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
CUnixDatagramSocket * unixSocket_5 = nullptr;


#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <set>

using namespace std;



std::set<std::string> split(const std::string& s, char delimiter)
{
    std::set<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.insert(token);
    }
    return tokens;
}

void standaloneDataFromUnixSocket_5()
{
    std::vector<char> buffer;
    unixSocket_5->readDatagram(&buffer);

    string text(buffer.data());

    set<string> products = split(text,'|');

    string txt;
    for(auto it = products.begin(); it != products.end(); it++)
    {
        txt.append(*it);
        txt.append(" ");
    }
    printToast("NDK callback Products(): " + txt);
}

void initPurchase(android_app * app)
{
    androidApp = app;

    unixSocket_5 = new CUnixDatagramSocket();
    unixSocket_5->connect<&standaloneDataFromUnixSocket_5>();
    unixSocket_5->Bind("\0to_com_reyfel_system_billing");
}

void uninitPurchase()
{
    delete unixSocket_5;
}



int callJava()
{
    JNIEnv* jni;
    return 15;
}

void Billing::purchaseProduct(const char *product) {
    JNIEnv* jni;
    androidApp->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(androidApp->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "purchaseProduct", "(Ljava/lang/String;)V");
    jstring jmessage = jni->NewStringUTF(product);
    jni->CallVoidMethod(androidApp->activity->clazz, methodID, jmessage);
    jni->DeleteLocalRef(jmessage);

    androidApp->activity->vm->DetachCurrentThread();
}

set<string> Billing::listOwnedProducts()
{
    JNIEnv* jni;
    android_app* app = androidApp;
    app->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass activityClass = jni->GetObjectClass(app->activity->clazz);
    //jclass activityClass = jni->FindClass("com/reyfel/sample/CapAfri/MainActivity");
    //jclass activityClass = jni->FindClass("android/app/NativeActivity");

    JNI_ASSERT(jni, activityClass);

    //GET STRING ARRAY
    jmethodID getStringArray = jni->GetMethodID(activityClass, "getStringArray", "()[Ljava/lang/String;");
    JNI_ASSERT(jni, getStringArray);

    jobjectArray stringArraryObject = (jobjectArray)jni->CallObjectMethod(app->activity->clazz,getStringArray);
    JNI_ASSERT(jni, stringArraryObject);

    jsize stringArrarySize = jni->GetArrayLength(stringArraryObject);
    JNI_ASSERT(jni, true);


    set<string> products;

    for (int i=0; i<stringArrarySize; i++)
    {
        string item;
        jstring string = (jstring)jni->GetObjectArrayElement(stringArraryObject, i);
        JNI_ASSERT(jni, string);

        const char* text = jni->GetStringUTFChars(string, nullptr);

        item.append(text, strlen(text));
        products.insert(item);

        jni->ReleaseStringUTFChars(string, text);
    }

    // TODO: Delete objects here.

    app->activity->vm->DetachCurrentThread();

    return products;
}

void printToast(const string message)
{
    JNIEnv* jni;
    androidApp->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(androidApp->activity->clazz);

    jmethodID methodID = jni->GetMethodID(clazz, "showAlert", "(Ljava/lang/String;)V");

    jstring jmessage = jni->NewStringUTF(message.c_str());

    jni->CallVoidMethod(androidApp->activity->clazz, methodID, jmessage);

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
