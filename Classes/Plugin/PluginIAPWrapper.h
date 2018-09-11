#ifndef _RUSHGIRL_PLUGINIAPWRAPPER_H_
#define _RUSHGIRL_PLUGINIAPWRAPPER_H_

#include "cocos2d.h"
#include "Channel/PluginChannelIAP.h"
#include "PluginProtocolWrapper.h"
#include "YiJie/SFGameNativeInterface.h"

namespace cocos2d{namespace plugin{class PluginProtocol;}}

namespace NsAircraft
{
    class PluginIAPWrapper : public PluginProtocolWrapper
    {
    public:
        struct PayResult
        {
            cocos2d::plugin::PayResultCode result;
            const char *msg;
            const char *error;
        };

        struct RequestResult
        {
            cocos2d::plugin::IAPProductRequest result;
            cocos2d::plugin::TProductList info;
            const char *error;
        };

        static const char* EVENT_PAY_RESULT;
        static const char* EVENT_REQUEST_RESULT;
        static const char* EVENT_GAME_EXIT;

        class PluginIAPListener : public cocos2d::plugin::PayResultListener, public SFNativeIPayResulBack
        {
        public:
             /**
            @brief The pay result
            */
            virtual void onPayResult(cocos2d::plugin::PayResultCode ret, const char* msg, const char* error) override;

            virtual void onRequestProductsResult(cocos2d::plugin::IAPProductRequest ret, cocos2d::plugin::TProductList info, const char* error) override;

            virtual void onCanceled(const char* remain) override;
            virtual void onFailed(const char* remain) override;
            virtual void onSuccess(const char* orderNo) override;
        };

        class PluginExitListerner : public SFNativeGameExitCallBack
        {
        public:
            virtual void onGameExit(bool result) override;
        };

        /** PluginIAPWrapper constructor */
        PluginIAPWrapper();

        /** PluginIAPWrapper destructor */
        virtual ~PluginIAPWrapper();

        virtual bool init(void) override;
        virtual bool uninit(void) override;
        virtual void loadPlugin() override;
        virtual void unloadPlugin() override;
        virtual void updateConfig() override;
        virtual void setDebugMode(bool mode) override;

        bool isMusicEnabled();
        bool canMakePayments();
        void payForProduct(const std::string& productId);
        void requestProducts(const std::string& paralist);
        void restoreProducts();
        void gameExit();

    private:
        void onPayResult(cocos2d::plugin::PayResultCode ret, const char* msg, const char* error);
        void onRequestProductsResult(cocos2d::plugin::IAPProductRequest ret, cocos2d::plugin::TProductList info, const char* error);
        void onGameExit(bool result);

    private:
        PluginChannelIAP* m_pluginIAP;
        PluginIAPListener* m_IAPListener;
        PluginExitListerner* m_gameExitListener;
        cocos2d::EventCustom* m_eventPayResult;
        cocos2d::EventCustom* m_eventRequestResult;
        cocos2d::EventCustom* m_eventGameExit;
    };
}

#endif
