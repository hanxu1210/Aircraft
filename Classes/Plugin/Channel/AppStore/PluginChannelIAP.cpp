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
        m_pluginPlatforms.insert(std::make_pair(PP_IOSIAP, "IOSIAP"));
    }

    PluginChannelIAP::~PluginChannelIAP()
    {

    }

    bool PluginChannelIAP::init( void )
    {
        bool result = PluginIAP::init();
        ProtocolIAP* IAP = dynamic_cast<ProtocolIAP*>(m_pluginProtocol[PP_IOSIAP]);
        if (IAP)
        {
            std::map<std::string, std::string> info = {{"null","null"}};
            IAP->configDeveloperInfo(info);      
        }
        return result;
    }

    bool PluginChannelIAP::canMakePayments()
    {
        ProtocolIAP* protocolIAP = dynamic_cast<ProtocolIAP*>(m_pluginProtocol[PP_IOSIAP]);
        if (protocolIAP != nullptr)
        {
            return protocolIAP->callBoolFuncWithParam("canMakePayments", NULL);
        }
        return false;
    }

    void PluginChannelIAP::payForProduct( const std::string& productId )
    {
        ProtocolIAP* protocolIAP = dynamic_cast<ProtocolIAP*>(m_pluginProtocol[PP_IOSIAP]);
        if (protocolIAP != nullptr)
        {
            TProductInfo IAPInfo={{"productId", productId}};
            protocolIAP->payForProduct(IAPInfo);
        }
    }

    void PluginChannelIAP::requestProducts( const std::string& paralist )
    {
        ProtocolIAP* protocolIAP = dynamic_cast<ProtocolIAP*>(m_pluginProtocol[PP_IOSIAP]);
        if (protocolIAP != nullptr)
        {
            PluginParam productIdList = paralist.c_str();
            protocolIAP->callFuncWithParam("requestProducts", &productIdList, NULL);
        }
    }

    void PluginChannelIAP::restoreProducts()
    {
        ProtocolIAP* protocolIAP = dynamic_cast<ProtocolIAP*>(m_pluginProtocol[PP_IOSIAP]);
        if (protocolIAP != nullptr)
        {
            protocolIAP->callFuncWithParam("restoreProducts", NULL);
        }
    }

    bool PluginChannelIAP::isMusicEnabled()
    {
        return true;
    }

    void PluginChannelIAP::gameExit()
    {

    }

    void PluginChannelIAP::setResultListener( PayResultListener* listener )
    {
        ProtocolIAP* protocolIAP = dynamic_cast<ProtocolIAP*>(m_pluginProtocol[PP_IOSIAP]);
        if (protocolIAP)
        {
            protocolIAP->setResultListener(listener);
        }
    }

    void PluginChannelIAP::setSFResultListener(SFNativeIPayResulBack* listener)
    {


    }
    void PluginChannelIAP::setGameExitListener(SFNativeGameExitCallBack* listener){}
}


