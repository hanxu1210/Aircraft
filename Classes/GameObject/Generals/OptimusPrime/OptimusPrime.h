#ifndef  _APP_OptimusPrime_H_
#define  _APP_OptimusPrime_H_

#include "GameObject/Generals/General.h"

USING_NS_CC;

class OptimusPrime : public General 
{
public:
    OptimusPrime();
    virtual ~OptimusPrime();

    virtual void onEnter();
    virtual void onExit();
private:
    OVERLOAD_FSM(OptimusPrime);
    DECLARE_VIRTUAL_STATE(CastSkill);
};

#endif // _APP_OptimusPrime_H_