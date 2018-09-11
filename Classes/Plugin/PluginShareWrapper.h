#ifndef _RUSHGIRL_PLUGINSHAREWRAPPER_H_
#define _RUSHGIRL_PLUGINSHAREWRAPPER_H_

#include "cocos2d.h"
#include "plugin/Channel/PluginChannelShare.h"
#include "PluginProtocolWrapper.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginShareWrapper : public PluginProtocolWrapper
    {
    public:
        static const char* EVENT_SHAKE_ACTION_COMPLETE;
        static const char* EVENT_SHAKE_CLOSED;
        class PluginShareListener : public cocos2d::plugin::ShareResultListener
        {
        public:
             /**
            @brief The share request result
            */
            virtual void onShareResult(cocos2d::plugin::ShareResultCode ret, const char* msg) override;
        };

        class PluginShakeLister : public cocos2d::plugin::ShakeToShareListener
        {
        public:
            virtual void onShakeActionComplete() override;
            virtual void onShakeClosed() override;
        };

        /** PluginAdsrWrapper constructor */
        PluginShareWrapper();

        /** PluginAdsrWrapper destructor */
        virtual ~PluginShareWrapper();

        virtual bool init(void) override;
        virtual bool uninit(void) override;
        virtual void loadPlugin() override;
        virtual void unloadPlugin() override;
        virtual void updateConfig() override;
        virtual void setDebugMode(bool mode) override;

        void share(const std::string& text, const std::string& image);

    private:
        void onShakeActionComplete();
        void onShakeClosed();

    private:
         PluginChannelShare* m_pluginShare;
         PluginShareListener* m_shareListener;
         PluginShakeLister* m_shakeListener;
         cocos2d::EventCustom* m_eventShakeActionComplete;
         cocos2d::EventCustom* m_eventShakeClosed;
    };
}

#endif