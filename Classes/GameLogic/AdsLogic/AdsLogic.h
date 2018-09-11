#ifndef _RUSHGIRL_AdsLogic_
#define _RUSHGIRL_AdsLogic_

#include "cocos2d.h"
#include "Utilities/Singleton.h"

USING_NS_CC;

namespace NsAircraft{class PluginAdsWrapper;}

namespace NsAircraft
{
    class AdsLogic : public Ref
    {
    public:
        DECLARE_SINGLETON(AdsLogic);

    public:
        void PopUpPopUpAdds();
        void ShowAdsWall();
        bool IsAddsClicked();
        void HideBannerAndDisableAds();
        void SetIsHideBanner(bool isHide);
        bool IsHideBanner();

    private:
        void OnAdsClicked(EventCustom* pEvent);

        PluginAdsWrapper* m_pAdsWrapper;
        EventListenerCustom* m_pListener;
        bool m_addsClicked;
        bool m_hideBanner;

        unsigned int m_popUpCount;
    };
}

#endif // _RUSHGIRL_AdsLogic_
