#include "PluginManagerWrapper.h"
#include "PluginManager.h"
#include "ProtocolShare.h"
#include "ProtocolAnalytics.h"
#include "ProtocolAds.h"
#include "ProtocolSocial.h"
#include "Config/AppConfig.h"
#include "Config/ShareConfig.h"
#include "PluginParam.h"
#include "PluginProtocolWrapper.h"
#include "PluginAnalyticsWrapper.h"
#include "PluginAdsWrapper.h"
#include "PluginShareWrapper.h"
#include "PluginSocialWrapper.h"
#include "PluginUserWrapper.h"
#include "PluginIAPWrapper.h"

using namespace cocos2d::plugin;

namespace NsAircraft
{
    static PluginManagerWrapper *s_sharedInstance = NULL;

    PluginManagerWrapper::PluginManagerWrapper()
    {
        memset(m_pluginProtocol, 0, sizeof(m_pluginProtocol));
    }

    PluginManagerWrapper::~PluginManagerWrapper()
    {
        uninit();
    }

    bool PluginManagerWrapper::init()
    {
        m_pluginProtocol[PP_ANALYTICS] = new PluginAnalyticsWrapper;
        m_pluginProtocol[PP_SHARE] = new PluginShareWrapper;
        m_pluginProtocol[PP_ADS] = new PluginAdsWrapper;
        //m_pluginProtocol[PP_SOCIAL] = new PluginSocialWrapper;
        //m_pluginProtocol[PP_USER] = new PluginUserWrapper;
        //m_pluginProtocol[PP_IAP] = new PluginIAPWrapper;
        loadAllPlugins();

#ifdef COCOS2D_DEBUG
        setDebugMode(true);
#endif
        for (auto& v : m_pluginProtocol)
        {
            if (v != nullptr)
            {
                v->init();
            }
        }
        return true;
    }

    bool PluginManagerWrapper::uninit(void)
    {
        unloadAllPlugins();
        for (auto& v : m_pluginProtocol)
        {
            if (v != nullptr)
            {
                v->uninit();
                v->release();
            }
        }
        return true;
    }

    void PluginManagerWrapper::loadAllPlugins()
    {
        for (auto& v : m_pluginProtocol)
        {
            if (v != nullptr)
                v->loadPlugin();
        }
    }

    void PluginManagerWrapper::unloadAllPlugins()
    {
        for (auto& v : m_pluginProtocol)
        {
            if (v != nullptr)
                v->unloadPlugin();
        }
    }

    void PluginManagerWrapper::setDebugMode(bool mode)
    {
        for (auto& v : m_pluginProtocol)
        {
            if (v)
            {
                v->setDebugMode(mode);
            }
        }
    }

    void PluginManagerWrapper::updateConfig()
    {
        for (auto& v : m_pluginProtocol)
        {
            if (v)
            {
                v->updateConfig();
            }
        }
    }

    PluginProtocolWrapper* PluginManagerWrapper::getProtocol(E_Plugin_Protocol protocol) const
    {
        return m_pluginProtocol[protocol];
    }

    PluginManagerWrapper *PluginManagerWrapper::sharedInstance(void)
    {
        if (!s_sharedInstance)
        {
            s_sharedInstance = new PluginManagerWrapper;
        }
        return s_sharedInstance;
    }

    void PluginManagerWrapper::purgeInstance(void)
    {
        CC_SAFE_RELEASE_NULL(s_sharedInstance);
    }

}
