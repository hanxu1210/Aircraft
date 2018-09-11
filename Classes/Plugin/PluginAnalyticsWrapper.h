#ifndef _RUSHGIRL_PLUGINANALYTICSWRAPPER_H_
#define _RUSHGIRL_PLUGINANALYTICSWRAPPER_H_

#include "cocos2d.h"
#include "Channel/PluginChannelAnalytics.h"
#include "PluginProtocolWrapper.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginAnalyticsWrapper : public PluginProtocolWrapper
    {
    public:
         class PluginOnlineConfigListener : public cocos2d::plugin::OnlineConfigListener
        {
        public:
             /**
            @brief The onlineconfig request result
            */
            virtual void onConfigDataReceived(const cocos2d::plugin::ConfigDataMap& data) override;
        };

        /** PluginAdsrWrapper constructor */
        PluginAnalyticsWrapper();

        /** PluginAdsrWrapper destructor */
        virtual ~PluginAnalyticsWrapper();

        virtual bool init(void) override;
        virtual bool uninit(void) override;
        virtual void loadPlugin() override;
        virtual void unloadPlugin() override;
        virtual void updateConfig() override;
        virtual void setDebugMode(bool mode) override;
        
        std::string getOnlineConfig(const std::string& key);
        void startSession();
        void stopSession();
        void showFeedback();
        void startLevel(const std::string& level, int levelAmount);
        void finishLevel(const std::string& level, int levelAmount);
        void failLevel(const std::string& level, int levelAmount);
        void buy(const std::string& itemId, int amount, double price);

    private:
        std::string getLevelId(const std::string& levelId, int levelAmount);
    private:
         PluginChannelAnalytics* m_pluginAnalytics;
         PluginOnlineConfigListener* m_onlineConfigListener;
    };
}

#endif