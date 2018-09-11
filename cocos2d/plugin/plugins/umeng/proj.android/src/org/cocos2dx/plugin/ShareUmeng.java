package org.cocos2dx.plugin;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.microedition.khronos.opengles.GL10;

import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.SensorEvent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.util.TypedValue;

import com.umeng.scrshot.adapter.UMBaseAdapter;
import com.umeng.socialize.bean.MultiStatus;
import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.bean.SocializeConfig;
import com.umeng.socialize.bean.SocializeEntity;
import com.umeng.socialize.common.SocializeConstants;
import com.umeng.socialize.controller.UMServiceFactory;
import com.umeng.socialize.controller.UMSocialService;
import com.umeng.socialize.controller.listener.SocializeListeners.SnsPostListener;
import com.umeng.socialize.controller.listener.SocializeListeners.MulStatusListener;
import com.umeng.socialize.media.QQShareContent;
import com.umeng.socialize.media.QZoneShareContent;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.sensor.UMSensor.OnSensorListener;
import com.umeng.socialize.sensor.UMSensor.WhitchButton;
import com.umeng.socialize.sensor.controller.UMShakeService;
import com.umeng.socialize.sensor.controller.impl.UMShakeServiceFactory;
import com.umeng.socialize.sso.EmailHandler;
import com.umeng.socialize.sso.QZoneSsoHandler;
import com.umeng.socialize.sso.RenrenSsoHandler;
import com.umeng.socialize.sso.SinaSsoHandler;
import com.umeng.socialize.sso.SmsHandler;
import com.umeng.socialize.sso.TencentWBSsoHandler;
import com.umeng.socialize.sso.UMQQSsoHandler;
import com.umeng.socialize.sso.UMSsoHandler;
import com.umeng.socialize.weixin.controller.UMWXHandler;
import com.umeng.socialize.weixin.media.CircleShareContent;
import com.umeng.socialize.weixin.media.WeiXinShareContent;

public class ShareUmeng implements InterfaceShare {
    private static final String LOG_TAG = "ShareUmeng";
    private static Activity mContext = null;
    private static ShareUmeng mShareAdapter = null;
    private static SocializeConfig mSocializeConfig = SocializeConfig
            .getSocializeConfig();
    private static String mScreenShotPath = null;
    protected static boolean bDebug = false;
    
    private List<SHARE_MEDIA> mShareMediaList = new ArrayList<SHARE_MEDIA>();
    private UMSocialService mController = null;
    private static UMShakeService mShakeController = null;
    private String mTargetUrl = null;
    
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
    
    public ShareUmeng(Context context) {
        mContext = (Activity)context;
        mShareAdapter = this;
    } 
    
    @Override
    public void configDeveloperInfo(Hashtable<String, String> cpInfo)
    {
        LogD("configDeveloperInfo invoked " + cpInfo.toString());
        try {
            
            String appKey = cpInfo.get("UmengAppKey");
            String offcialSinaWeiboUserId = cpInfo.get("OffcialSinaWeiboUserId");
            //String offcialTencentWeiboUserId = cpInfo.get("OffcialTencentWeiboUserId");
            
            mShareAdapter.mController = UMServiceFactory.getUMSocialService("com.umeng.share");
            SocializeConstants.APPKEY = appKey;
            mSocializeConfig = mController.getConfig();
            mSocializeConfig.addFollow(SHARE_MEDIA.SINA, offcialSinaWeiboUserId);
            //mSocializeConfig.addFollow(SHARE_MEDIA.TENCENT, offcialTencentWeiboUserId);
            mSocializeConfig.setOauthDialogFollowListener(mMulStatusListener);
            
            mShakeController.setShakeSpeedShreshold(3000);
        
        }catch (Exception e)
        {
            LogE("Developer info is wrong!", e);
        }
    }
    
