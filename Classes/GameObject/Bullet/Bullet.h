#ifndef  _APP_Bullet_H_
#define  _APP_Bullet_H_

#include "GameObject/GameObject.h"

USING_NS_CC;

class Bullet : public GameObject 
{
    typedef GameObject super;

public:
    Bullet(EBulletType type, EFactionType faction);
    virtual ~Bullet();
    void Setup(EBulletType type, EFactionType faction);

    float GetSpeed() const;
    void SetAttackPoint( unsigned attackPoint );
    EBulletType GetType() {return m_type;};

    virtual void onEnter();
    virtual void onExit();

    virtual void StateUpdate(float deltaTime);

    bool Attack();

protected:
    DECLARE_FSM(Bullet);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Move);

    bool CheckUnspawn();

protected:
    EBulletType m_type;
    Sprite* m_pMainSprite;
    ParticleGalaxy* m_pParticle;

    Vec2 m_targetPos;
    Vec2 m_direction;

    float m_speed;
    float m_deltaTime;
    EFactionType m_factionType;

    unsigned m_attackPoint;
};

#endif // _APP_Bullet_H_