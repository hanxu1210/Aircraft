#include "CommonHeaders.h"

#include "GameLogic/AdsLogic/AdsLogic.h"
#include "Plugin/PluginManagerWrapper.h"
#include "Plugin/PluginAdsWrapper.h"
#include "ProtocolAnalytics.h"
#include "ProtocolAds.h"
#include "PluginParam.h"
#include "Config/AppConfig.h"
#include "Config/AdvertisingType.h"

using namespace cocos2d::plugin;

namespace NsAircraft
{
    IMPLEMENT_SINGLETON(AdsLogic);

    AdsLogic::AdsLogic()
         : m_pAdsWrapper(nullptr)
         , m_pListener(nullptr)
         , m_addsClicked(false)
         , m_hideBanner(false)
         , m_popUpCount(0)
    {
        m_pAdsWrapper = dynamic_cast<PluginAdsWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_ADS));

        if (m_pAdsWrapper)
            m_pAdsWrapper->showAds(AT_BANNER, ProtocolAds::kPosBottom);

        m_pListener = EventListenerCustom::create(PluginAdsWrapper::EVENT_ADS_CLICKED, CC_CALLBACK_1(AdsLogic::OnAdsClicked,this));
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_pListener, 1); 
 
//         m_pListener = EventListenerCustom::create(PluginAdsWrapper::EVENT_ADS_CLICKED, [=](EventCustom* event){
//             if (m_pAdsWrapper)
//                 m_pAdsWrapper->hideAds(AT_BANNER);
//             m_addsClicked = true;
//        });
    }
 
    AdsLogic::~AdsLogic()
    {
    }

    void AdsLogic::PopUpPopUpAdds()
    {
        ++m_popUpCount;

        //if (m_popUpCount % 2 == 0)
        {
            PluginAdsWrapper* ads = dynamic_cast<PluginAdsWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_ADS));
            if (ads)
            {
                ads->showAds(AT_POPUP, ProtocolAds::kPosCenter);
            }
        }
    }

    void AdsLogic::ShowAdsWall()
    {
        PluginAdsWrapper* ads = dynamic_cast<PluginAdsWrapper*>(PluginManagerWrapper::sharedInstance()->getProtocol(PluginManagerWrapper::PP_ADS));
        if (ads)
        {
            ads->showAds(AT_OFFERWALL, ProtocolAds::kPosCenter);
        }
    }

    bool AdsLogic::IsAddsClicked()
    {
        return m_addsClicked;
    }

    void AdsLogic::OnAdsClicked( EventCustom* pEvent )
    {
        if (pEvent && m_pAdsWrapper)
        {
            auto clickedInfo = static_cast<PluginAdsWrapper::ClickedInfo*>(pEvent->getUserData());
            if (clickedInfo)
            {
                if ((*clickedInfo).type == AT_BANNER)
                {
                     //m_pAdsWrapper->hideAds(AT_BANNER);
                     //m_hideBanner = true;
                }
                else if ((*clickedInfo).type == AT_POPUP)
                    m_addsClicked = true;
            }
        }
    }

    bool AdsLogic::IsHideBanner()
    {
        return m_hideBanner;
    }

    void AdsLogic::SetIsHideBanner( bool isHide )
    {
        m_hideBanner = isHide;
    }

    void AdsLogic::HideBannerAndDisableAds()
    {
        if (m_pAdsWrapper)
        {
            m_pAdsWrapper->hideAds(AT_BANNER);
            m_pAdsWrapper->disableAds();
            m_hideBanner = true;
        }
    }
}
