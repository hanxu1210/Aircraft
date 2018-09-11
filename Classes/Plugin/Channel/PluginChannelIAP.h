#ifndef _RUSHGIRL_PLUGIN_CHANNEL_IAP_H_
#define _RUSHGIRL_PLUGIN_CHANNEL_IAP_H_

#include "cocos2d.h"
#include "PluginIAP.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginChannelIAP : public PluginIAP
    {
    public:      
        /** PluginChannelIAP constructor */
        PluginChannelIAP();

        /** PluginChannelIAP destructor */
        virtual ~PluginChannelIAP();

        virtual bool init(void) override;
        virtual bool canMakePayments() override;
        virtual void payForProduct(const std::string& productId) override;
        virtual void requestProducts(const std::string& paralist) override;
        virtual void restoreProducts() override;
        virtual bool isMusicEnabled() override;
        virtual void gameExit() override;

        virtual void setResultListener(cocos2d::plugin::PayResultListener* listener) override;
        virtual void setSFResultListener(SFNativeIPayResulBack* listener) override;
        virtual void setGameExitListener(SFNativeGameExitCallBack* listener) override;
    };
}

#endif
