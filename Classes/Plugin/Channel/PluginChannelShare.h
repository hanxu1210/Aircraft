#ifndef _RUSHGIRL_PLUGIN_CHANNEL_SHARE_H_
#define _RUSHGIRL_PLUGIN_CHANNEL_SHARE_H_

#include "cocos2d.h"
#include "PluginShare.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginChannelShare : public PluginShare
    {
    public:      
        /** PluginChannelShare constructor */
        PluginChannelShare();

        /** PluginChannelShare destructor */
        virtual ~PluginChannelShare();

        virtual bool init(void) override;
    };
}

#endif