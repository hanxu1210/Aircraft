#ifndef _RUSHGIRL_PLUGINPROTOCOLWRAPPER_H_
#define _RUSHGIRL_PLUGINPROTOCOLWRAPPER_H_

#include "cocos2d.h"

namespace NsAircraft
{
    class PluginProtocolWrapper : public cocos2d::Ref
    {
    public:
        PluginProtocolWrapper();
        virtual ~PluginProtocolWrapper();
        virtual bool init() = 0;
        virtual bool uninit() = 0;
        virtual void loadPlugin() = 0;
        virtual void unloadPlugin() = 0;
        virtual void updateConfig() = 0;
        virtual void setDebugMode(bool mode) = 0;

    protected:
        cocos2d::EventDispatcher* _eventDispatcher;
    };
}

#endif