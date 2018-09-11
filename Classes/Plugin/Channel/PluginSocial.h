#ifndef _RUSHGIRL_PLUGIN_SOCIAL_H_
#define _RUSHGIRL_PLUGIN_SOCIAL_H_

#include "cocos2d.h"
#include "Plugin.h"
#include "ProtocolSocial.h"
#include "../KTPlay/KTLeaderboardC.h"
#include "../KTPlay/KTPlayC.h"
#include "../KTPlay/KTAccountManagerC.h"
#include "../KTPlay/KTErrorC.h"
#include "../KTPlay/KTLeaderboardPaginatorC.h"
#include "../KTPlay/KTUserC.h"

namespace NsAircraft
{
    class PluginSocial : public Plugin
    {
    public:
        /** PluginSocial constructor */
        PluginSocial();

        /** PluginSocial destructor */
        virtual ~PluginSocial();

        virtual bool init() override;
     
        bool isEnabled();
        bool isShowing();
        void show();
        void dismiss();
        void showInterstitialNotification();
        bool isLoggedIn();
        void userProfile(const char* userId, KTUserProfileCallBack callback);
        KTUserC* currentAccount();
        void login(bool closeable, KTLoginCallBack callback);
        void logout();
        void setNotificationEnabled(bool enabled);
        void reportScore(long long score, const char *leaderboardId, KTReportScoreCallBack callback);
        void globalLeaderboard(const char *leaderboardId, int startIndex, int count, KTLeaderboardCallBack callback);

        int isSupportedLuyou();

        virtual void appEnteredForeground();
        virtual void logEvent();
        
        void setDeepLinkCallback(KTDeepLinkCallback callback);
        void setDidDispatchRewardsCallback(KTDispatchRewardsCallback callback);
        void setActivityStatusChangedCallback(KTActivityStatusChangedCallback callback);
        void setLoginStatusChangedCallback(KTLoginStatusChangedCallBack callback);
        void setViewDidAppearCallback(KTViewDidAppearCallback callback); 
        void setViewDidDisappearCallback(KTViewDidDisappearCallback callback);
    };
}

#endif