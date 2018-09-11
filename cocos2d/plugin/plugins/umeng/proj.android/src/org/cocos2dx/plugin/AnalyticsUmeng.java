/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.plugin;

import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;

import org.json.JSONObject;

import com.umeng.analytics.game.UMGameAgent;
import com.umeng.analytics.onlineconfig.UmengOnlineConfigureListener;
import com.umeng.fb.FeedbackAgent;
import com.umeng.update.UmengUpdateAgent;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

public class AnalyticsUmeng implements InterfaceAnalytics{
    
    private Activity mContext = null;
    private FeedbackAgent mFBAgent = null;
    private static AnalyticsUmeng mAnalyticssAdapter = null;
    protected static String TAG = "AnalyticsUmeng";

    protected static void LogE(String msg, Exception e) {
        Log.e(TAG, msg, e);
        e.printStackTrace();
    }

    private static boolean isDebug = true;
    protected static void LogD(String msg) {
        if (isDebug) {
            Log.d(TAG, msg);
        }
    }

    public static void init(Context context)
    {
        UMGameAgent.init(context);
    }
    
    public AnalyticsUmeng(Context context) {
        mContext = (Activity)context;
        mAnalyticssAdapter = this;
        LogD("AnalyticsUmeng invoked!" + mContext);
        UMGameAgent.setWrapper("Cocos2d-x", "1.0");
    }
    
    public boolean isValid() {
        return mContext != null;
    }
    
    @Override
    public void startSession(String appKey) {
        LogD("startSession invoked!");
        UMGameAgent.onResume(mContext);
    }

    @Override
    public void stopSession() {
        LogD("stopSession invoked!" + mContext);
        UMGameAgent.onPause(mContext);
    }

    @Override
    public void setSessionContinueMillis(int millis) {
        LogD("setSessionContinueMillis invoked!");
        UMGameAgent.setSessionContinueMillis(millis);
    }

    @Override
    public void setCaptureUncaughtException(boolean isEnabled) {
        LogD("setCaptureUncaughtException invoked!");
        if (isEnabled) {
            UMGameAgent.reportError(mContext,"");
        }
    }

    @Override
    public void setDebugMode(boolean isDebugMode) {
        LogD("setDebugMode invoked!");
        isDebug = isDebugMode;
        UMGameAgent.setDebugMode(isDebugMode);
        com.umeng.fb.util.Log.LOG = isDebugMode;
    }

    @Override
    public void logError(String errorId, String message) {
        LogD("logError invoked!");
        UMGameAgent.reportError(mContext, message);
    }

    @Override
    public void logEvent(String eventId) {
        LogD("logEvent(" + eventId + ") invoked!");
        UMGameAgent.onEvent(mContext, eventId);
    }

    @Override
    public void logEvent(String eventId, Hashtable<String, String> paramMap) {
        LogD("logEvent(" + eventId + "," + paramMap.toString() + ") invoked!");
        HashMap<String, String> curParam = changeTableToMap(paramMap);
        UMGameAgent.onEvent(mContext, eventId, curParam);
    }

    @Override
    public void logTimedEventBegin(String eventId) {
        LogD("logTimedEventBegin(" + eventId + ") invoked!");
        UMGameAgent.onEventBegin(mContext, eventId);
    }

    @Override
    public void logTimedEventEnd(String eventId) {
        LogD("logTimedEventEnd(" + eventId + ") invoked!");
        UMGameAgent.onEventEnd(mContext, eventId);
    }

    @Override
    public String getSDKVersion() {
        LogD("getSDKVersion invoked!");
        return "4.6.3";
    }

    protected String getConfigParams(String key) {
        LogD("getConfigParams(" + key + ") invoked!");
        if (!isValid()) return null;
        String ret = "";
        try{
            ret = UMGameAgent.getConfigParams(mContext, key);
        } catch(Exception e){
            LogE("Exception in getConfigParams", e);
        }
        LogD("get config : " + ret);
        return ret;
    }
    
    protected void updateOnlineConfig() {
        LogD("updateOnlineConfig invoked!");
        if (!isValid()) return;
        try{
            UMGameAgent.setOnlineConfigureListener(m_OnlineConfigureListener);
            UMGameAgent.updateOnlineConfig(mContext);
        } catch(Exception e){
            LogE("Exception in updateOnlineConfig", e);
        }
    }

