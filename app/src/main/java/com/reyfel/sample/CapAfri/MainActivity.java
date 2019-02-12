package com.reyfel.sample.CapAfri;

import android.Manifest;
import android.app.NativeActivity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.widget.Toast;
import android.support.v4.app.ActivityCompat;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.Log;
import android.content.Intent;


import com.anjlab.android.iab.v3.BillingProcessor;
import com.anjlab.android.iab.v3.SkuDetails;
import com.anjlab.android.iab.v3.TransactionDetails;

public class MainActivity extends NativeActivity {

    //BILLING
    // SAMPLE APP CONSTANTS
    private static final String ACTIVITY_NUMBER = "activity_num";
    private static final String LOG_TAG = "iabv3";
    // PRODUCT & SUBSCRIPTION IDS
    private static final String PRODUCT_ID = "com.anjlab.test.iab.s2.p5";
    private static final String SUBSCRIPTION_ID = "com.anjlab.test.iab.subs1";
    private static final String LICENSE_KEY = null; // PUT YOUR MERCHANT KEY HERE;
    // put your Google merchant id here (as stated in public profile of your Payments Merchant Center)
    // if filled library will provide protection against Freedom alike Play Market simulators
    private static final String MERCHANT_ID=null;
    // billing objects
    private BillingProcessor bp;
    private boolean readyToPurchase = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //Hide toolbar
        Context context = getApplicationContext();
        int duration = Toast.LENGTH_LONG;


        final int MY_PERMISSIONS_REQUEST_READ_CONTACTS;

        if(doesUserHavePermission())
        {
            CharSequence text = "Yes";
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        }else
        {
            CharSequence text = "No";
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();


            // Permission is not granted
           //  Should we show an explanation?
            if (ActivityCompat.shouldShowRequestPermissionRationale(MainActivity.this,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                // Show an explanation to the user *asynchronously* -- don't block
                // this thread waiting for the user's response! After the user
                // sees the explanation, try again to request the permission.
            } else {
                // No explanation needed; request the permission
               ActivityCompat.requestPermissions(MainActivity.this,
                       new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        0);

                // MY_PERMISSIONS_REQUEST_READ_CONTACTS is an
                // app-defined int constant. The callback method gets the
                // result of the request.

            }
           }

        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if(SDK_INT >= 23)
        {

        }


        //BILLING
        if(!BillingProcessor.isIabServiceAvailable(this))
        {
            showToast("In-app billing service is unavailable, please upgrade Android Market/Play to version >= 3.9.16");
        }
        else {
            bp = BillingProcessor.newBillingProcessor(this, "YOUR LICENSE KEY FROM GOOGLE PLAY CONSOLE HERE", new BillingProcessor.IBillingHandler() {
                @Override
                public void onProductPurchased(@NonNull String productId, @Nullable TransactionDetails details) {
                    showToast("onProductPurchased: " + productId);
                }
                @Override
                public void onBillingError(int errorCode, @Nullable Throwable error) {
                    showToast("onBillingError: " + Integer.toString(errorCode));
                }
                @Override
                public void onBillingInitialized() {
                    showToast("onBillingInitialized");
                    readyToPurchase = true;
                }
                @Override
                public void onPurchaseHistoryRestored() {
                    showToast("onPurchaseHistoryRestored");
                    for(String sku : bp.listOwnedProducts())
                        Log.d(LOG_TAG, "Owned Managed Product: " + sku);
                    for(String sku : bp.listOwnedSubscriptions())
                        Log.d(LOG_TAG, "Owned Subscription: " + sku);
                }
            }); // doesn't bind
            bp.initialize(); // binds
        }

    }

    @Override
    public void onDestroy() {
        if (bp != null)
            bp.release();
        super.onDestroy();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (!bp.handleActivityResult(requestCode, resultCode, data))
            super.onActivityResult(requestCode, resultCode, data);
    }


    private boolean doesUserHavePermission()
    {
        int result = getApplicationContext().checkCallingOrSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        return result == PackageManager.PERMISSION_GRANTED;
    }

    private void showToast(String message) {
        Toast.makeText(this, message, Toast.LENGTH_LONG).show();
    }
}
