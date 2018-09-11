#ifndef _RUSHGIRL_PLUGINMANAGERWRAPPER_H_
#define _RUSHGIRL_PLUGINMANAGERWRAPPER_H_

#include "cocos2d.h"

namespace NsAircraft
{
    class PluginProtocolWrapper;
    class PluginManagerWrapper : public cocos2d::Ref
    {
    public:
        enum E_Plugin_Protocol
        {
            PP_ANALYTICS, // plugin analytics must be fist one
            PP_SHARE,
            PP_ADS,
            PP_SOCIAL,
            //PP_USER,
            PP_IAP,
            PP_COUNT
        };
        /** Gets the single instance of PluginManagerWrapper. */
        static PluginManagerWrapper *sharedInstance(void);

        /** Destroys the single instance of ResourceManager. */
        static void purgeInstance(void);
        
        virtual bool init(void);
        virtual bool uninit(void);
        void loadAllPlugins();
        void unloadAllPlugins();
        void setDebugMode(bool mode);
        void updateConfig();
        PluginProtocolWrapper* getProtocol(E_Plugin_Protocol protocol) const;

    private:
        /** PluginManagerWrapper constructor */
        PluginManagerWrapper();

        /** PluginManagerWrapper destructor */
        virtual ~PluginManagerWrapper();

        PluginProtocolWrapper* m_pluginProtocol[PP_COUNT];
    };
}

#endif