#include "Plugin/Channel/PluginChannelShare.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/ShareConfig.h"
#include "PluginParam.h"
#include "Localization/Localization.h"
#include "Utilities/GraphicUtility.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginChannelShare::PluginChannelShare()
    {
        m_pluginPlatforms.insert(std::make_pair(PP_SHAREUMENG, "ShareUmeng"));
    }

    PluginChannelShare::~PluginChannelShare()
    {

    }

    bool PluginChannelShare::init( void )
    {
        bool result = PluginShare::init();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ProtocolShare* share = dynamic_cast<ProtocolShare*>(m_pluginProtocol[PP_SHAREUMENG]);
        if (share)
        {
            TShareDeveloperInfo info = {{"UmengAppKey", NsAircraft::AppConfig::UmengAppkey}, {"OffcialSinaWeiboUserId", NsAircraft::ShareConfig::OffcialSinaWeiboUserId},
            {"OffcialTencentWeiboUserId", NsAircraft::ShareConfig::OffcialTencentWeiboUserId}};
            share->configDeveloperInfo(info);
            PluginParam targetUrl = NsAircraft::ShareConfig::ShareTargetUrl;
            share->callFuncWithParam("setTargetUrl", &targetUrl, NULL);
            share->callFuncWithParam("supportSinaWeiboPlatform", NULL);
            PluginParam redirectUrl = NsAircraft::ShareConfig::ShareTencentWeiboRedirectUrl;
            share->callFuncWithParam("supportTencentWeiboPlatform", &redirectUrl, NULL);
            PluginParam appID = NsAircraft::ShareConfig::ShareWeixinAppID;
            PluginParam appSecret = NsAircraft::ShareConfig::ShareWeixinAppSecret;
            cocos2d::Value shareTitle = Localization::sharedInstance()->getValue(ShareConfig::ShareDefaultTitle);
            cocos2d::Value shareText = Localization::sharedInstance()->getValue(ShareConfig::ShareDefaultText);
            PluginParam title = shareTitle.asString().c_str();
            PluginParam content = shareText.asString().c_str();
            PluginParam image = NsAircraft::ShareConfig::ShareDefaultImage;
            share->callFuncWithParam("supportWeiXinPlatform", &appID, &appSecret, &title, &content, &image, NULL);

            appID = NsAircraft::ShareConfig::ShareQQAppID;
            PluginParam appKey = NsAircraft::ShareConfig::ShareQQAppKey;
            share->callFuncWithParam("supportQQPlatform", &appID, &appKey, &title, &content, &image, NULL);

            appID = NsAircraft::ShareConfig::ShareRenrenAppID;
            appKey = NsAircraft::ShareConfig::ShareRenrenAppKey;
            PluginParam secret = NsAircraft::ShareConfig::ShareRenrenAppSecret;   
            share->callFuncWithParam("supportRenrenPlatform", &appID, &appKey, &secret, NULL);
            share->callFuncWithParam("supportDoubanPlatform", NULL);
            //m_pluginShareProtocol->callFuncWithParam("supportSmsPlatform", NULL);
            share->callFuncWithParam("supportEmailPlatform", NULL);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + NsAircraft::ShareConfig::ShakeScreenShot;
            PluginParam fileName = path.c_str();
            share->callFuncWithParam("setScreenShotPath", &fileName, NULL);
#endif
            share->callFuncWithParam("shakeToShare", &content, NULL);
        }
#endif
        return result;
    }

}