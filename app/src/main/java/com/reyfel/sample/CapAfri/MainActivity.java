package com.reyfel.sample.CapAfri;

import android.Manifest;
import android.app.NativeActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.widget.Toast;
import android.support.v4.app.ActivityCompat;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.Log;
import android.content.Intent;

import java.net.DatagramSocket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.List;
import java.util.concurrent.Semaphore;


import com.anjlab.android.iab.v3.BillingProcessor;
import com.anjlab.android.iab.v3.SkuDetails;
import com.anjlab.android.iab.v3.TransactionDetails;

public class MainActivity extends NativeActivity implements BillingProcessor.IBillingHandler{

    //SOCKET
    private DatagramSocket socket;
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

        showToast(new Integer(stringFromJNI()).toString());

        //BILLING -- na koncu onCreate zeby uniknac race condition
        bp = BillingProcessor.newBillingProcessor(this, null, this); // doesn't bind
        bp.initialize();
    }

    @Override
    public void onDestroy() {
        if (bp != null)
            bp.release();
        super.onDestroy();
    }

    private boolean doesUserHavePermission()
    {
        int result = getApplicationContext().checkCallingOrSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        return result == PackageManager.PERMISSION_GRANTED;
    }

    private void showToast(String message) {
        Toast.makeText(this, message, Toast.LENGTH_LONG).show();
    }


    // This function will be called from C++ by name and signature
    public void showAlert(final String message)
    {
        final MainActivity activity = this;

        this.runOnUiThread(new Runnable()
        {
            public void run()
            {
                showToast(message);
            }
        });

    }

    // This function will be called from C++ by name and signature
    public void purchaseProduct(final String productId)
    {
        final MainActivity activity = this;

        this.runOnUiThread(new Runnable() {
            public void run()
            {
                showToast("Want buy " + productId);
                boolean isOneTimePurchaseSupported = bp.isOneTimePurchaseSupported();
                if(isOneTimePurchaseSupported)
                {
                    showToast("bp.purchase(" + productId + ")");
                    bp.purchase(activity, productId);
                }
            }
        });

    }

    //BILLING
    @Override
    public void onProductPurchased(@NonNull String productId, @Nullable TransactionDetails details) {
        Toast.makeText(this, "You've purchased something!", Toast.LENGTH_LONG).show();

        String[] products = bp.listOwnedProducts().toArray(new String[0]);
        sendCommand(products);
    }

    @Override
    public void onPurchaseHistoryRestored() {

    }

    @Override
    public void onBillingError(int errorCode, @Nullable Throwable error) {
        Toast.makeText(this, "Somthing were wrong!", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onBillingInitialized() {

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (!bp.handleActivityResult(requestCode, resultCode, data)) {
            super.onActivityResult(requestCode, resultCode, data);
        }
    }

    public int getValue()
    {
        return 12345;
    }

    public String getString()
    {
        return "To jest string z Javy przeslany do C++ i z powrotem do Javy!";
    }

    public String[] getStringArray()
    {
        String[] products = bp.listOwnedProducts().toArray(new String[0]);
        return products;

//        String[] s = new String[4];
//        s[0] = "dupa ";
//        s[1] = "dupa ";
//        s[2] = "zyzol ";
//        s[3] = "zyzol ";
//        return s;

    }

    public void getPurchases()
    {
        bp.listOwnedProducts();
    }

    public native /*String*/ int  stringFromJNI();
    public native int sendCommand(String[] pStringArray);

    static {
        System.loadLibrary("hello-jni");
    }

}
