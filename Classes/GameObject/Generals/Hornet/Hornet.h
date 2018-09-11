#ifndef  _APP_Hornet_H_
#define  _APP_Hornet_H_

#include "GameObject/Generals/General.h"

USING_NS_CC;

class Hornet : public General 
{
public:
    Hornet();
    virtual ~Hornet();

    virtual void onEnter();
    virtual void onExit();
private:
    OVERLOAD_FSM(Hornet);
    DECLARE_VIRTUAL_STATE(CastSkill);
};

#endif // _APP_Hornet_H_