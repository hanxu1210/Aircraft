#include "PluginUserWrapper.h"
#include "PluginManagerWrapper.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/UserConfig.h"
#include "Config/IAPConfig.h"
#include "PluginParam.h"
#include "CommonHeaders.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::plugin;
using namespace cocos2d::network;

namespace NsAircraft
{
    const char* PluginUserWrapper::EVENT_LOGIN_SUCCESS = "event_login_success";
    const char* PluginUserWrapper::EVENT_LOGIN_FAILED = "event_login_failed";
    const char* PluginUserWrapper::EVENT_LOGOUT = "event_logout";
    const char* PluginUserWrapper::EVENT_GAME_EXIT = "event_game_exit";

    PluginUserWrapper::PluginUserWrapper():
        m_eventLoginSuccess(nullptr),
        m_eventLoginFailed(nullptr),
        m_eventLogout(nullptr),
        m_eventGameExit(nullptr)
    {
    }

    PluginUserWrapper::~PluginUserWrapper()
    {
        CC_SAFE_RELEASE_NULL(m_eventLoginSuccess);
        CC_SAFE_RELEASE_NULL(m_eventLoginFailed);
        CC_SAFE_RELEASE_NULL(m_eventLogout);
        CC_SAFE_RELEASE_NULL(m_eventGameExit);
    }

    bool PluginUserWrapper::init()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        updateConfig();

        m_eventLoginSuccess = new cocos2d::EventCustom(EVENT_LOGIN_SUCCESS);
        m_eventLoginFailed = new cocos2d::EventCustom(EVENT_LOGIN_FAILED);
        m_eventLogout = new cocos2d::EventCustom(EVENT_LOGOUT);
        m_eventGameExit = new cocos2d::EventCustom(EVENT_GAME_EXIT);
#endif
        return true;
    }

    bool PluginUserWrapper::uninit()
    {
        return true;
    }

    void PluginUserWrapper::loadPlugin()
    {

    }

    void PluginUserWrapper::unloadPlugin()
    {

    }

    void PluginUserWrapper::login(const char *customParams)
    {

    }

    void PluginUserWrapper::logout(const char *customParams)
    {

    }

    void PluginUserWrapper::updateConfig()
    {

    }

    void PluginUserWrapper::setDebugMode(bool mode)
    {

    }

    void PluginUserWrapper::onLogin(HttpClient *sender, HttpResponse *response)
    {

    }
}
