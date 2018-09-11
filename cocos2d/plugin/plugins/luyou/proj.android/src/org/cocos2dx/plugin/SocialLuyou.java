package org.cocos2dx.plugin;

import java.util.Hashtable;

import com.luyousdk.core.LYCore;
import com.luyousdk.core.LYCore.Recorder.RecorderListener;
import com.luyousdk.core.ui.FloatViewUtils;

import android.app.Activity;
import android.content.Context;
import android.util.Log;


public class SocialLuyou implements InterfaceSocial {
    private static final String LOG_TAG = "SocialKTPlay";
    private static Activity mContext = null;
    private static SocialLuyou mSocialAdapter = null;
    protected static boolean bDebug = false;
    private static boolean bInitialized = false;
    
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
    
    public SocialLuyou(Context context) {
        mContext = (Activity)context;
        mSocialAdapter = this;
    } 
    
    @Override
    public void configDeveloperInfo(Hashtable<String, String> cpInfo)
    {
        LogD("configDeveloperInfo invoked " + cpInfo.toString());
        try {
            
            final String appKey = cpInfo.get("LuyouAppKey");
            LYCore.initialize(mContext);
            LYCore.appKey(appKey);
            FloatViewUtils.getInstance().initFloatView(mContext);
            LYCore.Recorder.registerListener(mRecorderListener);
            bInitialized = true;       
            
        }catch (Exception e)
        {
            LogE("Developer info is wrong!", e);
        }
    }
    
    @Override
    public void setDebugMode(boolean debug)
    {
        LogD("setDebugMode: " + bDebug);
        bDebug = debug;   
    }
    
    @Override
    public String getSDKVersion() {
        LogD("getSDKVersion invoked!");
        return "luyou cocos2dx sdk 1.0.16";
    }
    
    @Override
    public String getPluginVersion() {
        return "0.2.0";
    }
    
    public static void onResume(Context context)
    {  
        if (bInitialized)
        {
            if (LYCore.Recorder.isPaused()){
                LYCore.Recorder.resumeRecording();
            }
            FloatViewUtils.getInstance().showFloatView();
        }
    }
    
    public static void onPause(Context context)
    {
        if (bInitialized)
        {
            if (LYCore.Recorder.isRecording()){
                LYCore.Recorder.pauseRecording();
            }
            FloatViewUtils.getInstance().hideFloatView();
        }
    }

    public static void onDestory(Context context)
    {
        if (bInitialized)
        {
            //移除录制工具条
            FloatViewUtils.getInstance().removeFloatView();
             // 取消录制监听器
            LYCore.Recorder.unRegisterListener(mRecorderListener);
             // 释放资源
            LYCore.release((Activity)context);
        }
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
    
    public int isSupported()
    {
        LogD("isSupported");
        return LYCore.Recorder.isSupported();
    }
    
    private static RecorderListener mRecorderListener = new RecorderListener() {
        
        @Override
        public void onRecordingStopped() {
            LogD("LYCore.Recorder.isRecording() onRecordingStopped ");
        }
        
        @Override
        public void onRecordingStarted() {
            LogD("LYCore.Recorder.isRecording() onRecordingStarted ");
        }

        @Override
        public void onRecordingFailed(int errCode, String msg) {
            LogD("LYCore.Recorder.isRecording() onRecordingFailed ");
        }
        @Override
        public void  onRecordingTime(int seconds){
            LogD("LYCore.Recorder.isRecording() onRecordingTime ");
        }
        @Override
        public void onRecordingWarning(int warningCorde, String msg){
            LogD("LYCore.Recorder.isRecording() onRecordingWarning ");
        }
    };

}
