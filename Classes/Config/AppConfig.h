#ifndef  _RUSHGIRL_APP_CONFIG_H_
#define  _RUSHGIRL_APP_CONFIG_H_

namespace NsAircraft
{
    class AppConfig
    {
    public:
        // Umeng app config
        static const char* UmengAppkey;   
        // o2omobi app config
        static const char* o2omobiAppkey;
        static const char* o2omobiAppSecret;
        // AdChina app config
        static const char* AdChinaBannerID;
        static const char* AdChinaFullScreenID;
        static const char* AdChinaPopupID;
        static const char* AdChinaPadBannerID;
        static const char* AdChinaPadFullScreenID;
        static const char* AdChinaPadPopupID;
        // Admob app config
        static const char* AdmobBannerID;
        static const char* AdmobInterstitialID;
        static const char* AdmobTestDevices[];
        static const int AdmobTestDeviceCount;
        // KTPlay app config
        static const char* KTPlayAppkey;
        static const char* KTPlayAppSecret;
        // Luyou app config
        static const char* LuyouAppkey;
    };
}

#endif