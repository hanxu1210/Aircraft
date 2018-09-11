#ifndef _RUSHGIRL_PLUGIN_CHANNEL_ANALYTICS_H_
#define _RUSHGIRL_PLUGIN_CHANNEL_ANALYTICS_H_

#include "cocos2d.h"
#include "PluginAnalytics.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginChannelAnalytics : public PluginAnalytics
    {
    public:      
        /** PluginChannelAnalytics constructor */
        PluginChannelAnalytics();

        /** PluginChannelAnalytics destructor */
        virtual ~PluginChannelAnalytics();

        virtual bool init(void) override;
    };
}

#endif