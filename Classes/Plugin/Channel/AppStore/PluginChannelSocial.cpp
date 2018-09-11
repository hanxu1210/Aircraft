#include "Plugin/Channel/PluginChannelSocial.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/SocialConfig.h"
#include "PluginParam.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginChannelSocial::PluginChannelSocial()
    {
        m_pluginPlatforms.insert(std::make_pair(PP_KTPLAY, "SocialKTPlay"));
        m_pluginPlatforms.insert(std::make_pair(PP_APPRATING, "AppRating"));
    }

    PluginChannelSocial::~PluginChannelSocial()
    {

    }

    bool PluginChannelSocial::init( void )
    {
        bool result = PluginSocial::init();
        ProtocolSocial* social = dynamic_cast<ProtocolSocial*>(m_pluginProtocol[PP_KTPLAY]);
        if (social)
        {
            TSocialDeveloperInfo info = {{"KTPlayAppKey", AppConfig::KTPlayAppkey}, {"KTPlayAppSecret", AppConfig::KTPlayAppSecret}};
            social->configDeveloperInfo(info);
        }

        social = dynamic_cast<ProtocolSocial*>(m_pluginProtocol[PP_APPRATING]);
        if (social)
        {
            TSocialDeveloperInfo info = {{"AppStoreId", SocialConfig::AppStoreId}, {"EventsUntilPrompt", SocialConfig::EventsUntilPrompt}, {"DaysUntilPrompt", SocialConfig::DaysUntilPrompt},
                {"UsesUntilPrompt", SocialConfig::UsesUntilPrompt}};
            social->configDeveloperInfo(info);
        }
        
        /*social = dynamic_cast<ProtocolSocial*>(m_pluginProtocol[PP_LUYOU]);
        if (social)
        {
            TSocialDeveloperInfo info = {{"LuyouAppKey", AppConfig::LuyouAppkey}};
            social->configDeveloperInfo(info);      
        }*/

        return result;
    }

    void PluginChannelSocial::appEnteredForeground()
    {
        ProtocolSocial* social = dynamic_cast<ProtocolSocial*>(m_pluginProtocol[PP_APPRATING]);
        if (social)
        {
            social->callFuncWithParam("appEnteredForeground", NULL);
        }
    }

    void PluginChannelSocial::logEvent()
    {
        ProtocolSocial* social = dynamic_cast<ProtocolSocial*>(m_pluginProtocol[PP_APPRATING]);
        if (social)
        {
            social->callFuncWithParam("logEvent", NULL);
        }
    }

}


