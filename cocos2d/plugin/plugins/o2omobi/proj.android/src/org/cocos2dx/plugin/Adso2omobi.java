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

import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;

import com.otomod.ad.AdSize;
import com.otomod.ad.AdView;
import com.otomod.ad.listener.O2OAdListener;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

public class Adso2omobi implements InterfaceAds {

	private static final String LOG_TAG = "Adso2omobi";
	private static Activity mContext = null;
	private static boolean bDebug = false;
	private static Adso2omobi mAdapter = null;

	private String mAppkey = "";
	private static AdView mBannerAdView = null;
	private static LinearLayout mLayout;
	private static WindowManager mWm = null;
	private static final int O2OMOBI_TYPE_BANNER = 1;
	private static final int O2OMOBI_TYPE_FULLSCREEN = 2;
	private static final int O2OMOBI_TYPE_POPUP = 3;
	private static final int O2OMOBI_TYPE_OFFERWALL = 4;
    
	protected static void LogE(String msg, Exception e) {
		Log.e(LOG_TAG, msg, e);
		e.printStackTrace();
	}

	protected static void LogD(String msg) {
		if (bDebug) {
			Log.d(LOG_TAG, msg);
		}
	}

	public Adso2omobi(Context context) {
		mContext = (Activity) context;
		mAdapter = this;
	}
	
	@Override
	public void setDebugMode(boolean debug) {
		bDebug = debug;
	}

	@Override
	public String getSDKVersion() {
		return "4.0.1";
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> devInfo) {
		try {
			mAppkey = devInfo.get("o2omobiAppKey");
			LogD("init Appkey : " + mAppkey);
		} catch (Exception e) {
			LogE("initAppInfo, The format of appInfo is wrong", e);
		}
	}

	@Override
	public void showAds(Hashtable<String, String> info, int pos) {
	    if (! NetworkUtility.networkReachable(mContext)) 
	    {
	        return;
	    }
	    try
	    {
	        String strType = info.get("AdsType");
	        LogD("showAds" + strType);
	        int adsType = Integer.parseInt(strType);
	            
	        switch (adsType) {
    	        case O2OMOBI_TYPE_BANNER:
    	            {
    	                String strSize = info.get("AdsSizeEnum");
                        int sizeEnum = Integer.parseInt(strSize);
    	                showBannerAd(sizeEnum, pos);
                        break;
    	            }
    	        case O2OMOBI_TYPE_FULLSCREEN:
    	            showFullScreenAd();
    	            break;
    	        case O2OMOBI_TYPE_POPUP:
    	            showPopUpAd();
                    break;
    	        case O2OMOBI_TYPE_OFFERWALL:
    	            showOfferwall();
    	            break;
    	        default:
    	            break;
	        }
	    } catch (Exception e) {
	        LogE("Error when show Ads ( " + info.toString() + " )", e);
	    }
	}

	@Override
	public void spendPoints(int points) {
		LogD("Ado2omobi not support spend points!");
	}

    @Override
    public void queryPoints() {
        LogD("Ado2omobi not support query points!");
    }
    
	@Override
	public void hideAds(Hashtable<String, String> info) {
	    try
        {
            String strType = info.get("AdsType");
            int adsType = Integer.parseInt(strType);

            switch (adsType) {
            case O2OMOBI_TYPE_BANNER:
                hideBannerAd();
                break;
            case O2OMOBI_TYPE_FULLSCREEN:
                LogD("Now not support full screen view in Admob");
                break;
            default:
                break;
            }
        } catch (Exception e) {
            LogE("Error when hide Ads ( " + info.toString() + " )", e);
        }
	}
    
	private void showFullScreenAd()
	{
	    PluginWrapper.runOnMainThread(new Runnable() {

            @Override
            public void run() {
                AdView adView = AdView.createFullScreen(mContext, mAppkey);
                adView.setAdListener(new O2OAdListenerImpl("AdsTypeFullScreen"));
                adView.request();
            }
	    });
	}
	
	private void showPopUpAd()
	{
	    PluginWrapper.runOnMainThread(new Runnable() {

            @Override
            public void run() {
                AdView adView = AdView.createPopup(mContext, mAppkey);
                adView.setAdListener(new O2OAdListenerImpl("AdsTypePopup"));
                adView.request();
            }
        });
	}
	
	private void showOfferwall()
    {
        /*PluginWrapper.runOnMainThread(new Runnable() {

            @Override
            public void run() {
                //OffersManager.showOffers(mContext, mAppkey);
                //OffersManager.setOffersListener(new WallListener() {
                    @Override
                    //public void awardPoints(int points) {
                    //    LogD("awardPoints invoked");
                    //    AdsWrapper.onPlayerGetPoints(mAdapter, points);
                    //}
                //});
            }
        });   */  
    }

	private void showBannerAd(int sizeEnum, int pos) {
		final int curPos = pos;
		final int curSize = sizeEnum;

		PluginWrapper.runOnMainThread(new Runnable() {

            @Override
			public void run() {		    
                
                if (null != mLayout) {
                    if (null != mWm) {
                        mWm.removeView(mLayout);
                    }
                    mLayout = null;
                    mBannerAdView = null;
                }
                
                if (null == mWm) {
                    mWm = (WindowManager) mContext.getSystemService("window");
                }
                
                mLayout = new LinearLayout(mContext);
                mLayout.setOrientation(LinearLayout.VERTICAL);
                
                AdsWrapper.addAdView(mWm, mLayout, curPos);
                
                mBannerAdView = AdView.createBanner(mContext, mLayout, curSize, mAppkey);
                mBannerAdView.setAdListener(new O2OAdListenerImpl("AdsTypeBanner"));
                mBannerAdView.request();
                mLayout.setVisibility(View.GONE);
			}
		});
	}

	private void hideBannerAd() {
	    PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {
                if (null != mBannerAdView) {
                    if (null != mWm) {
                        mWm.removeView(mLayout);
                    }
                    mLayout = null;
                    mBannerAdView = null;
                }
            }
        });
	}
	
	private class O2OAdListenerImpl implements O2OAdListener {

	    private String type = "";
	    public O2OAdListenerImpl(String type) {
	        this.type = type;
	    }
		@Override
		public void onClick() {
			LogD("onClick invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsClicked, type);
		}

		@Override
		public void onClose() {
			LogD("onClose invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsDismissed, type);
		}

		@Override
		public void onAdFailed() {
			LogD("onAdFailed invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_UnknownError, type);
		}

		@Override
		public void onAdSuccess() {
			LogD("onAdSuccess invoked");
			if (type == "AdsTypeBanner" && mLayout != null)
			{
			    mLayout.setVisibility(View.VISIBLE);
			}
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsShown, type);
		}
	}

	@Override
	public String getPluginVersion() {
		return "0.2.0";
	}
	
    public static void dismiss()
    {
        LogD("onDestory o2omobi");
        if (null != mLayout) {
            if (null != mWm) {
                mWm.removeView(mLayout);
            }
            mLayout = null;
            mBannerAdView = null;
        }
    }
}