    protected void checkUpdate(){
    	LogD("checkUpdate invoked!");
    	if (!isValid()) return;
    	PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {               
                // syck new replies
                UmengUpdateAgent.setUpdateOnlyWifi(false);
                UmengUpdateAgent.update(mContext);
            }
        });
    }
    
    protected void initFeedback(String key){
        LogD("initFeedback invoked!");
        if (!isValid()) return;
        mFBAgent = new FeedbackAgent(mContext);
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {               
                // syck new replies
                mFBAgent.sync();
            }
        });
        
    }
    
    protected void showFeedback(){
        LogD("showFeedback invoked!");
        if (!isValid()) return;
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {               
                // open feedback panel
                mFBAgent.startFeedbackActivity();
            }
        });
        
    }
    
    protected void logEventWithLabel(JSONObject eventInfo) {
        LogD("logEventWithLabel invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try{
            String eventId = eventInfo.getString("Param1");
            String label = eventInfo.getString("Param2");
            UMGameAgent.onEvent(mContext, eventId, label);
        } catch(Exception e){
            LogE("Exception in logEventWithLabel", e);
        }
    }
    
    protected void logEventWithDurationLabel(JSONObject eventInfo) {
        LogD("logEventWithDurationLabel invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try {
            String eventId = eventInfo.getString("Param1");
            int duration = eventInfo.getInt("Param2");
            if (eventInfo.has("Param3")) {
                String label = eventInfo.getString("Param3");
                UMGameAgent.onEventDuration(mContext, eventId, label, duration);
            } else {
                UMGameAgent.onEventDuration(mContext, eventId, duration);
            }
        } catch (Exception e) {
            LogE("Exception in logEventWithDurationLabel", e);
        }
    }

    protected void logEventWithDurationParams(JSONObject eventInfo) {
        LogD("logEventWithDurationParams invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try {
            String eventId = eventInfo.getString("Param1");
            int duration = eventInfo.getInt("Param2");
            if (eventInfo.has("Param3")) {
                JSONObject params = eventInfo.getJSONObject("Param3");
                HashMap<String, String> curMap = getMapFromJson(params);
                UMGameAgent.onEventDuration(mContext, eventId, curMap, duration);
            } else {
                UMGameAgent.onEventDuration(mContext, eventId, duration);
            }
        } catch (Exception e) {
            LogE("Exception in logEventWithDurationParams", e);
        }
    }

    protected void logEventWithDuration(JSONObject eventInfo) {
        LogD("logEventWithDuration invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try{
            String eventId = eventInfo.getString("Param1");
            int duration = eventInfo.getInt("Param2");
            UMGameAgent.onEventDuration(mContext, eventId, duration);
        } catch(Exception e){
            LogE("Exception in logEventWithDuration", e);
        }
    }

    protected void logTimedEventWithLabelBegin(JSONObject eventInfo) {
        LogD("logTimedEventWithLabelBegin invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try{
            String eventId = eventInfo.getString("Param1");
            String label = eventInfo.getString("Param2");
            UMGameAgent.onEventBegin(mContext, eventId, label);
        } catch(Exception e){
            LogE("Exception in logTimedEventWithLabelBegin", e);
        }
    }
    
    protected void logTimedEventWithLabelEnd(JSONObject eventInfo) {
        LogD("logTimedEventWithLabelEnd invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try{
            String eventId = eventInfo.getString("Param1");
            String label = eventInfo.getString("Param2");
            UMGameAgent.onEventEnd(mContext, eventId, label);
        } catch(Exception e){
            LogE("Exception in logTimedEventWithLabelEnd", e);
        }
    }
    
    protected void logTimedKVEventBegin(JSONObject eventInfo) {
        LogD("logTimedKVEventBegin invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try{
            String eventId = eventInfo.getString("Param1");
            String label = eventInfo.getString("Param2");
            JSONObject params = eventInfo.getJSONObject("Param3");
            
            if (params != null) {
                HashMap<String, String> curMap = getMapFromJson(params);
                UMGameAgent.onKVEventBegin(mContext, eventId, curMap, label);
            }
        } catch(Exception e){
            LogE("Exception in logTimedKVEventBegin", e);
        }
    }
    
    protected void logTimedKVEventEnd(JSONObject eventInfo) {
        LogD("logTimedKVEventEnd invoked! event : " + eventInfo.toString());
        if (!isValid()) return;
        try{
            String eventId = eventInfo.getString("Param1");
            String label = eventInfo.getString("Param2");
            UMGameAgent.onKVEventEnd(mContext, eventId, label);
        } catch(Exception e){
            LogE("Exception in logTimedKVEventEnd", e);
        }
    }
    
    protected void pay(JSONObject eventInfo){
    	LogD("pay invoked! event : " + eventInfo.toString());
    	if (!isValid()) return;
    	try{
    		double cash = eventInfo.getDouble("Param1");
    		int source = eventInfo.getInt("Param2");
    		double coin = eventInfo.getDouble("Param3");
    		UMGameAgent.pay(cash, coin, source);
    	}catch(Exception e){
    		LogE("Exception in pay", e);
    	}
    }
    
    protected void payWithItem(JSONObject eventInfo){
    	LogD("payWithItem invoked! event : " + eventInfo.toString());
    	if (!isValid()) return;
    	try{
    		double cash = eventInfo.getDouble("Param1");
    		int source = eventInfo.getInt("Param2");
    		String item = eventInfo.getString("Param3");
    		int amount = eventInfo.getInt("Param4");
    		double price = eventInfo.getDouble("Param5");
    		UMGameAgent.pay(cash, item, amount, price, source);
    	}catch(Exception e){
    		LogE("Exception in payWithItem", e);
    	}
    }
    
    protected void buy(JSONObject eventInfo){
    	LogD("buy invoked! event : " + eventInfo.toString());
    	if (!isValid()) return;
    	try{
    		String item = eventInfo.getString("Param1");
    		int amount = eventInfo.getInt("Param2");
    		double price = eventInfo.getDouble("Param3");
    		UMGameAgent.buy(item, amount, price);
    	}catch(Exception e){
    		LogE("Exception in buy", e);
    	}
    }
    
    protected void use(JSONObject eventInfo){
    	LogD("use invoked! event : " + eventInfo.toString());
    	if (!isValid()) return;
    	try{
    		String item = eventInfo.getString("Param1");
    		int amount = eventInfo.getInt("Param2");
    		double price = eventInfo.getDouble("Param3");
    		UMGameAgent.use(item, amount, price);
    	}catch(Exception e){
    		LogE("Exception in use", e);
    	}
    }
    
    protected void bonusWithCoin(JSONObject eventInfo){
    	LogD("bonusWithCoin invoked! event : " + eventInfo.toString());
    	if (!isValid()) return;
    	try{
    		double coin = eventInfo.getDouble("Param1");
    		int source = eventInfo.getInt("Param2");
    		UMGameAgent.bonus(coin, source);
    	}catch(Exception e){
    		LogE("Exception in bonusWithCoin", e);
    	}
    }
    
    protected void bounsWithItem(JSONObject eventInfo){
    	LogD("bounsWithItem invoked! event : " + eventInfo.toString());
    	if (!isValid()) return;
    	try{
    		String item = eventInfo.getString("Param1");
    		int amount = eventInfo.getInt("Param2");
    		double price = eventInfo.getDouble("Param3");
    		int source = eventInfo.getInt("Param4");
    		UMGameAgent.bonus(item, amount, price, source);
    	}catch(Exception e){
    		LogE("Exception in bounsWithItem", e);
    	}
    }
    
    protected void startLevel(String level)
    {
    	LogD("startLevel invoked! event : " + level);
    	if (!isValid()) return;
    	try{
    		UMGameAgent.startLevel(level);
    	}catch(Exception e){
    		LogE("Exception in startLevel", e);
    	}
    }
    
    protected void finishLevel(String level)
    {
    	LogD("finishLevel invoked! event : " + level);
    	if (!isValid()) return;
    	try{
    		UMGameAgent.finishLevel(level);
    	}catch(Exception e){
    		LogE("Exception in finishLevel", e);
    	}
    }
    
    protected void failLevel(String level)
    {
    	LogD("failLevel invoked! event : " + level);
    	if (!isValid()) return;
    	try{
    		UMGameAgent.failLevel(level);
    	}catch(Exception e){
    		LogE("Exception in failLevel", e);
    	}
    }
    
    private HashMap<String, String> changeTableToMap(Hashtable<String, String> param) {
        HashMap<String, String> retParam = new HashMap<String, String>();
        for(Iterator<String> it = param.keySet().iterator(); it.hasNext(); ) {   
            String key = it.next();
            String value = param.get(key);

            retParam.put(key, value);
        }

        return retParam;
    }

    private static HashMap<String, String> getMapFromJson(JSONObject json) {
        HashMap<String, String> curMap = new HashMap<String, String>();
        try {
            @SuppressWarnings("rawtypes")
            Iterator it = json.keys();
            while (it.hasNext()) {
                String key = (String) it.next();
                String value = json.getString(key);
                curMap.put(key, value);
            }
        } catch (Exception e) {
            LogE("Error when get HashMap from JSONObject", e);
        }

        return curMap;
    }

    @Override
    public String getPluginVersion() {
        return "0.2.0";
    }
    
    private static UmengOnlineConfigureListener m_OnlineConfigureListener = new UmengOnlineConfigureListener() {
        @Override
        public void onDataReceived(JSONObject data) {
            LogD("UmengOnlineConfigureListener");
            if (data != null)
            {
                HashMap<String, String> dataMap = getMapFromJson(data);
                AnalyticsWrapper.onConfigDataReceived(mAnalyticssAdapter, dataMap);
            }   
        }
    
    };
}
