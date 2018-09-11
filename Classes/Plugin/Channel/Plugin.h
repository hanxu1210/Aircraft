#ifndef _RUSHGIRL_PLUGIN_H_
#define _RUSHGIRL_PLUGIN_H_

#include "cocos2d.h"
#include "PluginProtocol.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class Plugin : public cocos2d::Ref
    {
    public:
        enum E_Plugin_Platform
        {
            PP_O2OMOBI,
            PP_ADCHINA,
            PP_ADMOB,
            PP_SHAREUMENG,
            PP_ANALYTICSUMENG,
            PP_IOSIAP,
            PP_YIJIE,
            PP_APPRATING,
            PP_KTPLAY,
            PP_LUYOU,
            PP_COUNT,
            PP_INVALID
        };

        /** Plugin constructor */
        Plugin();

        /** Plugin destructor */
        virtual ~Plugin();

        virtual bool init(void);
        virtual bool uninit(void);
        virtual void loadPlugin();
        virtual void unloadPlugin();
        virtual void setDebugMode(bool mode);

    protected:
        std::map<E_Plugin_Platform, cocos2d::plugin::PluginProtocol*> m_pluginProtocol;
        std::map<E_Plugin_Platform, std::string> m_pluginPlatforms;
    };
}

#endif