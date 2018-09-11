#ifndef  _APP_StayPlane_H_
#define  _APP_StayPlane_H_

#include "GameObject/Monster/Monster.h"

USING_NS_CC;

class StayPlanGhost : public Monster
{
    typedef Monster super;

public:
    StayPlanGhost(EMonsterType type, unsigned variationId, unsigned groupId)
        : super(type, variationId, groupId) 
        , m_type(type), m_variantionId(variationId)
    {}

    OVERLOAD_FSM(StayPlanGhost);
    DECLARE_VIRTUAL_STATE(Idle);
    bool CheckCollision(GameObject* pGameObject){return false;}

    EMonsterType m_type;
    unsigned m_variantionId;
};

class StayPlane : public Monster 
{
    typedef Monster super;

public:
    StayPlane(EMonsterType type, unsigned variationId, unsigned groupId);
    virtual ~StayPlane();

    virtual void onEnter();
    virtual void onExit();

private:
    void PlayMonsterWalkAnimation();

    OVERLOAD_FSM(StayPlane);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Move);
    DECLARE_VIRTUAL_STATE(MoveOut);
    DECLARE_VIRTUAL_STATE(ChangePos);
    DECLARE_VIRTUAL_STATE(Rest);
    DECLARE_VIRTUAL_STATE(Killed);

private:
    // MoveOut
    reflected float m_moveOutLength;
    float m_startMoveX;

    // ChangePos
    float m_changePosDistance;
    float m_startChangePosY;
};

#endif // _APP_StayPlane_H_