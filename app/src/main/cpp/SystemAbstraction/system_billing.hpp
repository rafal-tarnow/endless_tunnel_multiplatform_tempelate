#pragma once

#include <string>
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

void printToast(const char *message);
int callJava();
string print_dpi();


class BillingEventListener
{
    virtual void Billing_onOwnedProductsListChanged(list<string> ownedProducts);
};


class Billing
{
public:
    static list<string> listOwnedProducts();
};
