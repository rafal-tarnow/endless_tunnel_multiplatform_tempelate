package com.reyfel.sample.CapAfri;

import android.Manifest;
import android.app.NativeActivity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.widget.Toast;
import android.support.v4.app.ActivityCompat;



public class MainActivity extends NativeActivity {

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


    }

    private boolean doesUserHavePermission()
    {
        int result = getApplicationContext().checkCallingOrSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        return result == PackageManager.PERMISSION_GRANTED;
    }
}