    @Override
    public void share(Hashtable<String, String> cpInfo)
    {
        LogD("share invoked " + cpInfo.toString());
        
        if (! NetworkUtility.networkReachable(mContext)) {
            ShareWrapper.onShareResult(mShareAdapter, ShareWrapper.SHARERESULT_TIMEOUT, "Network error!");
            return;
        }
        
        mController.registerListener(mSocialShareListener);
        String shareText = cpInfo.get("shareText");
        String shareImage = cpInfo.get("shareImage");
        UMImage shareUMImage = getUIImage(shareImage);
        
        mController.setShareContent(shareText);
        mController.setShareMedia(shareUMImage);
        
        // ��UI�߳�ִ�д򿪷���������
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {               
                // �򿪷������
                mController.openShare(mContext, false);
            }
        });

    }
    
    @Override
    public void setDebugMode(boolean debug)
    {
        bDebug = debug;   
        com.umeng.socialize.utils.Log.LOG = bDebug;
        SocializeConstants.SHOW_ERROR_CODE = bDebug;
        Log.d(LOG_TAG, "setDebugMode: " + bDebug);    
    }
    
    @Override
    public String getSDKVersion() {
        LogD("getSDKVersion invoked!");
        return "UMeng social sdk 4.2.1";
    }
    
    @Override
    public String getPluginVersion() {
        return "0.2.0";
    }
    
    public void shakeToShare(String shareText)
    {
        LogD("shakeToShare invoked " + shareText);
        mShakeController.setShareContent(shareText);           
    }
    
    public void setScreenShotPath(String path)
    {
        LogD("setScreenShotPath invoked " + path);
        mScreenShotPath = path;
    }
    
    public void setTargetUrl(String url)
    {
        LogD("setTargetUrl invoked " + url);
        if (!isValid()) return;
        mTargetUrl = url;
    }
    
    public void supportWeiXinPlatform(JSONObject info)
    {
        LogD("supportWeiXinPlatform invoked: " + info.toString());
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.WEIXIN);
        mShareMediaList.add(SHARE_MEDIA.WEIXIN_CIRCLE);
        
        try{
            final String appID = info.getString("Param1");
            final String appSecret = info.getString("Param2");
            final String title = info.getString("Param3");
            final String content = info.getString("Param4");
            final String image = info.getString("Param5");
            PluginWrapper.runOnMainThread(new Runnable() {
                @Override
                public void run() {               
                    
                 // ���΢��ƽ̨
                    UMWXHandler wxHandler = new UMWXHandler(mContext, appID, appSecret);
                    wxHandler.addToSocialSDK();
                    
                    WeiXinShareContent weixinContent = new WeiXinShareContent();
                    weixinContent.setShareContent(content);
                    weixinContent.setTitle(title);
                    weixinContent.setTargetUrl(mTargetUrl);
                    weixinContent.setShareImage(getUIImage(image));
                    mController.setShareMedia(weixinContent);
                    
                    // ֧��΢������Ȧ
                    UMWXHandler wxCircleHandler = new UMWXHandler(mContext, appID, appSecret);
                    wxCircleHandler.setToCircle(true);
                    wxCircleHandler.addToSocialSDK();
                    
                    CircleShareContent circleMedia = new CircleShareContent();
                    circleMedia.setShareContent(content);
                    circleMedia.setTargetUrl(mTargetUrl);
                    circleMedia.setShareImage(getUIImage(image));
                    circleMedia.setTitle(title);
                    mController.setShareMedia(circleMedia);
                    
                    SortPlatform();
                }
            });
        } catch(Exception e){
            LogE("Exception in supportQQPlatform", e);
        }  
    }
    
    public void supportQQPlatform(JSONObject info)
    {
        LogD("supportQQPlatform invoked: " + info.toString());
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.QQ);
        mShareMediaList.add(SHARE_MEDIA.QZONE);
        
        try{
            final String appID = info.getString("Param1");
            final String appKey = info.getString("Param2");
            final String title = info.getString("Param3");
            PluginWrapper.runOnMainThread(new Runnable() {
                @Override
                public void run() {     
                  //����1Ϊ��ǰActivity�� ����2Ϊ��������QQ���������APP ID��
                  //����3Ϊ��������QQ���������APP kEY.
                  UMQQSsoHandler qqSsoHandler = new UMQQSsoHandler(mContext, appID, appKey);
                  qqSsoHandler.setTargetUrl(mTargetUrl);
                  qqSsoHandler.setTitle(title);
                  qqSsoHandler.addToSocialSDK();  
                  
                  QZoneSsoHandler qZoneSsoHandler = new QZoneSsoHandler(mContext, appID, appKey);
                  qZoneSsoHandler.setTargetUrl(mTargetUrl);
                  qZoneSsoHandler.addToSocialSDK();
                  
                  SortPlatform();
                }
            });
        } catch(Exception e){
            LogE("Exception in supportQQPlatform", e);
        }  
    }
    
    public void supportSinaWeiboPlatform()
    {
        LogD("supportSinaWeiboPlatform invoked");
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.SINA);
    
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {     
              //��������SSO handler
              mController.getConfig().setSsoHandler(new SinaSsoHandler()); 
              
              SortPlatform();
            }
        });
    }
    
    public void supportTencentWeiboPlatform(final String redirectUrl)
    {
        LogD("supportTencentWeiboPlatform invoked");
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.TENCENT);
        
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {     
              //������Ѷ΢��SSO handler
              mController.getConfig().setSsoHandler(new TencentWBSsoHandler());   
              
              SortPlatform();
            }
        });     
    }
    
    public void supportRenrenPlatform(JSONObject info)
    {
        LogD("supportRenrenPlatform invoked");
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.RENREN);
        
        try{
            final String appID = info.getString("Param1");
            final String appKey = info.getString("Param2");
            final String secret = info.getString("Param3");
            PluginWrapper.runOnMainThread(new Runnable() {
                @Override
                public void run() {     
                  //���������SSO��Ȩ����     
                  //APPID:201874      
                  //API Key:28401c0964f04a72a14c812d6132fcef    
                  //Secret:3bf66e42db1e4fa9829b955cc300b737     
                  RenrenSsoHandler renrenSsoHandler = new RenrenSsoHandler(mContext,
                          appID, appKey, secret);
                  mController.getConfig().setSsoHandler(renrenSsoHandler);
                  
                  SortPlatform();
                }
            });
        } catch(Exception e){
            LogE("Exception in supportRenrenPlatform", e);
        }  
    }
    
    public void supportDoubanPlatform()
    {
        LogD("supportDoubanPlatform invoked");
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.DOUBAN);
        
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {                  
                SortPlatform();
            }
        });
    }
    
    public void supportSmsPlatform()
    {
        LogD("supportSmsPlatform invoked");
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.SMS);
        
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {     
                // ��Ӷ���
                SmsHandler smsHandler = new SmsHandler();
                smsHandler.addToSocialSDK();   
              
                SortPlatform();
            }
        });
    }
    
    public void supportEmailPlatform()
    {
        LogD("supportEmailPlatform invoked");
        if (!isValid()) return;
        mShareMediaList.add(SHARE_MEDIA.EMAIL);
        
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {    
                // ���email
                EmailHandler emailHandler = new EmailHandler();
                emailHandler.addToSocialSDK();  
                
                SortPlatform();
            }
        });
    }
    
    private void SortPlatform()
    {
        PluginWrapper.runOnMainThread(new Runnable() {
            @Override
            public void run() {     
                SHARE_MEDIA[] platformsMedias = new SHARE_MEDIA[mShareMediaList.size()];
                mShareMediaList.toArray(platformsMedias);
                // ����ƽ̨
                mSocializeConfig.setPlatforms(platformsMedias);
                // ������ʾ˳��
                mSocializeConfig.setPlatformOrder(platformsMedias);         
            }
        });
      
    }
    
    public boolean isValid() {
        return mContext != null;
    }
    
    private boolean networkReachable() {
        boolean bRet = false;
        try {
            ConnectivityManager conn = (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo netInfo = conn.getActiveNetworkInfo();
            bRet = (null == netInfo) ? false : netInfo.isAvailable();
        } catch (Exception e) {
            LogE("Fail to check network status", e);
        }
        LogD("NetWork reachable : " + bRet);
        return bRet;
    }
    
    private UMImage getUIImage(String shareImage)
    {
        UMImage shareUMImage = null;
        // ����ͼƬ
        if (shareImage.startsWith("http://") || shareImage.startsWith("https://")) {
            shareUMImage = new UMImage(mContext, shareImage);
            
        } else {
            // ����ͼƬ
            File imgFile = new File(shareImage);
            if (!imgFile.exists()) {
                InputStream is = null;
                try {
                    final TypedValue value = new TypedValue();
                    is = mContext.getAssets().open(shareImage);
                    shareUMImage = new UMImage(mContext, BitmapFactory.decodeResourceStream(mContext.getResources(), value, is, null, null));
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    LogE("file: " + shareImage + "not found!", e);                   
                }finally{
                    try {
                        if (is != null) is.close();
                    } catch (IOException e) {
                        // Ignore
                    }
                }
                
            } else {
                shareUMImage = new UMImage(mContext, imgFile);
            }
        }
        
        return shareUMImage;
    }
    
    public static void onActivityResult(int requestCode, int resultCode,
            Intent data) {
        UMSsoHandler ssoHandler = mSocializeConfig.getSsoHandler(requestCode);
        if (ssoHandler != null) {
            ssoHandler.authorizeCallBack(requestCode, resultCode, data);
        }
    }
    
    public static void onResume(Context context) {
       
        // ע��ҡһҡ��ͼ���� GameActivityΪ�����ϷActivity�� ������ SensorListener�н�����Ϸ����
        // ͣ���ָ��Ȳ����������뿴������ SensorListener�Ķ���       
        if (mShakeController == null)
        {
            mShakeController = UMShakeServiceFactory
                    .getShakeService("com.umeng.share");
        }
        LogD("onResume ShareUmeng.");
        if (mShakeController != null)
        {
            UMBaseAdapter gameAdapter = new SurfaceViewAdapter();
            // ����ҡһҡ��������ʱ�û���ѡ��ƽ̨�����֧�����ƽ̨ 
            List<SHARE_MEDIA> platforms = new ArrayList<SHARE_MEDIA>();
            
            mShakeController.registerShakeListender((Activity)context, gameAdapter,
                    platforms, mSensorListener);
        }
    }
    
    public static void onPause(Context context)
    {
        LogD("onPause ShareUmeng.");
        mShakeController.unregisterShakeListener((Activity)context);
    }
    
    public static void onDestory(Context context)
    {
        LogD("onDestory ShareUmeng");
        mShakeController.unregisterShakeListener((Activity)context);
    }
    
    /******************************************************************************
     * ����ص��ӿ�,�����native���Ӧ�Ļص�����, �����߿�����Java����native�������Ӧ�Ĵ���
     *****************************************************************************/
    private static SnsPostListener mSocialShareListener = new SnsPostListener() {

        /*
         * ����ʼ (non-Javadoc)
         * 
         * @see com.umeng.socialize.controller.listener.SocializeListeners.
         * SnsPostListener#onStart()
         */
        @Override
        public void onStart() {
            LogD("SnsPostListener share start.");
        }

        /*
         * ������� (non-Javadoc)
         * 
         * @see com.umeng.socialize.controller.listener.SocializeListeners.
         * SnsPostListener#onComplete(com.umeng.socialize.bean.SHARE_MEDIA, int,
         * com.umeng.socialize.bean.SocializeEntity)
         */
        @Override
        public void onComplete(final SHARE_MEDIA platform, final int eCode,
                final SocializeEntity entity) {
            if(eCode == 200)
            {
                ShareWrapper.onShareResult(mShareAdapter, ShareWrapper.SHARERESULT_SUCCESS, entity.mDescriptor);
            }
            else
            {
                ShareWrapper.onShareResult(mShareAdapter, ShareWrapper.SHARERESULT_FAIL, entity.mDescriptor);
            }
           
        }
    };
    
    private static MulStatusListener mMulStatusListener = new MulStatusListener() {
        @Override
        public void onStart() {
            LogD("Follow Start");
        }

        @Override
        public void onComplete(MultiStatus multiStatus, int st,
                SocializeEntity entity) {
            if (st == 200) {
                Map<String, Integer> allChildren = multiStatus
                        .getAllChildren();
                Set<String> set = allChildren.keySet();
                for (String fid : set)
                    LogD(fid + "    " + allChildren.get(fid));
            }
        }
    };
    
    /**
    * ��������������������ļ�����ʹ��
    */
    private static OnSensorListener mSensorListener = new OnSensorListener() {

        @Override
        public void onStart() {
            LogD("OnSensorListener onStart invoked");
        }

        /**
        * ������ɺ�ص� 
        */
        @Override
        public void onComplete(SHARE_MEDIA platform, int eCode, SocializeEntity entity) {
            if(eCode == 200)
            {
                ShareWrapper.onShareResult(mShareAdapter, ShareWrapper.SHARERESULT_SUCCESS, entity.mDescriptor);
            }
            else
            {
                ShareWrapper.onShareResult(mShareAdapter, ShareWrapper.SHARERESULT_FAIL, entity.mDescriptor);
            }
        }

        /**
        * @Description: ҡһҡ������ɺ�ص� 
        */
        @Override
        public void onActionComplete(SensorEvent event) {
            LogD("onActionComplete invoked");
            ShareWrapper.onShakeActionComplete(mShareAdapter);
        }

        /**
        * @Description: �û���������ڵ�ȡ���ͷ���ť�����Ļص�
        * @param button �û��ڷ����ڵ���İ�ť����ȡ���ͷ���������ť
        */
        @Override
        public void onButtonClick(WhitchButton button) {
            if (button == WhitchButton.BUTTON_CANCEL) {
                ShareWrapper.onShakeClosed(mShareAdapter);
            } 
            else {
            // ������, ( �û�����˷���ť )
                ShareWrapper.onShakeClosed(mShareAdapter);
            }
        }
    };
    
    public static class SurfaceViewAdapter extends UMBaseAdapter {
        // ע�� : gitBitmap()��sdk�ڲ������߳��е��ã���������ڸú����и���UI.
    @Override
    public Bitmap getBitmap() {
       // ������Ϸ��ͼ��ͼ��
        LogD("getBitmap invoked");
        synchronized (mShareAdapter) { 
            try {
                mShareAdapter.wait();
                File f = new File(mScreenShotPath);
                if (!f.exists() || f.isDirectory()) {
                    LogD("File inexist!!");
                    return null;
                }
                LogD("BitmapFactory invoked");
                Bitmap bm = BitmapFactory.decodeFile(mScreenShotPath);
                return bm;
            } catch (Exception e) {
                LogD("File excepetion!!!");
                return null;
            }
        }
    }
}
}
