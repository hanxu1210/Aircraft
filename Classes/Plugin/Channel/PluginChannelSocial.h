#ifndef _RUSHGIRL_PLUGIN_CHANNEL_SOCIAL_H_
#define _RUSHGIRL_PLUGIN_CHANNEL_SOCIAL_H_

#include "cocos2d.h"
#include "PluginSocial.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginChannelSocial : public PluginSocial
    {
    public:      
        /** PluginChannelSocial constructor */
        PluginChannelSocial();

        /** PluginChannelSocial destructor */
        virtual ~PluginChannelSocial();

        virtual bool init(void) override;
        virtual void appEnteredForeground() override;
        virtual void logEvent() override;
    };
}

#endif