#ifndef __PLUGIN_UTILSEX_H__
#define __PLUGIN_UTILSEX_H__

#include "PluginUtils.h"
#include <map>

namespace cocos2d { namespace plugin {

class PluginUtilsEx : public PluginUtils
{
public:
    static std::map<std::string, std::string> getMapFromHashMap(JNIEnv*  env, jobject hashMap);
};

}}

#endif