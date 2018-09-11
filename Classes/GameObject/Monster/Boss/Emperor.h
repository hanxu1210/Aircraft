#ifndef  _APP_Emperor_H_
#define  _APP_Emperor_H_

#include "GameObject/Monster/Monster.h"

USING_NS_CC;
class BossLaser;
class EmperorGhost : public Monster
{
    typedef Monster super;

public:
    EmperorGhost(EMonsterType type, unsigned variationId, unsigned groupId);
    ~EmperorGhost();

    void CreateEmperor();
    virtual void StateUpdate(float deltaTime);
    bool CheckCollision(GameObject* pGameObject){return false;}

    EMonsterType m_type;
    unsigned m_variantionId;
};

class Emperor : public Monster 
{
    typedef Monster super;

public:
    Emperor(EMonsterType type, unsigned variationId, unsigned groupId);
    virtual ~Emperor();

    virtual void onEnter();
    virtual void onExit();

private:
    void PlayAnimation(unsigned state, const std::function<void()>& func);
    void PlayAnimationCallback();

    OVERLOAD_FSM(Emperor);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Move);
    DECLARE_VIRTUAL_STATE(MoveOut);
    DECLARE_VIRTUAL_STATE(ChangePos);
    DECLARE_VIRTUAL_STATE(Killed);
    DECLARE_VIRTUAL_STATE(PreShoot);
    DECLARE_VIRTUAL_STATE(Shoot);

    void GetMoveTarget();
private:
    // MoveOut
    reflected float m_moveOutLength;
    float m_startMoveX;

    // ChangePos
    float m_changePosDistance;
    float m_startChangePosY;

    // Timer
    reflected float m_moveTime;
    float m_moveTimer;
    reflected float m_preShootTime;
    float m_preShootTimer;
    reflected float m_shootTime;
    float m_shootTimer;
    // Shoot Type Probability
    reflected float m_skillProbability;
    reflected float m_skillTypeProbability;
    unsigned m_skillType;

    std::vector<BossLaser*> m_pBossLaserVec;
    FiniteTimeAction* m_animationAction;
};

#endif // _APP_Emperor_H_