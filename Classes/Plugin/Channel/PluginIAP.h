#ifndef _RUSHGIRL_PLUGIN_IAP_H_
#define _RUSHGIRL_PLUGIN_IAP_H_

#include "cocos2d.h"
#include "Plugin.h"
#include "ProtocolIAP.h"
#include "../YiJie/SFGameNativeInterface.h"

namespace NsAircraft
{
    class PluginIAP : public Plugin
    {
    public:
        /** PluginIAP constructor */
        PluginIAP();

        /** PluginIAP destructor */
        virtual ~PluginIAP();

        virtual bool init() override;
        virtual bool canMakePayments() = 0;
        virtual void payForProduct(const std::string& productId) = 0;
        virtual void requestProducts(const std::string& paralist) = 0;
        virtual void restoreProducts() = 0;
        virtual bool isMusicEnabled() = 0;
        virtual void gameExit() = 0;

        virtual void setResultListener(cocos2d::plugin::PayResultListener* listener){};
        virtual void setSFResultListener(SFNativeIPayResulBack* listener){};
        virtual void setGameExitListener(SFNativeGameExitCallBack* listener){};
    };
}

#endif