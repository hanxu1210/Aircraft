#include "CommonHeaders.h"

#include "PluginAdsWrapper.h"
#include "PluginAnalyticsWrapper.h"
#include "PluginManagerWrapper.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "Utilities/Utilities.h"
#include <sstream>
#include <iostream>
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{
    const std::string PluginAdsWrapper::typeNames[] = {"AdsTypeBanner", "AdsTypeFullScreen", "AdsTypePopup", "AdsTypeOfferWall", "AdsTypeTestBanner", "AdsTypeTestFullScreen", "AdsTypeTestPopup", "AdsTypeTestOfferWall"};
    const char* PluginAdsWrapper::EVENT_ADS_CLICKED = "event_ads_clicked";
    const char* PluginAdsWrapper::EVENT_ADS_ENTERLANDPAGE = "event_ads_enterlandpage";
    const char* PluginAdsWrapper::EVENT_ADS_EXITLANDPAGE = "event_ads_exitlandpage";
    void PluginAdsWrapper::PluginAdsListener::onAdsResult(AdsResultCode code, const char* msg)
    {
        PluginAdsWrapper* pluginAds = dynamic_cast<PluginAdsWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_ADS));
        switch (code)
        {
        case kUnknownError:
            {
                if (pluginAds)
                {
                    pluginAds->retry(msg);
                }
            }
            break;
        case kAdsClicked:
            {
                if (pluginAds)
                {
                    pluginAds->onAdsClicked(msg);
                }
            }
            break;
        case kAdsReceived:
        case kAdsShown:
            {
                if (pluginAds)
                {
                    pluginAds->onAdsReceived(msg);
                }
            }
            break;
        case kAdsDismissed:
            {
                if (pluginAds)
                {
                    pluginAds->onAdsDismissed(msg);
                }
            }
            break;
        case kAdsEntenLandPage:
            {
                if (pluginAds)
                {
                    pluginAds->onAdsEntenLandPage(msg);
                }
            }
            break;
        case kAdsExitLandPage:
            {
                if (pluginAds)
                {
                    pluginAds->onAdsExitLandPage(msg);
                }
            }
            break;
        case kAdsLeaveApplication:
            {
                
            }
            break;
        default:
            break;
        }
    }

    PluginAdsWrapper::PluginAdsWrapper():
        m_currentAds(PluginChannelAds::AP_INVALID),
        m_maxRetry(2),
        m_adsListener(nullptr),
        m_isEnableAds(false),
        m_pluginAds(nullptr),
        m_eventAdsClicked(nullptr),
        m_eventAdsEntenLandPage(nullptr),
        m_eventAdsExitLandPage(nullptr)
    {
        m_adsListener = new PluginAdsListener;
        m_pluginAds = new PluginChannelAds();

        for(size_t i = 0; i < 3; ++i)
        {
            m_adsInfos[typeNames[i]].retryCount = 0;
        }
    }

    PluginAdsWrapper::~PluginAdsWrapper()
    {
        CC_SAFE_DELETE(m_adsListener);
        CC_SAFE_RELEASE_NULL(m_pluginAds);
        CC_SAFE_RELEASE_NULL(m_eventAdsClicked);
        CC_SAFE_RELEASE_NULL(m_eventAdsEntenLandPage);
        CC_SAFE_RELEASE_NULL(m_eventAdsExitLandPage);
    }

    bool PluginAdsWrapper::init()
    {
        updateConfig();
       
        PluginChannelAds::E_Ads_Platform platform = m_currentAds;
        for (size_t i = AT_BANNER; i < AT_COUNT; ++i)
        {
            ADVERTISING_TYPE type = static_cast<ADVERTISING_TYPE>(i);
            platform = getSelectedAds(type);
            m_adsSelectedPlatform.insert(std::make_pair(type, platform));
        }

        m_pluginAds->init();
        m_pluginAds->setAdsListener(m_adsListener);

        m_eventAdsClicked = new cocos2d::EventCustom(EVENT_ADS_CLICKED);
        m_eventAdsEntenLandPage = new cocos2d::EventCustom(EVENT_ADS_ENTERLANDPAGE);
        m_eventAdsExitLandPage = new cocos2d::EventCustom(EVENT_ADS_EXITLANDPAGE);
       
        preloadAds(AT_POPUP, AS_SMART_BANNER, ProtocolAds::kPosCenter);
        return true;
    }

    bool PluginAdsWrapper::uninit()
    {

        return true;
    }

    void PluginAdsWrapper::loadPlugin()
    {
        m_pluginAds->loadPlugin();
    }

    void PluginAdsWrapper::unloadPlugin()
    {
        m_pluginAds->unloadPlugin();
    }

    void PluginAdsWrapper::updateConfig()
    {
        PluginAnalyticsWrapper* pluginAnalytics = dynamic_cast<PluginAnalyticsWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_ANALYTICS));
        if (pluginAnalytics)
        {
            std::string probabilites;
            std::map<std::string, std::string> valueMap;
            int test = 0;
#ifdef COCOS2D_DEBUG
            test = 4;
#endif
            for (size_t i = AT_BANNER; i < AT_COUNT; ++i)
            {
                probabilites = pluginAnalytics->getOnlineConfig(typeNames[i - 1 + test]);
                if (!probabilites.empty())
                {
                    valueMap = getMapFromString(probabilites);
                    m_pluginAds->updateProbability(valueMap, static_cast<ADVERTISING_TYPE>(i));
                }
            }
            std::string maxRetry = pluginAnalytics->getOnlineConfig("AdsMaxRetry");
            if (!maxRetry.empty())
            {
                m_maxRetry = atoi(maxRetry.c_str());
            }

            std::string adsEnabled = pluginAnalytics->getOnlineConfig("AdsEnabled");
            if (!adsEnabled.empty())
            {
                std::istringstream is(adsEnabled);
                is >> std::boolalpha >> m_isEnableAds;
            }
        }
    }

    void PluginAdsWrapper::setDebugMode(bool mode)
    {
        m_pluginAds->setDebugMode(mode);
    }

    void PluginAdsWrapper::showAds(ADVERTISING_TYPE type, ProtocolAds::AdsPos pos)
    {
        for(size_t i = 0; i < 3; ++i)
        {
            m_adsInfos[typeNames[i]].retryCount = 0;
        }
        showAds(type, AS_SMART_BANNER, pos);
    }

    void PluginAdsWrapper::showAds(ADVERTISING_TYPE type, ADVERTISING_SIZE size, ProtocolAds::AdsPos pos)
    {
        if (isEnableAds())
        {
            m_currentAds = m_adsSelectedPlatform[type];
            if (m_currentAds == PluginChannelAds::AP_INVALID)
            {
                return;
            }
            Plugin::E_Plugin_Platform platform = m_pluginAds->getPluginPlatform(m_currentAds);
           
            m_pluginAds->showAds(platform, type, size, pos);

            std::string typeName = typeNames[type - 1];

            m_adsInfos[typeName].type = type;
            m_adsInfos[typeName].size = size;
            m_adsInfos[typeName].pos = pos;
            
        }
    }
    
    void PluginAdsWrapper::preloadAds(ADVERTISING_TYPE type, ADVERTISING_SIZE size, cocos2d::plugin::ProtocolAds::AdsPos pos)
    {
        if (isEnableAds())
        {
            m_currentAds = m_adsSelectedPlatform[type];
            if (m_currentAds == PluginChannelAds::AP_INVALID)
            {
                return;
            }
            Plugin::E_Plugin_Platform platform = m_pluginAds->getPluginPlatform(m_currentAds);
            
            m_pluginAds->preloadAds(platform, type, size, pos);
            
            std::string typeName = typeNames[type - 1];
            m_adsInfos[typeName].type = type;
            m_adsInfos[typeName].size = size;
            m_adsInfos[typeName].pos = pos;
            
            CCLOG("showAds retryCount = %d",  m_adsInfos[typeName].retryCount);
        }
    }

    void PluginAdsWrapper::hideAds(const std::string& typeName)
    {
        AdsInfo& info = m_adsInfos[typeName];
        hideAds(info.type);
    }
    
    void PluginAdsWrapper::hideAds(ADVERTISING_TYPE type)
    {
        m_currentAds = m_adsSelectedPlatform[type];
        if (m_currentAds == PluginChannelAds::AP_INVALID)
        {
            return;
        }
        Plugin::E_Plugin_Platform platform = m_pluginAds->getPluginPlatform(m_currentAds);       
        m_pluginAds->hideAds(platform, type);
        //std::string typeName = typeNames[type - 1];
        //m_adsInfos[typeName].retryCount = m_maxRetry;
    }

    void PluginAdsWrapper::disableAds()
    {
        m_isEnableAds = false;
    }

    void PluginAdsWrapper::enableAds()
    {
        m_isEnableAds = true;
    }

    bool PluginAdsWrapper::isEnableAds()
    {
        return true;
    }

    PluginChannelAds::E_Ads_Platform PluginAdsWrapper::getAdsPlatform(ADVERTISING_TYPE type)
    {
        return m_adsSelectedPlatform[type];
    }

    void PluginAdsWrapper::retry(const std::string& typeName)
    {
        AdsInfo& info = m_adsInfos[typeName];
        CCLOG("retry retryCount = %d",  info.retryCount);
        if (isEnableAds() && !info.isShowing && info.retryCount++ < m_maxRetry)
        {
            hideAds(typeName);
            int currentIndex = m_adsSelectedPlatform[info.type];
            currentIndex = ++currentIndex % PluginChannelAds::AP_COUNT;
            m_adsSelectedPlatform[info.type] = static_cast<PluginChannelAds::E_Ads_Platform>(currentIndex);
            const std::vector<double>& probability = m_pluginAds->getProbability(info.type);
            if (currentIndex < probability.size())
            {
                if (probability[currentIndex] >= 0.0f)
                {
                    if (info.type == AT_BANNER)
                    {
                        showAds(info.type, info.size, info.pos);
                    }
                    else
                    {
                        preloadAds(info.type, info.size, info.pos);
                    }
                    
                }
                else
                {
                    CCLOG("the platfome %d of ads type %d probability less than 0", currentIndex, info.type);
                }
            }
        }
    }

    PluginChannelAds::E_Ads_Platform PluginAdsWrapper::getSelectedAds(ADVERTISING_TYPE type) 
    {
        const std::vector<double>& probability = m_pluginAds->getProbability(type);
        unsigned int ads = Fitness_proportionate_selection(probability);
        if (ads == -1)
        {
            return PluginChannelAds::AP_INVALID;
        }
        return static_cast<PluginChannelAds::E_Ads_Platform>(ads);
    }

    std::map<std::string, std::string> PluginAdsWrapper::getMapFromString(const std::string& content)
    {
        std::map<std::string, std::string> valueMap;
        std::vector<std::string> elems;
        std::istringstream  ss(content);
        std::string item, key, value;
        size_t pos;
        while (std::getline(ss, item, ';')) {
           pos = item.find_first_of("=");
           if (pos != std::string::npos)
           {
               key = item.substr(0, pos);
               value = item.substr(pos + 1);
               valueMap[key] = value;
           }
        }

        return valueMap;
    }

    void PluginAdsWrapper::onAdsClicked(const std::string& typeName)
    {
        AdsInfo& info = m_adsInfos[typeName];
        PluginChannelAds::E_Ads_Platform platform = m_adsSelectedPlatform[info.type];

        ClickedInfo clickInfo;
        clickInfo.type = info.type;
        clickInfo.platform = platform;
        m_eventAdsClicked->setUserData(&clickInfo);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventAdsClicked);
    }
    
    void PluginAdsWrapper::onAdsReceived(const std::string& typeName)
    {
        AdsInfo& info = m_adsInfos[typeName];
        info.isShowing = true;
    }
    
    void PluginAdsWrapper::onAdsDismissed(const std::string& typeName)
    {
        AdsInfo& info = m_adsInfos[typeName];
        info.isShowing = false;
        preloadAds(info.type, info.size, info.pos);
    }
    
    void PluginAdsWrapper::onAdsEntenLandPage(const std::string& typeName)
    {
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventAdsEntenLandPage);
    }
    
    void PluginAdsWrapper::onAdsExitLandPage(const std::string& typeName)
    {
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventAdsExitLandPage);
    }
}
