#ifndef _RUSHGIRL_PLUGINSOCIALWRAPPER_H_
#define _RUSHGIRL_PLUGINSOCIALWRAPPER_H_

#include "cocos2d.h"
#include "Channel/PluginChannelSocial.h"
#include "PluginProtocolWrapper.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

class KTUserC;
class KTErrorC;
class KTLeaderboardPaginatorC;

namespace NsAircraft
{
    class PluginSocialWrapper : public PluginProtocolWrapper
    {
    public:
        static const char* EVENT_REPORT_SCORE_RESULT;
        static const char* EVENT_GET_LEADERBARD_RESULT;
        static const char* EVENT_KT_LOGIN_RESULT;
        static const char* EVENT_OPEN_APPSTORE_RESULT;

        class PluginSocialListener : public cocos2d::plugin::SocialListener
        {
        public:
            /**
             @brief The share request result
             */
            virtual void onSocialResult(cocos2d::plugin::SocialRetCode ret, const char* msg) override;
        };
        
        enum E_Social_Platform
        {
            SP_KTPLAY,            
            SP_APPRATING,
            SP_LUYOU,
            SP_COUNT,
            SP_INVALID
        };

        struct ReportScoreResult
        {
            bool isSuccess;
            const char *leaderboardId;
            long long score;
            KTErrorC* error;
        };

        struct GetLeaderboardResult
        {
            bool isSuccess;
            const char *leaderboardId;
            KTLeaderboardPaginatorC *leaderboard;
            KTErrorC *error;
        };

        struct KTLoginResult
        {
            bool isSuccess;
            KTUserC * user;
            KTErrorC *error;
        };
        
        struct OpenAppstoreResult
        {
            bool isSuccess;
            const char* error;
        };

        /** PluginSocialWrapper constructor */
        PluginSocialWrapper();

        /** PluginSocialWrapper destructor */
        virtual ~PluginSocialWrapper();

        virtual bool init(void) override;
        virtual bool uninit(void) override;
        virtual void loadPlugin() override;
        virtual void unloadPlugin() override;
        virtual void updateConfig() override;
        virtual void setDebugMode(bool mode) override;

        bool isEnabled();
        bool isShowing();
        void show();
        void dismiss();
        void showInterstitialNotification();
        bool isLoggedIn();
        void userProfile(const char* userId);
        KTUserC* currentAccount();
        void login(bool closeable);
        void logout();
        void setNotificationEnabled(bool enabled);
        void reportScore(long long score, const char *leaderboardId);
        void globalLeaderboard(const char *leaderboardId, int startIndex, int count);

        void onReportScoreResult(bool isSuccess, const char *leaderboardId, long long score, KTErrorC *error);
        void onGetLeaderboardResult(bool isSuccess, const char *leaderboardId, KTLeaderboardPaginatorC *leaderboard, KTErrorC *error);
        void onKTLoginResult(bool isSuccess, KTUserC * user, KTErrorC *error);
        void onSocialResult(cocos2d::plugin::SocialRetCode ret, const char* msg);

        int isSupportedLuyou();
        
        void appEnteredForeground();
        void ratingLogEvent();
    private:
         PluginChannelSocial* m_pluginSocial;
         cocos2d::EventCustom* m_eventReportScoreResult;
         cocos2d::EventCustom* m_eventGetLeaderboardResult;
         cocos2d::EventCustom* m_eventKTLoginResult;
         cocos2d::EventCustom* m_eventOpenAppstoreResult;
    };
}

#endif