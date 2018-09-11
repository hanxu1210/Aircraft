#include "PluginIAPWrapper.h"
#include "PluginManagerWrapper.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "Config/IAPConfig.h"
#include "PluginParam.h"
#include "CommonHeaders.h"

using namespace cocos2d::plugin;

namespace NsAircraft
{

    const char* PluginIAPWrapper::EVENT_PAY_RESULT = "event_pay_result";
    const char* PluginIAPWrapper::EVENT_REQUEST_RESULT = "event_request_result";
    const char* PluginIAPWrapper::EVENT_GAME_EXIT = "event_game_exit";

    void PluginIAPWrapper::PluginIAPListener::onPayResult(PayResultCode ret, const char* msg, const char* error)
    {
        PluginIAPWrapper* pluginIAP = dynamic_cast<PluginIAPWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_IAP));
        if (pluginIAP)
        {
            pluginIAP->onPayResult(ret, msg, error);
        }
    }

    void PluginIAPWrapper::PluginIAPListener::onRequestProductsResult(IAPProductRequest ret, TProductList info, const char* error)
    {
        PluginIAPWrapper* pluginIAP = dynamic_cast<PluginIAPWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_IAP));
        if (pluginIAP)
        {
            pluginIAP->onRequestProductsResult(ret, info, error);
        }
    }

    void PluginIAPWrapper::PluginIAPListener::onCanceled(const char* remain)
    {
        PluginIAPWrapper* pluginIAP = dynamic_cast<PluginIAPWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_IAP));
        if (pluginIAP)
        {
            pluginIAP->onPayResult(kPayCanceled, remain, "");
        }
    }

    void PluginIAPWrapper::PluginIAPListener::onFailed(const char* remain)
    {
        PluginIAPWrapper* pluginIAP = dynamic_cast<PluginIAPWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_IAP));
        if (pluginIAP)
        {
            pluginIAP->onPayResult(kPayFail, remain, "");
        }
    }

    void PluginIAPWrapper::PluginIAPListener::onSuccess(const char* orderNo)
    {
        PluginIAPWrapper* pluginIAP = dynamic_cast<PluginIAPWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_IAP));
        if (pluginIAP)
        {
            pluginIAP->onPayResult(kPaySuccess, orderNo, "");
        }
    }

    void PluginIAPWrapper::PluginExitListerner::onGameExit(bool result)
    {
        PluginIAPWrapper* pluginIAP = dynamic_cast<PluginIAPWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_IAP));
        if (pluginIAP)
        {
            pluginIAP->onGameExit(result);
        }
    }


    PluginIAPWrapper::PluginIAPWrapper():
        m_pluginIAP(nullptr),
        m_IAPListener(nullptr),
        m_gameExitListener(nullptr),
        m_eventPayResult(nullptr),
        m_eventRequestResult(nullptr),
        m_eventGameExit(nullptr)
    {
        m_pluginIAP = new PluginChannelIAP();
    }

    PluginIAPWrapper::~PluginIAPWrapper()
    {
        CC_SAFE_DELETE(m_IAPListener);
        CC_SAFE_DELETE(m_gameExitListener);
        CC_SAFE_RELEASE_NULL(m_pluginIAP);
        CC_SAFE_RELEASE_NULL(m_eventPayResult);
        CC_SAFE_RELEASE_NULL(m_eventRequestResult);
        CC_SAFE_RELEASE_NULL(m_eventGameExit);
    }

    bool PluginIAPWrapper::init()
    {
        updateConfig();

        if (m_pluginIAP)
        {
            m_pluginIAP->init();
            m_IAPListener = new PluginIAPListener;
            m_gameExitListener = new PluginExitListerner;
            m_pluginIAP->setResultListener(m_IAPListener);
            m_pluginIAP->setSFResultListener(m_IAPListener);
            m_pluginIAP->setGameExitListener(m_gameExitListener);

            m_eventPayResult = new cocos2d::EventCustom(EVENT_PAY_RESULT);
            m_eventRequestResult = new cocos2d::EventCustom(EVENT_REQUEST_RESULT);
            m_eventGameExit = new cocos2d::EventCustom(EVENT_GAME_EXIT);
        }
        return true;
    }

    bool PluginIAPWrapper::uninit()
    {
        return true;
    }

    void PluginIAPWrapper::loadPlugin()
    {
        m_pluginIAP->loadPlugin();
    }

    void PluginIAPWrapper::unloadPlugin()
    {
        m_pluginIAP->unloadPlugin();
    }

    void PluginIAPWrapper::updateConfig()
    {

    }

    void PluginIAPWrapper::setDebugMode(bool mode)
    {
        if (m_pluginIAP)
        {
            m_pluginIAP->setDebugMode(mode);
        }
    }

    bool PluginIAPWrapper::isMusicEnabled()
    {
        return m_pluginIAP->isMusicEnabled();
    }

    bool PluginIAPWrapper::canMakePayments()
    {
        if (m_pluginIAP)
        {
            return m_pluginIAP->canMakePayments();
        }
        return false;
    }

    void PluginIAPWrapper::payForProduct(const std::string& productId)
    {
        if (m_pluginIAP)
        {
            m_pluginIAP->payForProduct(productId);
        }
    }

    void PluginIAPWrapper::requestProducts(const std::string& paralist)
    {
        if (m_pluginIAP)
        {
            m_pluginIAP->requestProducts(paralist);
        }
    }

    void PluginIAPWrapper::restoreProducts()
    {
        if (m_pluginIAP)
        {
            m_pluginIAP->restoreProducts();
        }
    }

    void PluginIAPWrapper::gameExit()
    {
        return m_pluginIAP->gameExit();
    }

    void PluginIAPWrapper::onPayResult(PayResultCode ret, const char* msg, const char* error)
    {
        CCLOG("onPayResult %s", msg);
        PayResult payResult;
        payResult.result = ret;
        payResult.msg = msg;
        payResult.error = error;
        m_eventPayResult->setUserData(&payResult);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventPayResult);
    }

    void PluginIAPWrapper::onRequestProductsResult(IAPProductRequest ret, TProductList info, const char* error)
    {
        CCLOG("onRequestProductsResult %d", ret);
        RequestResult requestResult;
        requestResult.result = ret;
        requestResult.info = info;
        requestResult.error = error;
        m_eventRequestResult->setUserData(&requestResult);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventRequestResult);
    }

    void PluginIAPWrapper::onGameExit(bool result)
    {
        CCLOG("onExit %d", result);
        m_eventGameExit->setUserData(&result);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(m_eventGameExit);
    }
}
