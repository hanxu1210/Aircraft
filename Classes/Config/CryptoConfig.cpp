#include "CryptoConfig.h"

namespace NsAircraft
{
    unsigned char CryptoConfig::aesKey[16] = {  0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,
                                                0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88 };
    unsigned char CryptoConfig::initVector[16] = {  0x1f,0x32,0x43,0x51,0x56,0x98,0xaf,0xed,
                                                    0xab,0xc8,0x21,0x45,0x63,0x72,0xac,0xfc }; 
}