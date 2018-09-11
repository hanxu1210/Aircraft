#ifndef  _APP_DashPlane_H_
#define  _APP_DashPlane_H_

#include "GameObject/Monster/Monster.h"

USING_NS_CC;

class DashPlane : public Monster 
{
    typedef Monster super;

public:
    DashPlane(EMonsterType type, unsigned variationId, unsigned groupId);
    virtual ~DashPlane();

    virtual void onEnter();
    virtual void onExit();

private:
    void PlayMonsterWalkAnimation();

    OVERLOAD_FSM(DashPlane);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Move);
    DECLARE_VIRTUAL_STATE(MoveOut);
    DECLARE_VIRTUAL_STATE(ChangePos);
    DECLARE_VIRTUAL_STATE(Dash);

private:
    // MoveOut
    reflected float m_moveOutLength;
    float m_startMoveX;

    // ChangePos
    float m_changePosDistance;
    float m_startChangePosY;
};

#endif // _APP_DashPlane_H_