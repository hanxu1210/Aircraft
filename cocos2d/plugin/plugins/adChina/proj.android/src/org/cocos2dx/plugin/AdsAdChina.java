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

import com.adchina.android.ads.AdManager;
import com.adchina.android.ads.api.AdBannerListener;
import com.adchina.android.ads.api.AdFsListener;
import com.adchina.android.ads.api.AdFullScreen;
import com.adchina.android.ads.api.AdInterstitial;
import com.adchina.android.ads.api.AdInterstitialListener;
import com.adchina.android.ads.api.AdView;

import android.app.Activity;
import android.content.Context;
import android.location.LocationManager;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

public class AdsAdChina implements InterfaceAds {

	private static final String LOG_TAG = "AdsAdChina";
	private static Activity mContext = null;
	private static boolean bDebug = false;
	private static AdsAdChina mAdapter = null;

	private String mBannerID = "";
	private String mFullScreenID = "";
	private String mPopupID = "";
	private String mOfferwallID = "";
	private static AdView mBannerAdView = null;
	private AdFullScreen mFullScreenAdView = null;
	private AdInterstitial mPopupAdView = null;
	private static WindowManager mWm = null;
	private static final int ADCHINA_TYPE_BANNER = 1;
	private static final int ADCHINA_TYPE_FULLSCREEN = 2;
	private static final int ADCHINA_TYPE_POPUP = 3;
	private static final int ADCHINA_TYPE_OFFERWALL = 4;
    
	protected static void LogE(String msg, Exception e) {
		Log.e(LOG_TAG, msg, e);
		e.printStackTrace();
	}

	protected static void LogD(String msg) {
		if (bDebug) {
			Log.d(LOG_TAG, msg);
		}
	}

	public AdsAdChina(Context context) {
		mContext = (Activity) context;
		mAdapter = this;
	}
	
	@Override
	public void setDebugMode(boolean debug) {
	    LogD("setDebugMode" + debug);
		bDebug = debug;
		AdManager.setLogMode(debug);   
	}

	@Override
	public String getSDKVersion() {
		return "3.2.1";
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> devInfo) {
		try {
		    mBannerID = devInfo.get("AdChinaBannerID");
		    mFullScreenID = devInfo.get("AdChinaFullScreenID");
		    mPopupID = devInfo.get("AdChinaPopupID");
		    mOfferwallID = devInfo.get("AdChinaOfferwallID");
		    AdManager.setCanHardWare(true);
		    AdManager.setExpandToolBar(true);
		    AdManager.setAnimation(true);
		    AdManager.setEnableLbs(true);
		    AdManager.setLocationManager((LocationManager)mContext.getSystemService(Context.LOCATION_SERVICE));
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
	        int adsType = Integer.parseInt(strType);
	            
	        switch (adsType) {
    	        case ADCHINA_TYPE_BANNER:
    	            {
    	                String strSize = info.get("AdsSizeEnum");
                        int sizeEnum = Integer.parseInt(strSize);
    	                showBannerAd(sizeEnum, pos);
                        break;
    	            }
    	        case ADCHINA_TYPE_FULLSCREEN:
    	            showFullScreenAd();
    	            break;
    	        case ADCHINA_TYPE_POPUP:
    	            showPopUpAd();
                    break;
    	        case ADCHINA_TYPE_OFFERWALL:
    	            LogD("AdChina not support showOfferwall!");
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
		LogD("AdChina not support spend points!");
	}

    @Override
    public void queryPoints() {
        LogD("AdChina not support query points!");
    }
    
	@Override
	public void hideAds(Hashtable<String, String> info) {
	    try
        {
            String strType = info.get("AdsType");
            int adsType = Integer.parseInt(strType);

            switch (adsType) {
            case ADCHINA_TYPE_BANNER:
                hideBannerAd();
                break;
            default:
                break;
            }
        } catch (Exception e) {
            LogE("Error when hide Ads ( " + info.toString() + " )", e);
        }
	}
    
	public void requestAd(int adsType)
    {
        try
        {
            switch (adsType) {
                case ADCHINA_TYPE_BANNER:
                    {
                      
                        break;
                    }
                case ADCHINA_TYPE_FULLSCREEN:
                    
                    break;
                case ADCHINA_TYPE_POPUP:
                    
                    break;
                case ADCHINA_TYPE_OFFERWALL:
                    
                    break;
                default:
                    break;
                
            }
        } catch (Exception e) {
            LogE("Error when request Ads ( " + adsType + " )", e);
        }
    }
    
	private void showFullScreenAd()
	{
	    PluginWrapper.runOnMainThread(new Runnable() {

            @Override
            public void run() {
                mFullScreenAdView = new AdFullScreen(mContext, mFullScreenID);
                mFullScreenAdView.setAdFsListener(new AdChinaFsListenerImpl());
                mFullScreenAdView.start();
            }
	    });
	}
	
	private void showPopUpAd()
	{
	    PluginWrapper.runOnMainThread(new Runnable() {

            @Override
            public void run() {
                mPopupAdView = new AdInterstitial(mContext, mPopupID);
                mPopupAdView.setAdInterstitialListener(new AdChinaInterstitialListenerImpl());
                mPopupAdView.start();
            }
        });
	}

	private void showBannerAd(int sizeEnum, int pos) {
		final int curPos = pos;
		final int curSize = sizeEnum;

		PluginWrapper.runOnMainThread(new Runnable() {

            @Override
			public void run() {		    
                
                if (null != mBannerAdView) {
                    if (null != mWm) {
                        mWm.removeView(mBannerAdView);
                    }
                    mBannerAdView = null;
                }
                
                if (null == mWm) {
                    mWm = (WindowManager) mContext.getSystemService("window");
                }               
                
				mBannerAdView = new AdView(mContext, mBannerID, false, false);
				AdsWrapper.addAdView(mWm, mBannerAdView, curPos);
				
				mBannerAdView.setAdBannerListener(new AdChinaBannerListenerImpl());
				mBannerAdView.start();
				mBannerAdView.setVisibility(View.GONE);
			}
		});
	}

	private void hideBannerAd() {
	    PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {
                if (null != mBannerAdView) {
                    if (null != mWm) {
                        mWm.removeView(mBannerAdView);
                    }
                    mBannerAdView.stop();
                    mBannerAdView = null;
                }
            }
        });
	}

	
	private class AdChinaBannerListenerImpl implements AdBannerListener {

