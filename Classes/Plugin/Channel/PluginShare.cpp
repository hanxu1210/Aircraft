#include "PluginShare.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "Utilities/Utilities.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginShare::PluginShare()
    {
    }

    PluginShare::~PluginShare()
    {
    }

    bool PluginShare::init()
    {
        return Plugin::init();
    }

    void PluginShare::share(const std::string& text, const std::string& image)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        ProtocolShare* protocolShare = dynamic_cast<ProtocolShare*>(m_pluginProtocol[PP_SHAREUMENG]);
        if (protocolShare != nullptr)
        {
            TShareInfo shareInfo={{"shareText", text}, {"shareImage", image}};
            protocolShare->share(shareInfo);
        }
#endif
    }

    void PluginShare::setResultListener( ShareResultListener* listener )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolShare* protocolShare = dynamic_cast<ProtocolShare*>(m_pluginProtocol[PP_SHAREUMENG]);
        if (protocolShare)
        {
            protocolShare->setResultListener(listener);
        }
#endif
    }

    void PluginShare::setShakeToShareListener( cocos2d::plugin::ShakeToShareListener* listener )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolShare* protocolShare = dynamic_cast<ProtocolShare*>(m_pluginProtocol[PP_SHAREUMENG]);
        if (protocolShare)
        {
            protocolShare->setShakeToShareListener(listener);
        }
#endif
    }



}