#include "PluginAnalyticsWrapper.h"
#include "PluginManagerWrapper.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"

using namespace cocos2d::plugin;

namespace NsAircraft
{
    void PluginAnalyticsWrapper::PluginOnlineConfigListener:: onConfigDataReceived(const ConfigDataMap& data)
    {
        PluginManagerWrapper::sharedInstance()->updateConfig();  
    }

    PluginAnalyticsWrapper::PluginAnalyticsWrapper():
        m_pluginAnalytics(nullptr),
        m_onlineConfigListener(nullptr)
    {
        m_pluginAnalytics = new PluginChannelAnalytics();
    }

    PluginAnalyticsWrapper::~PluginAnalyticsWrapper()
    {
        CC_SAFE_DELETE(m_onlineConfigListener);
        CC_SAFE_RELEASE_NULL(m_pluginAnalytics);
    }

    bool PluginAnalyticsWrapper::init()
    {
        if (m_pluginAnalytics)
        {
            m_pluginAnalytics->init();
            m_onlineConfigListener = new PluginOnlineConfigListener;
            m_pluginAnalytics->setOnlineConfigListener(m_onlineConfigListener);
        }

        updateConfig();

        return true;
    }

    bool PluginAnalyticsWrapper::uninit()
    {
        return true;
    }

    void PluginAnalyticsWrapper::loadPlugin()
    {
        m_pluginAnalytics->loadPlugin();
    }

    void PluginAnalyticsWrapper::unloadPlugin()
    {
        m_pluginAnalytics->unloadPlugin();
    }

    void PluginAnalyticsWrapper::updateConfig()
    {

    }

    void PluginAnalyticsWrapper::setDebugMode(bool mode)
    {
        if (m_pluginAnalytics)
        {
            m_pluginAnalytics->setDebugMode(mode);
        }
    }

    std::string PluginAnalyticsWrapper::getOnlineConfig(const std::string& key)
    {
        std::string value = "";
        if (m_pluginAnalytics)
        {
             value = m_pluginAnalytics->getOnlineConfig(key);
             CCLOG("the onlineConfig value is %s", value.c_str());
        }
        return value;
    }

    void PluginAnalyticsWrapper::startSession()
    {
        if (m_pluginAnalytics)
        {
            m_pluginAnalytics->startSession();
        }
    }

    void PluginAnalyticsWrapper::stopSession()
    {
        if (m_pluginAnalytics)
        {
            m_pluginAnalytics->stopSession();
        }
    }

    void PluginAnalyticsWrapper::showFeedback()
    {
        if (m_pluginAnalytics)
        {
            m_pluginAnalytics->showFeedback();
        }
    }

    void PluginAnalyticsWrapper::startLevel(const std::string& level, int levelAmount)
    {
        if (m_pluginAnalytics)
        {
            std::string levelId = getLevelId(level, levelAmount).c_str();
            m_pluginAnalytics->startLevel(level, levelId);
        }
    }

    void PluginAnalyticsWrapper::finishLevel(const std::string& level, int levelAmount)
    {
        if (m_pluginAnalytics)
        {
            std::string levelId = getLevelId(level, levelAmount).c_str();
            m_pluginAnalytics->finishLevel(level, levelId);
        }
    }

    void PluginAnalyticsWrapper::failLevel(const std::string& level, int levelAmount)
    {
        if (m_pluginAnalytics)
        {
            std::string levelId = getLevelId(level, levelAmount).c_str();
            m_pluginAnalytics->failLevel(level, levelId);
        }
    }

    void PluginAnalyticsWrapper::buy(const std::string& itemId, int amount, double price)
    {
        if (m_pluginAnalytics)
        {
            m_pluginAnalytics->buy(itemId, amount, price);
        }
    }

    std::string PluginAnalyticsWrapper::getLevelId(const std::string& levelId, int levelAmount)
    {
        return cocos2d::StringUtils::format("%s-%02d", levelId.c_str(), levelAmount);
    }

}
