#include "PluginIAP.h"
#include "PluginManager.h"
#include "Config/AppConfig.h"
#include "PluginParam.h"
#include "Utilities/Utilities.h"
#include <string>

using namespace cocos2d::plugin;

namespace NsAircraft
{

    PluginIAP::PluginIAP()
    {
    }

    PluginIAP::~PluginIAP()
    {
    }

    bool PluginIAP::init()
    {
        return Plugin::init();
    }

}