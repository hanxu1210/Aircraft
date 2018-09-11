#ifndef  _APP_Gpe_H_
#define  _APP_Gpe_H_

#include "GameObject/GameObject.h"

USING_NS_CC;

class Gpe : public GameObject 
{
public:
    Gpe();
    Gpe(unsigned type);
    virtual ~Gpe();
    virtual void Reset(){};

protected:
    unsigned m_type;
    Sprite* m_pMainSprite;
    float m_deltaTime;

};

#endif // _APP_Gpe_H_
