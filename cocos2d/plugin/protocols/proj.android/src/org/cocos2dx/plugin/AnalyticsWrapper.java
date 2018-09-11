package org.cocos2dx.plugin;

import java.util.HashMap;
import android.util.Log;    

public class AnalyticsWrapper {

	public static void onConfigDataReceived(InterfaceAnalytics adapter, HashMap<String, String> data) {
		final HashMap<String, String> dataHashMap = data;
		final InterfaceAnalytics curObj = adapter;

        Log.d("AnalyticsWrapper", "HashMap size" + data.size());
       
		PluginWrapper.runOnGLThread(new Runnable(){
			@Override
			public void run() {
				String name = curObj.getClass().getName();
				name = name.replace('.', '/');
				nativeOnConfigDataReceived(name, dataHashMap);
			}
		});
	}
	
	private native static void nativeOnConfigDataReceived(String className, HashMap<String, String> configData);
	
}
