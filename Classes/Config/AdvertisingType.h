#ifndef  _RUSHGIRL_ADVERTISING_TYPE_H_
#define  _RUSHGIRL_ADVERTISING_TYPE_H_

namespace NsAircraft
{
    enum ADVERTISING_TYPE
    {
        AT_BANNER = 1,
        AT_FULLSCREEN,
        AT_POPUP,
        AT_OFFERWALL,
        AT_COUNT
    };

    enum ADVERTISING_SIZE
    {
        AS_320x50 = 1,
        AS_468x60,
        AS_728x90,
        AS_SMART_BANNER
    };
}
#endif