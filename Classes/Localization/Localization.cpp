#include "Localization.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Config/IOS/LanguageFetcher.h"
#endif
USING_NS_CC;

namespace NsAircraft
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
    std::string Localization::FileName = "Localization.plist";
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string Localization::FileName = "Localization";
#endif
    static Localization *s_sharedInstance = NULL;

    Localization::Localization()
    {

    }

    Localization::~Localization()
    {

    }

    bool Localization::init()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
       FileName = FileName + std::string(FetchCurrentLanguage()) + std::string(".plist");
#endif
       m_valueDict = FileUtils::getInstance()->getValueMapFromFile(FileName);

       return true;
    }

    Localization *Localization::sharedInstance(void)
    {
        if (!s_sharedInstance)
        {
            s_sharedInstance = new Localization;
        }
        return s_sharedInstance;
    }

    void Localization::purgeInstance(void)
    {
        CC_SAFE_RELEASE_NULL(s_sharedInstance);
    }

    //
    // generic getters for properties
    //
    const Value& Localization::getValue(const std::string& key, const Value& defaultValue) const
    {
        auto iter = m_valueDict.find(key);
        if (iter != m_valueDict.cend())
            return m_valueDict.at(key);
        return defaultValue;
    }
}