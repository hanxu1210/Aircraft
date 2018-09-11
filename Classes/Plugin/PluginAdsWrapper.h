#ifndef _RUSHGIRL_PLUGINADSWRAPPER_H_
#define _RUSHGIRL_PLUGINADSWRAPPER_H_

#include "cocos2d.h"
#include "ProtocolAds.h"
#include "plugin/Channel/PluginChannelAds.h"
#include "PluginProtocolWrapper.h"
#include "Config/AdvertisingType.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginAdsWrapper : public PluginProtocolWrapper
    {
    public:
        struct AdsInfo
        {
            ADVERTISING_TYPE type;
            ADVERTISING_SIZE size;
            cocos2d::plugin::ProtocolAds::AdsPos pos;
            int retryCount;
            bool isShowing;
        };

        struct ClickedInfo
        {
            ADVERTISING_TYPE type;
            PluginChannelAds::E_Ads_Platform platform;
        };

        static const std::string typeNames[];
        static const char* EVENT_ADS_CLICKED;
        static const char* EVENT_ADS_ENTERLANDPAGE;
        static const char* EVENT_ADS_EXITLANDPAGE;
        class PluginAdsListener : public cocos2d::plugin::AdsListener
        {
        public:
             /**
            @brief The advertisement request result
            */
            virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg) override;
        };

        /** PluginAdsrWrapper constructor */
        PluginAdsWrapper();

        /** PluginAdsrWrapper destructor */
        virtual ~PluginAdsWrapper();

        virtual bool init(void) override;
        virtual bool uninit(void) override;
        virtual void loadPlugin() override;
        virtual void unloadPlugin() override;
        virtual void updateConfig() override;
        virtual void setDebugMode(bool mode) override;

        void showAds(ADVERTISING_TYPE type, cocos2d::plugin::ProtocolAds::AdsPos pos);        
        void hideAds(ADVERTISING_TYPE type);
        void hideAds(const std::string& typeName);

        void disableAds();
        void enableAds();
        bool isEnableAds();
        
        PluginChannelAds::E_Ads_Platform getAdsPlatform(ADVERTISING_TYPE type);

    private:
        void showAds(ADVERTISING_TYPE type, ADVERTISING_SIZE size, cocos2d::plugin::ProtocolAds::AdsPos pos);
        void preloadAds(ADVERTISING_TYPE type, ADVERTISING_SIZE size, cocos2d::plugin::ProtocolAds::AdsPos pos);
        void retry(const std::string& typeName);
        PluginChannelAds::E_Ads_Platform getSelectedAds(ADVERTISING_TYPE type);
        std::map<std::string, std::string> getMapFromString(const std::string& content);
        void onAdsClicked(const std::string& typeName);
        void onAdsReceived(const std::string& typeName);
        void onAdsDismissed(const std::string& typeName);
        void onAdsEntenLandPage(const std::string& typeName);
        void onAdsExitLandPage(const std::string& typeName);
        
    private:
        PluginChannelAds* m_pluginAds;
        PluginChannelAds::E_Ads_Platform m_currentAds;
        int m_maxRetry;
        typedef std::map<ADVERTISING_TYPE, PluginChannelAds::E_Ads_Platform> AdsSelectedPlatform;
        AdsSelectedPlatform m_adsSelectedPlatform;
        typedef std::map<std::string, AdsInfo> AdsInfos;
        AdsInfos m_adsInfos;
        PluginAdsListener* m_adsListener;
        bool m_isEnableAds;

        cocos2d::EventCustom* m_eventAdsClicked;
        cocos2d::EventCustom* m_eventAdsEntenLandPage;
        cocos2d::EventCustom* m_eventAdsExitLandPage;
    };
}

#endif