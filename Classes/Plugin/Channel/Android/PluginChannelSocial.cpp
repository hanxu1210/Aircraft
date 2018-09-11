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
        m_pluginPlatforms.insert(std::make_pair(PP_KTPLAY, "SocialLuyou"));
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

        
        social = dynamic_cast<ProtocolSocial*>(m_pluginProtocol[PP_LUYOU]);
        if (social)
        {
            TSocialDeveloperInfo info = {{"LuyouAppKey", AppConfig::LuyouAppkey}};
            social->configDeveloperInfo(info);      
        }

        return result;
    }

    void PluginChannelSocial::appEnteredForeground()
    {

    }

    void PluginChannelSocial::logEvent()
    {

    }
}


