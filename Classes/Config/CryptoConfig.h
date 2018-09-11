#ifndef  _RUSHGIRL_CRYPTO_CONFIG_H_
#define  _RUSHGIRL_CRYPTO_CONFIG_H_

namespace NsAircraft
{
    class CryptoConfig
    {
    public:
        // crypto config
        static unsigned char aesKey[16];
        static unsigned char initVector[16];
    };
}

#endif