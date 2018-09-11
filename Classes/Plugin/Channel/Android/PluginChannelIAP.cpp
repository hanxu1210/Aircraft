#include "Plugin/Channel/PluginChannelIAP.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/IAPConfig.h"
#include "PluginParam.h"
#include "Plugin/YiJie/SFGameNativeInterface.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginChannelIAP::PluginChannelIAP()
    {
        m_pluginPlatforms.insert(std::make_pair(PP_YIJIE, "IAPYiJie"));
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
        return true;
    }

    void PluginChannelIAP::payForProduct( const std::string& productId )
    {
        SFGameNativeInterface::pay(productId.c_str());
    }

    void PluginChannelIAP::requestProducts( const std::string& paralist )
    {

    }

    void PluginChannelIAP::restoreProducts()
    {

    }

    bool PluginChannelIAP::isMusicEnabled()
    {
        return SFGameNativeInterface::isMusicEnabled();
    }

    void PluginChannelIAP::gameExit()
    {
        SFGameNativeInterface::onExit();
    }

    void PluginChannelIAP::setResultListener(cocos2d::plugin::PayResultListener* listener)
    {

    }

    void PluginChannelIAP::setSFResultListener( SFNativeIPayResulBack* listener )
    {
        SFGameNativeInterface::setSFIPayResulBack(listener);
    }

    void PluginChannelIAP::setGameExitListener(SFNativeGameExitCallBack* listener)
    {
        SFGameNativeInterface::setSFGameExitCallBack(listener);
    }
}


