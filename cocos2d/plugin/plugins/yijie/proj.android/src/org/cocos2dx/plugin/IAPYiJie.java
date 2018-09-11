package org.cocos2dx.plugin;

import java.util.Hashtable;

import com.snowfish.cn.ganga.offline.basic.SFActionCallback;
import com.snowfish.cn.ganga.offline.basic.SFNativeAdapter;
import com.snowfish.cn.ganga.offline.helper.SFCommonSDKInterface;

import android.app.Activity;
import android.content.Context;
import android.util.Log;


public class IAPYiJie implements InterfaceIAP {
    private static final String LOG_TAG = "IAPYiJie";
    private static Activity mContext = null;
    protected static boolean bDebug = false;
    
    protected static void LogE(String msg, Exception e) {
        Log.e(LOG_TAG, msg, e);
        e.printStackTrace();
    }

    private static boolean isDebug = true;
    protected static void LogD(String msg) {
        if (isDebug) {
            Log.d(LOG_TAG, msg);
        }
    }
    
    public IAPYiJie(Context context) {
        mContext = (Activity)context;
    } 
    
    @Override
    public void configDeveloperInfo(Hashtable<String, String> cpInfo)
    {
        LogD("Developer info should configed in AndroidManifest.xml");
    }
    
    @Override
    public void setDebugMode(boolean debug)
    {
        Log.d(LOG_TAG, "setDebugMode: " + bDebug);
        bDebug = debug;   
    }
    
    @Override
    public String getSDKVersion() {
        LogD("getSDKVersion invoked!");
        return "1.9";
    }
    
    @Override
    public String getPluginVersion() {
        return "0.2.0";
    }

    @Override
    public void payForProduct(Hashtable<String, String> cpInfo) {
        // TODO Auto-generated method stub
        
    }
    
    public static void onInit (Context context)
    {
        LogD("onInit IAPYiJie");
        SFCommonSDKInterface.onInit((Activity) context);
        
        SFNativeAdapter.init((Activity) context, new SFActionCallback() {        
            @Override
            public void callback(Runnable run) {
                PluginWrapper.runOnGLThread(run);
            }
        });
    }
}
