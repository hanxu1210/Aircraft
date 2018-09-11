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
        m_pluginPlatforms.insert(std::make_pair(PP_O2OMOBI, "Adso2omobi"));
        m_pluginPlatforms.insert(std::make_pair(PP_ADCHINA, "AdsAdChina"));
        m_pluginPlatforms.insert(std::make_pair(PP_ADMOB, "AdsAdmob"));

        std::vector<double> probabilities;
        probabilities.push_back(0.0);
        probabilities.push_back(0.0);
        probabilities.push_back(1.0);
        m_adsProbalility.insert(std::make_pair(AT_BANNER, probabilities));
        m_adsProbalility.insert(std::make_pair(AT_FULLSCREEN, probabilities));
        m_adsProbalility.insert(std::make_pair(AT_POPUP, probabilities));
        m_adsProbalility.insert(std::make_pair(AT_OFFERWALL, probabilities));
    }

    PluginChannelAds::~PluginChannelAds()
    {

    }

    bool PluginChannelAds::init( void )
    {
        bool result = PluginAds::init();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::map<std::string, std::string> info = {{"o2omobiAppKey", AppConfig::o2omobiAppkey}, {"o2omobiAppSecret", AppConfig::o2omobiAppSecret}};
        ProtocolAds* ads = dynamic_cast<ProtocolAds*>(m_pluginProtocol[PP_O2OMOBI]);
        if (ads)
        {
            ads->configDeveloperInfo(info);
        }
        ads = dynamic_cast<ProtocolAds*>(m_pluginProtocol[PP_ADCHINA]);
        if (ads)
        {
            const char* AdChinaBannerID = nullptr;
            const char* AdChinaFullScreenID = nullptr;
            const char* AdChinaPopupID = nullptr;
            ApplicationProtocol::Platform platform = Application::getInstance()->getTargetPlatform();
            if (ApplicationProtocol::Platform::OS_IPHONE == platform || ApplicationProtocol::Platform::OS_ANDROID == platform)
            {
                AdChinaBannerID = AppConfig::AdChinaBannerID;
                AdChinaFullScreenID = AppConfig::AdChinaFullScreenID;
                AdChinaPopupID = AppConfig::AdChinaPopupID;
            }
            else if (ApplicationProtocol::Platform::OS_IPAD == platform || ApplicationProtocol::Platform::OS_ANDROID == platform)
            {
                AdChinaBannerID = AppConfig::AdChinaPadBannerID;
                AdChinaFullScreenID = AppConfig::AdChinaPadFullScreenID;
                AdChinaPopupID = AppConfig::AdChinaPadPopupID;
            }

            std::map<std::string, std::string> info = {{"AdChinaBannerID", AdChinaBannerID}, {"AdChinaFullScreenID", AdChinaFullScreenID},{"AdChinaPopupID", AdChinaPopupID}};
            ads->configDeveloperInfo(info);
        }
        
        ads = dynamic_cast<ProtocolAds*>(m_pluginProtocol[PP_ADMOB]);
        if (ads)
        {
            std::map<std::string, std::string> infoAdmob = {{"AdmobBannerID", AppConfig::AdmobBannerID}, {"AdmobInterstitialID", AppConfig::AdmobInterstitialID}};
            ads->configDeveloperInfo(infoAdmob);
            for(size_t i = 0; i < AppConfig::AdmobTestDeviceCount; ++i)
            {
                PluginParam deviceId = AppConfig::AdmobTestDevices[i];
                ads->callFuncWithParam("addTestDevice", &deviceId, NULL);
            }
           
        }
#endif

        return result;
    }

    void PluginChannelAds::updateProbability( std::map<std::string, std::string>& config, ADVERTISING_TYPE type )
    {
        std::vector<double> platform;
        platform.push_back(atof(config["o2omobi"].c_str()));
        platform.push_back(atof(config["AdChina"].c_str()));
        platform.push_back(atof(config["Admob"].c_str()));
        m_adsProbalility[type] = platform;
    }

    Plugin::E_Plugin_Platform PluginChannelAds::getPluginPlatform( E_Ads_Platform ads )
    {
        E_Plugin_Platform plugin = Plugin::PP_INVALID;
        switch (ads)
        {
        case AP_O2OMOBI:
            plugin = PP_O2OMOBI;
            break;
        case AP_ADCHINA:
            plugin = PP_ADCHINA;
            break;
        case AP_ADMOB:
            plugin = PP_ADMOB;
            break;
        default:
            break;
        }
        return plugin;
    }



}