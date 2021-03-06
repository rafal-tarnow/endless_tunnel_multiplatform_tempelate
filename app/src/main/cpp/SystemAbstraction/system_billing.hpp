#pragma once

#include <string>
#include <set>
#include <list>

using namespace std;

#ifdef __ANDROID__
#   include <jni.h>
#   include <android_native_app_glue.h>
#endif

#ifdef __ANDROID__
void initPurchase(android_app * );
void uninitPurchase();
#endif

void printToast(const string message);



class BillingEventListener
{
public:
    virtual void Billing_onOwnedProductsListChanged(set<string> ownedProducts) = 0;
};


//To clear purchase
///in Terminal write: adb shell pm clear com.android.vending

class Billing
{
public:
    static set<string> listOwnedProducts();
    static void purchaseProduct(const char *product);
    static void setEventListener(BillingEventListener *);

private:

};
