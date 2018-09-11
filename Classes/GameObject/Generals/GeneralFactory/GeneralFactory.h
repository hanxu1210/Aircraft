#ifndef  _APP_GENERALFACTORY_H_
#define  _APP_GENERALFACTORY_H_

#include "GameObject/GameObjectEnum.h"

USING_NS_CC;

class GeneralFactory
{
public:
    static GeneralFactory& Get()
    {
        static GeneralFactory factory;
        return factory;
    }

    void CreateGeneral(Node* parent, EGeneralType type);

private:
    GeneralFactory();
    ~GeneralFactory();
};

#endif //_APP_GENERALFACTORY_H_