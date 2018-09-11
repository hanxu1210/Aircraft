#ifndef  _APP_BossLaser_H_
#define  _APP_BossLaser_H_

#include "Bullet.h"

USING_NS_CC;

class BossLaser : public Bullet 
{
    typedef Bullet super;

public:
    BossLaser(EBulletType type, EFactionType faction);
    virtual ~BossLaser();

    float GetSpeed() const;
    void SetAttackPoint( unsigned attackPoint );

    virtual void onEnter();
    virtual void onExit();

    virtual void StateUpdate(float deltaTime);

    bool Attack();

protected:
    DECLARE_FSM(BossLaser);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Move);

};

#endif // _APP_BossLaser_H_