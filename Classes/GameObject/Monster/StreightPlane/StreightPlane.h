#ifndef  _APP_StreightPlane_H_
#define  _APP_StreightPlane_H_

#include "GameObject/Monster/Monster.h"

USING_NS_CC;

class StreightPlane : public Monster 
{
    typedef Monster super;

public:
    StreightPlane(EMonsterType type, unsigned variationId, unsigned groupId);
    virtual ~StreightPlane();

    virtual void onEnter();
    virtual void onExit();

private:
    void PlayMonsterWalkAnimation();

    OVERLOAD_FSM(StreightPlane);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Move);
    DECLARE_VIRTUAL_STATE(MoveOut);
};

#endif // _APP_StreightPlane_H_