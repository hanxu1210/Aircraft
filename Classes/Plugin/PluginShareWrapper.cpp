#include "PluginShareWrapper.h"
#include "PluginManagerWrapper.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/ShareConfig.h"
#include "PluginParam.h"
#include "Localization/Localization.h"
#include "Utilities/GraphicUtility.h"

using namespace cocos2d::plugin;

namespace NsAircraft
{
    const char* PluginShareWrapper::EVENT_SHAKE_ACTION_COMPLETE = "event_shake_action_complete";
    const char* PluginShareWrapper::EVENT_SHAKE_CLOSED = "event_shake_closed";

    void PluginShareWrapper::PluginShareListener::onShareResult(ShareResultCode ret, const char* msg)
    {
       
    }

    void PluginShareWrapper::PluginShakeLister::onShakeActionComplete()
    {
         PluginShareWrapper* pluginShare = dynamic_cast<PluginShareWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_SHARE));
         if (pluginShare)
         {
             pluginShare->onShakeActionComplete();
         }
         
    }

    void PluginShareWrapper::PluginShakeLister::onShakeClosed() 
    {
        PluginShareWrapper* pluginShare = dynamic_cast<PluginShareWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_SHARE));
        if (pluginShare)
        {
            pluginShare->onShakeClosed();
        }
    }


    PluginShareWrapper::PluginShareWrapper():
        m_pluginShare(nullptr),
        m_shareListener(nullptr),
        m_shakeListener(nullptr),
        m_eventShakeActionComplete(nullptr),
        m_eventShakeClosed(nullptr)
    {
        m_pluginShare = new PluginChannelShare();
    }

    PluginShareWrapper::~PluginShareWrapper()
    {
        CC_SAFE_DELETE(m_shareListener);
        CC_SAFE_DELETE(m_shakeListener);
        CC_SAFE_RELEASE_NULL(m_pluginShare);
        CC_SAFE_RELEASE_NULL(m_eventShakeActionComplete);
        CC_SAFE_RELEASE_NULL(m_eventShakeClosed);
    }

    bool PluginShareWrapper::init()
    {
        updateConfig();

        if (m_pluginShare)
        {
            m_pluginShare->init();
            m_shareListener = new PluginShareListener;
            m_shakeListener = new PluginShakeLister;
            m_pluginShare->setResultListener(m_shareListener);
            m_pluginShare->setShakeToShareListener(m_shakeListener);
            m_eventShakeActionComplete = new cocos2d::EventCustom(EVENT_SHAKE_ACTION_COMPLETE);
            m_eventShakeClosed = new cocos2d::EventCustom(EVENT_SHAKE_CLOSED);
        }
        return true;
    }

    bool PluginShareWrapper::uninit()
    {
        return true;
    }

    void PluginShareWrapper::loadPlugin()
    {
       m_pluginShare->loadPlugin();
    }

    void PluginShareWrapper::unloadPlugin()
    {
       m_pluginShare->unloadPlugin();
    }

    void PluginShareWrapper::share(const std::string& text, const std::string& image)
    {
        if (m_pluginShare)
        {
            m_pluginShare->share(text, image);
        }
    }

    void PluginShareWrapper::updateConfig()
    {

    }

    void PluginShareWrapper::setDebugMode(bool mode)
    {
        if (m_pluginShare)
        {
            m_pluginShare->setDebugMode(mode);
        }
    }

    void PluginShareWrapper::onShakeActionComplete()
    {
        GraphiccUtility::saveScreenshot(NsAircraft::ShareConfig::ShakeScreenShot);
        CCLOG("PluginShareWrapper::onShakeActionComplete");
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventShakeActionComplete);
    }

    void PluginShareWrapper::onShakeClosed()
    {
        CCLOG("PluginShareWrapper::onShakeClosed");
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventShakeClosed);
    }

}
