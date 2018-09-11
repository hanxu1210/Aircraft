#include "PluginSocialWrapper.h"
#include "PluginManagerWrapper.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "CommonHeaders.h"


using namespace cocos2d::plugin;

namespace NsAircraft
{
    
    static void deepLinkCallback (const char *linkSchemas)
    {
        //linkSchemas内容为"GoToLeaderboard"，游戏接收到此字符串后，跳转到游戏排行版界面
    }

    static void dispatchRewards (KTRewardItemC * rewards, int length)
    {
        //int i = 0;
        //for(i = 0; i < length; i++){
        //    Game::applyReward(rewards[i]); //游戏接收奖励
        //}
    }
    
    static void updateKTTip(bool hasNewActivities){
        if(hasNewActivities){
           
        }else{
        }
    }

    static void loginViewCallback(bool isSuccess, KTUserC * user, KTErrorC *error)
    {
        PluginSocialWrapper* pluginSocial = dynamic_cast<PluginSocialWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_SOCIAL));
        if (pluginSocial)
        {
            pluginSocial->onKTLoginResult(isSuccess, user,  error);
        }
    }

    static void KTLoginStatusChanged(bool isLoggedIn, KTUserC * user)
    {
    }

    static void userProfileCallback(bool isSuccess, const char *userId, KTUserC * user, KTErrorC *error)
    {
        if (isSuccess) {

            //  user  是玩家信息

        }else{
            // error 详细错误信息

        }

    }

    static void reportScoreCallback(bool isSuccess, const char *leaderboardId, long long score, KTErrorC *error)
    {
        //MessageBoxLogic::Singleton().PopUpMessage("~~~~~~~~~~~~~~~~~~~~~~~~");

        PluginSocialWrapper* pluginSocial = dynamic_cast<PluginSocialWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_SOCIAL));
        if (pluginSocial)
        {
            pluginSocial->onReportScoreResult(isSuccess, leaderboardId, score, error);
        }
    }

    static void globalLeaderboardCallback(bool isSuccess, const char *leaderboardId, KTLeaderboardPaginatorC *leaderboard, KTErrorC *error)
    {
        PluginSocialWrapper* pluginSocial = dynamic_cast<PluginSocialWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_SOCIAL));
        if (pluginSocial)
        {
            pluginSocial->onGetLeaderboardResult(isSuccess, leaderboardId, leaderboard, error);
        }
    }

    static void onKTAppear(){
        //Game::pause();
    }

    static void onKTDisappear(){
        //Game::resume();
    }

    const char* PluginSocialWrapper::EVENT_REPORT_SCORE_RESULT = "event_report_score_result";
    const char* PluginSocialWrapper::EVENT_GET_LEADERBARD_RESULT = "event_get_leaderboard_result";
    const char* PluginSocialWrapper::EVENT_KT_LOGIN_RESULT = "event_kt_login_result";
    const char* PluginSocialWrapper::EVENT_OPEN_APPSTORE_RESULT = "event_open_appstore_result";

    void PluginSocialWrapper::PluginSocialListener::onSocialResult(cocos2d::plugin::SocialRetCode ret, const char* msg)
    {
        PluginSocialWrapper* pluginSocial = dynamic_cast<PluginSocialWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_SOCIAL));
        if (pluginSocial)
        {
            pluginSocial->onSocialResult(ret, msg);
        }
    }
    
    PluginSocialWrapper::PluginSocialWrapper():
        m_pluginSocial(nullptr),
        m_eventReportScoreResult(nullptr),
        m_eventGetLeaderboardResult(nullptr),
        m_eventKTLoginResult(nullptr),
        m_eventOpenAppstoreResult(nullptr)
    {
        m_pluginSocial = new PluginChannelSocial();
    }

    PluginSocialWrapper::~PluginSocialWrapper()
    {
        CC_SAFE_RELEASE_NULL(m_pluginSocial);
        CC_SAFE_RELEASE_NULL(m_eventReportScoreResult);
        CC_SAFE_RELEASE_NULL(m_eventGetLeaderboardResult);
        CC_SAFE_RELEASE_NULL(m_eventKTLoginResult);
        CC_SAFE_RELEASE_NULL(m_eventOpenAppstoreResult);
    }

    bool PluginSocialWrapper::init()
    {
        updateConfig();

        if (m_pluginSocial)
        {
            m_pluginSocial->init();

            m_pluginSocial->setDeepLinkCallback(deepLinkCallback);
            m_pluginSocial->setDidDispatchRewardsCallback(dispatchRewards);
            m_pluginSocial->setActivityStatusChangedCallback(updateKTTip);
            m_pluginSocial->setLoginStatusChangedCallback(KTLoginStatusChanged);
            m_pluginSocial->setViewDidAppearCallback(onKTAppear); 
            m_pluginSocial->setViewDidDisappearCallback(onKTDisappear);

            m_eventReportScoreResult = new cocos2d::EventCustom(EVENT_REPORT_SCORE_RESULT);
            m_eventGetLeaderboardResult = new cocos2d::EventCustom(EVENT_GET_LEADERBARD_RESULT);
            m_eventKTLoginResult = new cocos2d::EventCustom(EVENT_KT_LOGIN_RESULT);

             m_eventOpenAppstoreResult = new cocos2d::EventCustom(EVENT_OPEN_APPSTORE_RESULT);
        }
       
        return true;
    }

    bool PluginSocialWrapper::uninit()
    {
        return true;
    }

    void PluginSocialWrapper::loadPlugin()
    {
        m_pluginSocial->loadPlugin();
    }

    void PluginSocialWrapper::unloadPlugin()
    {
        m_pluginSocial->unloadPlugin();
    }

    void PluginSocialWrapper::setDebugMode(bool mode)
    {
        m_pluginSocial->setDebugMode(mode);
    }

    void PluginSocialWrapper::updateConfig()
    {

    }

    bool PluginSocialWrapper::isEnabled()
    {
        return m_pluginSocial->isEnabled();
    }

    bool PluginSocialWrapper::isShowing()
    {
        return m_pluginSocial->isShowing();
    }

    void PluginSocialWrapper::show()
    {
        m_pluginSocial->show();
    }

    void PluginSocialWrapper::dismiss()
    {
        m_pluginSocial->dismiss();
    }

    void PluginSocialWrapper::showInterstitialNotification()
    {
        m_pluginSocial->showInterstitialNotification();
    }

     bool PluginSocialWrapper::isLoggedIn()
     {
         return m_pluginSocial->isLoggedIn();
     }

     void PluginSocialWrapper::userProfile(const char* userId)
     {
         m_pluginSocial->userProfile(userId ,userProfileCallback); 
     }

     KTUserC* PluginSocialWrapper::currentAccount()
     {
         return m_pluginSocial->currentAccount();
     }

     void PluginSocialWrapper::login(bool closeable)
     {
         m_pluginSocial->login(closeable, loginViewCallback);               
     }

     void PluginSocialWrapper::logout()
     {
         m_pluginSocial->logout();
     }

     void PluginSocialWrapper::setNotificationEnabled(bool enabled)
     {
         m_pluginSocial->setNotificationEnabled(enabled);
     }

     void PluginSocialWrapper::reportScore(long long score, const char *leaderboardId)
     {
         m_pluginSocial->reportScore(score, leaderboardId, reportScoreCallback);
     }

     void PluginSocialWrapper::globalLeaderboard(const char *leaderboardId, int startIndex, int count)
     {
         m_pluginSocial->globalLeaderboard(leaderboardId, startIndex, count, globalLeaderboardCallback);
     }

    int PluginSocialWrapper::isSupportedLuyou()
    {
        if (m_pluginSocial)
        {
            return m_pluginSocial->isSupportedLuyou();
        }
        return 0;

    }
    
    void PluginSocialWrapper::appEnteredForeground()
    {
        if (m_pluginSocial)
        {
            m_pluginSocial->appEnteredForeground();
        }
    }
    
    void PluginSocialWrapper::ratingLogEvent()
    {
        if (m_pluginSocial)
        {
            m_pluginSocial->logEvent();
        }
    }

    void PluginSocialWrapper::onReportScoreResult(bool isSuccess, const char *leaderboardId, long long score, KTErrorC *error)
    {
        CCLOG("onReportScoreResult");
        ReportScoreResult reportScoreResult;
        reportScoreResult.isSuccess = isSuccess;
        reportScoreResult.leaderboardId = leaderboardId;
        reportScoreResult.score = score;
        reportScoreResult.error = error;
        m_eventReportScoreResult->setUserData(&reportScoreResult);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventReportScoreResult);
    }

    void PluginSocialWrapper::onGetLeaderboardResult(bool isSuccess, const char *leaderboardId, KTLeaderboardPaginatorC *leaderboard, KTErrorC *error)
    {
        CCLOG("onGetLeaderboardResult");
        GetLeaderboardResult getLeaderboardResult;
        getLeaderboardResult.isSuccess = isSuccess;
        getLeaderboardResult.leaderboardId = leaderboardId;
        getLeaderboardResult.leaderboard = leaderboard;
        getLeaderboardResult.error = error;
        m_eventGetLeaderboardResult->setUserData(&getLeaderboardResult);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventGetLeaderboardResult);
    }

    void PluginSocialWrapper::onKTLoginResult(bool isSuccess, KTUserC * user, KTErrorC *error)
    {
        CCLOG("onKTLoginResult");
        KTLoginResult KTLoginResult;
        KTLoginResult.isSuccess = isSuccess;
        KTLoginResult.user = user;
        KTLoginResult.error = error;
        m_eventKTLoginResult->setUserData(&KTLoginResult);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventKTLoginResult);
    }

    void PluginSocialWrapper::onSocialResult(cocos2d::plugin::SocialRetCode ret, const char* msg)
    {
        CCLOG("onSocialResult");
        OpenAppstoreResult result;
        if (ret == OPEN_APPSTORE_SUCCESS)
        {
            result.isSuccess = true;
        }
        else if (ret == OPEN_APPSTORE_FAILED)
        {
            result.isSuccess = false;
            result.error = msg;
        }
        
        m_eventOpenAppstoreResult->setUserData(&result);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventOpenAppstoreResult);
    }
}
