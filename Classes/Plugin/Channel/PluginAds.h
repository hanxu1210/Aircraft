#ifndef _RUSHGIRL_PLUGIN_ADS_H_
#define _RUSHGIRL_PLUGIN_ADS_H_

#include "cocos2d.h"
#include "Plugin.h"
#include "ProtocolAds.h"
#include "Config/AdvertisingType.h"

namespace NsAircraft
{
    class PluginAds : public Plugin
    {
    public:
        /** PluginAds constructor */
        PluginAds();

        /** PluginAds destructor */
        virtual ~PluginAds();

        virtual bool init() override;

        void showAds(E_Plugin_Platform platform, ADVERTISING_TYPE type, ADVERTISING_SIZE size, cocos2d::plugin::ProtocolAds::AdsPos pos);
        void preloadAds(E_Plugin_Platform platform, ADVERTISING_TYPE type, ADVERTISING_SIZE size, cocos2d::plugin::ProtocolAds::AdsPos pos);
        void hideAds(E_Plugin_Platform platform, ADVERTISING_TYPE type);
        void setAdsListener(cocos2d::plugin::AdsListener* listener);
        virtual void updateProbability(std::map<std::string, std::string>& config, ADVERTISING_TYPE type) = 0;
        
        const std::vector<double>& getProbability(ADVERTISING_TYPE type);
    protected:      
        typedef std::map<ADVERTISING_TYPE, std::vector<double>> AdsProbability;
        AdsProbability m_adsProbalility;
    };
}

#endif