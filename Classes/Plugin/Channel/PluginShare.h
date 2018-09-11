#ifndef _RUSHGIRL_PLUGIN_SHARE_H_
#define _RUSHGIRL_PLUGIN_SHARE_H_

#include "cocos2d.h"
#include "Plugin.h"
#include "ProtocolShare.h"

namespace NsAircraft
{
    class PluginShare : public Plugin
    {
    public:
        /** PluginShare constructor */
        PluginShare();

        /** PluginShare destructor */
        virtual ~PluginShare();

        virtual bool init() override;
        void share(const std::string& text, const std::string& image);
        void setResultListener( cocos2d::plugin::ShareResultListener* listener );
        void setShakeToShareListener(  cocos2d::plugin::ShakeToShareListener* listener );
    };
}

#endif