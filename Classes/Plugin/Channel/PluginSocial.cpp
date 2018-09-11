#include "PluginSocial.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "Utilities/Utilities.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginSocial::PluginSocial()
    {
    }

    PluginSocial::~PluginSocial()
    {
    }

    bool PluginSocial::init()
    {
        return Plugin::init();
    }

    bool PluginSocial::isEnabled()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return KTPlayC::isEnabled();
#endif
        return false;
    }

    bool PluginSocial::isShowing()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return KTPlayC::isShowing();
#endif
        return false;
    }

    void PluginSocial::show()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::show();
#endif
    }

    void PluginSocial::dismiss()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::dismiss();
#endif
    }

    void PluginSocial::showInterstitialNotification()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::showInterstitialNotification();
#endif
    }

    bool PluginSocial::isLoggedIn()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return KTAccountManagerC::isLoggedIn();
#endif
        return false;
    }

    void PluginSocial::userProfile(const char* userId, KTUserProfileCallBack callback)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTAccountManagerC::userProfile(userId, callback); 
#endif
    }

    KTUserC* PluginSocial::currentAccount()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return KTAccountManagerC::currentAccount();
#endif
        return nullptr;
    }

    void PluginSocial::login(bool closeable, KTLoginCallBack callback)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTAccountManagerC::showLoginView(closeable, callback);
#endif                
    }

    void PluginSocial::logout()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTAccountManagerC::logout();
#endif
    }

    void PluginSocial::setNotificationEnabled(bool enabled)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::setNotificationEnabled(enabled);
#endif
    }

    void PluginSocial::reportScore(long long score, const char *leaderboardId, KTReportScoreCallBack callback)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTLeaderboardC::reportScore(score, leaderboardId, callback);
#endif
    }

    void PluginSocial::globalLeaderboard(const char *leaderboardId, int startIndex, int count, KTLeaderboardCallBack callback)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTLeaderboardC::globalLeaderboard(leaderboardId, startIndex, count, callback);
#endif
    }

    int PluginSocial::isSupportedLuyou()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolSocial* protocolSocial = dynamic_cast<ProtocolSocial*>(m_pluginProtocol[PP_LUYOU]);
        if (protocolSocial)
        {
            return protocolSocial->callIntFuncWithParam("isSupported", NULL);
        }
        return 0;
#else
        return 0;
#endif
    }

    void PluginSocial::appEnteredForeground()
    {
        
    }
    
    void PluginSocial::logEvent()
    {
        
    }
    
    void PluginSocial::setDeepLinkCallback( KTDeepLinkCallback callback )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::setDeepLinkCallback(callback);
#endif
    }

    void PluginSocial::setDidDispatchRewardsCallback( KTDispatchRewardsCallback callback )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::setDidDispatchRewardsCallback(callback);
#endif
    }

    void PluginSocial::setActivityStatusChangedCallback( KTActivityStatusChangedCallback callback )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::setActivityStatusChangedCallback(callback);
#endif
    }

    void PluginSocial::setLoginStatusChangedCallback( KTLoginStatusChangedCallBack callback )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTAccountManagerC::setLoginStatusChangedCallback(callback);
#endif
    }

    void PluginSocial::setViewDidAppearCallback( KTViewDidAppearCallback callback )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::setViewDidAppearCallback(callback); 
#endif
    }

    void PluginSocial::setViewDidDisappearCallback( KTViewDidDisappearCallback callback )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        KTPlayC::setViewDidDisappearCallback(callback);
#endif
    }


}