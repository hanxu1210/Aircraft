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

    }

    PluginChannelSocial::~PluginChannelSocial()
    {

    }

    bool PluginChannelSocial::init( void )
    {
        bool result = PluginSocial::init();
       

        return result;
    }

    void PluginChannelSocial::appEnteredForeground()
    {

    }

    void PluginChannelSocial::logEvent()
    {

    }

}


