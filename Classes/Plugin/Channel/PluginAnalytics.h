#ifndef _RUSHGIRL_PLUGIN_ANALYTICS_H_
#define _RUSHGIRL_PLUGIN_ANALYTICS_H_

#include "cocos2d.h"
#include "Plugin.h"
#include "ProtocolAnalytics.h"

namespace NsAircraft
{
    class PluginAnalytics : public Plugin
    {
    public:
        typedef enum {
            REALTIME = 0,
            BATCH = 1,
            SENDDAILY = 4,
            SENDWIFIONLY = 5,
            SEND_INTERVAL = 6,
            SEND_ON_EXIT = 7
        } ReportPolicy;
        
        /** PluginAnalytics constructor */
        PluginAnalytics();

        /** PluginAnalytics destructor */
        virtual ~PluginAnalytics();

        virtual bool init() override;
        std::string getOnlineConfig(const std::string& key);
        void startSession();
        void stopSession();
        void showFeedback();
        void startLevel(const std::string& level, const std::string& levelId);
        void finishLevel(const std::string& level, const std::string& levelId);
        void failLevel(const std::string& level, const std::string& levelId);
        void buy(const std::string& itemId, int amount, double price);

        virtual void setOnlineConfigListener(cocos2d::plugin::OnlineConfigListener* listener);
    };
}

#endif