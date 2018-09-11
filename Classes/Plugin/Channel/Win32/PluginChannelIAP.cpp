#include "Plugin/Channel/PluginChannelIAP.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/IAPConfig.h"
#include "PluginParam.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginChannelIAP::PluginChannelIAP()
    {

    }

    PluginChannelIAP::~PluginChannelIAP()
    {

    }

    bool PluginChannelIAP::init( void )
    {
        bool result = PluginIAP::init();

        return result;
    }

    bool PluginChannelIAP::canMakePayments()
    {
        return false;
    }

    void PluginChannelIAP::payForProduct( const std::string& productId )
    {

    }

    void PluginChannelIAP::requestProducts( const std::string& paralist )
    {

    }

    void PluginChannelIAP::restoreProducts()
    {

    }

    bool PluginChannelIAP::isMusicEnabled()
    {
        return true;
    }

    void PluginChannelIAP::gameExit()
    {

    }

    void PluginChannelIAP::setResultListener( cocos2d::plugin::PayResultListener* listener )
    {

    }

    void PluginChannelIAP::setSFResultListener( SFNativeIPayResulBack* listener )
    {

    }

    void PluginChannelIAP::setGameExitListener( SFNativeGameExitCallBack* listener )
    {

    }
}


