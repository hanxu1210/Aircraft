package org.cocos2dx.plugin;

import java.util.Hashtable;

import com.ktplay.open.KTPlay;

import android.app.Activity;
import android.content.Context;
import android.util.Log;


public class SocialKTPlay implements InterfaceSocial {
    private static final String LOG_TAG = "SocialKTPlay";
    private static Activity mContext = null;
    private static SocialKTPlay mSocialAdapter = null;
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
    
    public SocialKTPlay(Context context) {
        mContext = (Activity)context;
        mSocialAdapter = this;
    } 
    
    @Override
    public void configDeveloperInfo(Hashtable<String, String> cpInfo)
    {
        LogD("configDeveloperInfo invoked " + cpInfo.toString());
        try {
            
            final String appKey = cpInfo.get("KTPlayAppKey");
            final String appSecret = cpInfo.get("KTPlayAppSecret");
          
            KTPlay.startWithAppKey(mContext, appKey, appSecret);
        }catch (Exception e)
        {
            LogE("Developer info is wrong!", e);
        }
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
        return "ktplay cocos2dx sdk 1.5.2";
    }
    
    @Override
    public String getPluginVersion() {
        return "0.2.0";
    }
    
    public static void onResume(Context context)
    {     
        KTPlay.onResume(mContext);
    }
    
    public static void onPause(Context context)
    {
        KTPlay.onPause(mContext);
    }

    @Override
    public void submitScore(String leaderboardID, long score) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void showLeaderboard(String leaderboardID) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void unlockAchievement(Hashtable<String, String> achInfo) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void showAchievements() {
        // TODO Auto-generated method stub
        
    }
}
