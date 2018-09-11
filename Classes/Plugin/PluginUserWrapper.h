#ifndef _RUSHGIRL_PLUGINUSERWRAPPER_H_
#define _RUSHGIRL_PLUGINUSERWRAPPER_H_

#include "cocos2d.h"
#include "PluginProtocolWrapper.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}
namespace cocos2d{namespace network{class HttpClient; class HttpResponse;}}

namespace NsAircraft
{
    class PluginUserWrapper : public PluginProtocolWrapper
    {
    public:
        static const char* EVENT_LOGIN_SUCCESS;
        static const char* EVENT_LOGIN_FAILED;
        static const char* EVENT_LOGOUT;
        static const char* EVENT_GAME_EXIT;

        /** PluginUserWrapper constructor */
        PluginUserWrapper();

        /** PluginUserWrapper destructor */
        virtual ~PluginUserWrapper();

        virtual bool init(void) override;
        virtual bool uninit(void) override;
        virtual void loadPlugin() override;
        virtual void unloadPlugin() override;
        virtual void updateConfig() override;
        virtual void setDebugMode(bool mode) override;

        void login(const char *customParams);
        void logout(const char *customParams);

        void onLogin(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);


    private:
        cocos2d::EventCustom* m_eventLoginSuccess;
        cocos2d::EventCustom* m_eventLoginFailed;
        cocos2d::EventCustom* m_eventLogout;
        cocos2d::EventCustom* m_eventGameExit;
    };
}

#endif