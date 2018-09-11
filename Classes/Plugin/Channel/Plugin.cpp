#include "Plugin.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "Utilities/Utilities.h"


using namespace cocos2d::plugin;

namespace NsAircraft
{
    Plugin::Plugin() 
    { 
    }

    Plugin::~Plugin() 
    { 
        m_pluginProtocol.clear();
        m_pluginPlatforms.clear();
    }

    bool Plugin::init(void)
    {
        return true;
    }

    bool Plugin::uninit(void)
    {     
        return true;
    }

    void Plugin::loadPlugin()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        for (auto &kv : m_pluginPlatforms)
        {
            m_pluginProtocol[kv.first] = PluginManager::getInstance()->loadPlugin(kv.second.c_str());
        }
#endif
    }

    void Plugin::unloadPlugin()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        for (auto &kv : m_pluginPlatforms)
        {
            PluginManager::getInstance()->unloadPlugin(kv.second.c_str());
        }
#endif
    }

    void Plugin::setDebugMode(bool mode)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        for (auto& kv : m_pluginProtocol)
        {
            if (kv.second)
            {
                kv.second->setDebugMode(mode);
            }
        }
#endif     
    }

}