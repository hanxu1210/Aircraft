#include "PluginAds.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "Utilities/Utilities.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{
 
    PluginAds::PluginAds()
    {
    }

    PluginAds::~PluginAds()
    {
        m_adsProbalility.clear();
    }

    bool PluginAds::init()
    {
        return Plugin::init();
    }

    void PluginAds::showAds(E_Plugin_Platform platform, ADVERTISING_TYPE type, ADVERTISING_SIZE size, cocos2d::plugin::ProtocolAds::AdsPos pos)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        ProtocolAds* protocolAds = dynamic_cast<ProtocolAds*>(m_pluginProtocol[platform]);
        if (protocolAds != nullptr)
        {
            std::map<std::string, std::string> info = {{"AdsType", IntToString(type)}, {"AdsSizeEnum", IntToString(size)}};
            protocolAds->showAds(info, pos);
        }
#endif
    }
    
    void PluginAds::preloadAds(E_Plugin_Platform platform, ADVERTISING_TYPE type, ADVERTISING_SIZE size, cocos2d::plugin::ProtocolAds::AdsPos pos)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        ProtocolAds* protocolAds = dynamic_cast<ProtocolAds*>(m_pluginProtocol[platform]);
        if (protocolAds != nullptr)
        {
            PluginParam adsType = type;
            PluginParam adsSize = size;
            PluginParam adsPos = pos;
            protocolAds->callFuncWithParam("preloadAds", &adsType, &adsSize, &adsPos, NULL);
        }
#endif
    }

    void PluginAds::hideAds(E_Plugin_Platform platform, ADVERTISING_TYPE type)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAds* protocolAds = dynamic_cast<ProtocolAds*>(m_pluginProtocol[platform]);
        if (protocolAds != nullptr)
        {
            std::map<std::string, std::string> info = {{"AdsType", IntToString(type)}};
            protocolAds->hideAds(info);
        }
#endif
    }

    void PluginAds::setAdsListener( AdsListener* listener )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        for (auto& kv : m_pluginProtocol)
        {
            ProtocolAds* protocolAds = dynamic_cast<ProtocolAds*>(kv.second);
            if (protocolAds)
            {
                protocolAds->setAdsListener(listener);
            }
        }
#endif
    }

    const std::vector<double>& PluginAds::getProbability( ADVERTISING_TYPE type )
    {
        return m_adsProbalility[type];
    }

}