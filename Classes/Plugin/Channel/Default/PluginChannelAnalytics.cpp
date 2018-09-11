#include "Plugin/Channel/PluginChannelAnalytics.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/AnalyticsConfig.h"
#include "PluginParam.h"
#include "Localization/Localization.h"
#include "Utilities/GraphicUtility.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginChannelAnalytics::PluginChannelAnalytics()
    {
        m_pluginPlatforms.insert(std::make_pair(PP_ANALYTICSUMENG, "AnalyticsUmeng"));
    }

    PluginChannelAnalytics::~PluginChannelAnalytics()
    {

    }

    bool PluginChannelAnalytics::init( void )
    {
        bool result = PluginAnalytics::init();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolAnalytics* analytics = dynamic_cast<ProtocolAnalytics*>(m_pluginProtocol[PP_ANALYTICSUMENG]);
        if (analytics)
        {
            PluginParam appKey = AppConfig::UmengAppkey;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            PluginParam policy = BATCH;
            PluginParam channelId = "App Store";
            analytics->callFuncWithParam("startSessionWithParams", &appKey, &policy, &channelId, NULL);
#endif
            analytics->callFuncWithParam("checkUpdate", NULL);
            analytics->callFuncWithParam("updateOnlineConfig", NULL);

            analytics->callFuncWithParam("initFeedback", &appKey, NULL);
        }
#endif
        return result;
    }

}
