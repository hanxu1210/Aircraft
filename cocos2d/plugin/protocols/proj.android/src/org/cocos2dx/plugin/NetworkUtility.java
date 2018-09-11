package org.cocos2dx.plugin;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;

public class NetworkUtility {
    private static final String LOG_TAG = "Network";
    static public boolean networkReachable(Activity activity) {
        boolean bRet = false;
        try {
            ConnectivityManager conn = (ConnectivityManager)activity.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo netInfo = conn.getActiveNetworkInfo();
            bRet = (null == netInfo) ? false : netInfo.isAvailable();
        } catch (Exception e) {
            Log.e(LOG_TAG, "Fail to check network status", e);
        }
        Log.d(LOG_TAG, "NetWork reachable : " + bRet);
        return bRet;
    } 
}
