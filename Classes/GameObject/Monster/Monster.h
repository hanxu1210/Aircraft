#ifndef  _APP_Monster_H_
#define  _APP_Monster_H_

#include "GameObject/GameObject.h"

USING_NS_CC;

class Monster : public GameObject 
{
public:
    Monster(EMonsterType type, unsigned variationId, unsigned groupId);
    virtual ~Monster();
    void Setup(EMonsterType type, unsigned variationId, unsigned groupId);

    virtual void onEnter();
    virtual void onExit();

    EMonsterType GetType() {return m_type;};
    virtual void StateUpdate(float deltaTime);
    bool Attack();
    void Killed();
    bool NeedShakeCamera();

protected:
    DECLARE_FSM(Monster);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Killed);

    virtual void Shoot(Vec2 offset, Node* pNode = NULL);
    void Tint();
    virtual void ReduceHp(unsigned hp);

protected:
    EMonsterType m_type;
    unsigned m_variationId;
    unsigned m_groupId;
    Sprite* m_pMainSprite;

    float m_speed;
    float m_deltaTime;
    bool m_shake;
    unsigned m_score;
    ActionInterval* m_pTintAction;
    bool m_lastInGroup;
};

#endif // _APP_Monster_H_