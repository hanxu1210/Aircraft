#include "PluginProtocolWrapper.h"

namespace NsAircraft
{
    PluginProtocolWrapper::PluginProtocolWrapper()
    {
         _eventDispatcher = new cocos2d::EventDispatcher();
    }

    PluginProtocolWrapper::~PluginProtocolWrapper()
    {
        CC_SAFE_RELEASE(_eventDispatcher);
    }
}