		@Override
		public void onClickBanner(AdView arg0) {
			LogD("onClickBanner invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsClicked, "AdsTypeBanner");
		}

		@Override
		public void onFailedToReceiveAd(AdView arg0) {
			LogD("onFailedToReceiveAd invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_UnknownError, "AdsTypeBanner");
		}

		@Override
		public void onReceiveAd(AdView arg0) {
			LogD("onReceiveAd invoked");
			if (arg0 != null)
			{
			    arg0.setVisibility(View.VISIBLE);
			}
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsReceived, "AdsTypeBanner");
		}
	}

	private class AdChinaInterstitialListenerImpl implements AdInterstitialListener {

        @Override
        public void onClickItst() {
            LogD("onClickItst invoked");
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsClicked, "AdsTypePopup");
        }

        @Override
        public void onFailedToReceiveItstAd() {
            LogD("onFailedToReceiveItstAd invoked");
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_UnknownError, "AdsTypePopup");
        }

        @Override
        public void onReceivedItstAd() {
            LogD("onReceivedItstAd invoked");
            if(mPopupAdView != null)
            {
                mPopupAdView.showItst();
            }
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsReceived, "AdsTypePopup");
        }
        
        @Override
        public void onCloseItst() {
            LogD("onCloseItst invoked");
            if(mPopupAdView != null)
            {
                mPopupAdView.stop();
            }
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsDismissed, "AdsTypePopup");
        }

        @Override
        public void onDisplayItst() {
            LogD("onDisplayItst invoked");
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsShown, "AdsTypePopup");
        }
    }
	
	private class AdChinaFsListenerImpl implements AdFsListener {

        @Override
        public void onClickFullScreenAd() {
            LogD("onClickFullScreenAd invoked");
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsClicked, "AdsTypeFullScreen");
        }

        @Override
        public void onFailedToReceiveFullScreenAd() {
            LogD("onFailedToReceiveFullScreenAd invoked");
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_UnknownError, "AdsTypeFullScreen");
        }

        @Override
        public void onEndFullScreenLandpage() {
            LogD("onEndFullScreenLandpage invoked");  
        }
        
        @Override
        public void onStartFullScreenLandPage() {
            LogD("onStartFullScreenLandPage invoked");     
        }
        
        @Override
        public void onReceiveFullScreenAd() {
            LogD("onReceiveFullScreenAd invoked");
            if(mFullScreenAdView != null)
            {
                mFullScreenAdView.showFs();
            }
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsReceived, "AdsTypeFullScreen!");
        }

        @Override
        public void onDisplayFullScreenAd() {
            LogD("onDisplayFullScreenAd invoked");
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsShown, "AdsTypeFullScreen");
        }

        @Override
        public void onFinishFullScreenAd() {
            LogD("onFinishFullScreenAd invoked");
            if(mFullScreenAdView != null)
            {
                mFullScreenAdView.stop();
            }
            AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsDismissed, "AdsTypeFullScreen");        
        }
    }
	
	@Override
	public String getPluginVersion() {
		return "0.2.0";
	}
	
	public static void dismiss()
    {
        LogD("onDestory AdChina");
        if (null != mBannerAdView) {
            if (null != mWm) {
                mWm.removeView(mBannerAdView);
            }
            mBannerAdView = null;
        }
    }
}
