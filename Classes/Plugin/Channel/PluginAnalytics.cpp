#include "PluginAnalytics.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "Utilities/Utilities.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginAnalytics::PluginAnalytics()
    {
    }

    PluginAnalytics::~PluginAnalytics()
    {
    }

    bool PluginAnalytics::init()
    {
        return Plugin::init();
    }

    std::string PluginAnalytics::getOnlineConfig(const std::string& key)
    {
        std::string value = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        PluginParam paramKey = key.c_str();
         ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            value = protocolAnalytics->callStringFuncWithParam("getConfigParams", &paramKey, NULL);
        }
#endif
        return value;
    }

    void PluginAnalytics::startSession()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            protocolAnalytics->startSession(AppConfig::UmengAppkey);
        }
#endif
    }

    void PluginAnalytics::stopSession()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            protocolAnalytics->stopSession();
        }
#endif
    }

    void PluginAnalytics::showFeedback()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            protocolAnalytics->callFuncWithParam("showFeedback", NULL);
        }
#endif
    }

    void PluginAnalytics::startLevel(const std::string& level, const std::string& levelId)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            PluginParam paramLevel = levelId.c_str();
            protocolAnalytics->callFuncWithParam("startLevel", &paramLevel, NULL);
        }
#endif
    }

    void PluginAnalytics::finishLevel(const std::string& level, const std::string& levelId)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            PluginParam paramLevel = levelId.c_str();
            protocolAnalytics->callFuncWithParam("finishLevel", &paramLevel, NULL);
        }
#endif
    }

    void PluginAnalytics::failLevel(const std::string& level, const std::string& levelId)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            PluginParam paramLevel = levelId.c_str();
            protocolAnalytics->callFuncWithParam("failLevel", &paramLevel, NULL);
        }
#endif
    }

    void PluginAnalytics::buy(const std::string& itemId, int amount, double price)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            PluginParam paramItemId = itemId.c_str();
            PluginParam paramAmount = amount;
            PluginParam paramPrice = (float)price;
            protocolAnalytics->callFuncWithParam("buy", &paramItemId, &paramAmount, &paramPrice, NULL);
        }
#endif
    }

    void PluginAnalytics::setOnlineConfigListener( OnlineConfigListener* listener )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* protocolAnalytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (protocolAnalytics)
        {
            protocolAnalytics->setOnlineConfigListener(listener);
        }
#endif
    }
}