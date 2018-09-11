#ifndef _RUSHGIRL_PLUGIN_CHANNEL_ADS_H_
#define _RUSHGIRL_PLUGIN_CHANNEL_ADS_H_

#include "cocos2d.h"
#include "PluginAds.h"
#include "Config/AdvertisingType.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginChannelAds : public PluginAds
    {
    public:
        enum E_Ads_Platform
        {
            AP_O2OMOBI,
            AP_ADCHINA,
            AP_ADMOB,
            AP_COUNT,
            AP_INVALID
        };
        /** PluginChannelAds constructor */
        PluginChannelAds();

        /** PluginChannelAds destructor */
        virtual ~PluginChannelAds();

        virtual bool init(void) override;

        virtual void updateProbability(std::map<std::string, std::string>& config, ADVERTISING_TYPE type) override;

        E_Plugin_Platform getPluginPlatform(E_Ads_Platform ads);
    };
}

#endif