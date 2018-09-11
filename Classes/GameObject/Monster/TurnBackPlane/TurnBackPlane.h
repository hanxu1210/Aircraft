#ifndef  _APP_TurnBackPlane_H_
#define  _APP_TurnBackPlane_H_

#include "GameObject/Monster/Monster.h"

USING_NS_CC;

class TurnBackPlane : public Monster 
{
    typedef Monster super;

public:
    TurnBackPlane(EMonsterType type, unsigned variationId, unsigned groupId);
    virtual ~TurnBackPlane();

    virtual void onEnter();
    virtual void onExit();

private:
    void PlayMonsterWalkAnimation();

    OVERLOAD_FSM(TurnBackPlane);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Move);
    DECLARE_VIRTUAL_STATE(MoveOut);
    DECLARE_VIRTUAL_STATE(RotateBack);

private:
    // MoveOut
    reflected float m_moveOutLength;
    float m_startMoveX;

    // ChangePos
    float m_changePosDistance;
    float m_startChangePosY;
};

#endif // _APP_TurnBackPlane_H_