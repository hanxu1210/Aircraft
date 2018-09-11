#ifndef  _RUSHGIRL_LOCALIZATION_H_
#define  _RUSHGIRL_LOCALIZATION_H_

#include "cocos2d.h"
#include <string>

namespace NsAircraft
{
    class Localization : public cocos2d::Ref
    {
    public:
        static std::string FileName;
        /** Localization constructor */
        Localization();

        /** Localization destructor */
        ~Localization();

        virtual bool init(void);

        /** Gets the single instance of ResourceManager. */
        static Localization *sharedInstance(void);

        /** Destroys the single instance of ResourceManager. */
        static void purgeInstance(void);

        /** returns the value of a given key as a double */
        const cocos2d::Value& getValue(const std::string& key, const cocos2d::Value& defaultValue = cocos2d::Value::Null) const;
    private:
        cocos2d::ValueMap m_valueDict;
    };
}

#endif