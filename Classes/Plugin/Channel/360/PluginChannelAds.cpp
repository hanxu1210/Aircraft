#include "CommonHeaders.h"

#include "Plugin/Channel/PluginChannelAds.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginChannelAds::PluginChannelAds()
    {
       
    }

    PluginChannelAds::~PluginChannelAds()
    {

    }

    bool PluginChannelAds::init( void )
    {
        bool result = PluginAds::init();

        return result;
    }

    void PluginChannelAds::updateProbability( std::map<std::string, std::string>& config, ADVERTISING_TYPE type )
    {
       
    }

    Plugin::E_Plugin_Platform PluginChannelAds::getPluginPlatform( E_Ads_Platform ads )
    {
        E_Plugin_Platform plugin = Plugin::PP_INVALID;
       
        return plugin;
    }



}