#ifndef  _APP_Laser_H_
#define  _APP_Laser_H_

#include "Bullet.h"

USING_NS_CC;

class Laser : public Bullet 
{
    typedef Bullet super;

public:
    Laser(EBulletType type, EFactionType faction);
    virtual ~Laser();

    float GetSpeed() const;
    void SetAttackPoint( unsigned attackPoint );

    virtual void onEnter();
    virtual void onExit();

    virtual void StateUpdate(float deltaTime);

    bool Attack();

protected:
    DECLARE_FSM(Laser);
    DECLARE_VIRTUAL_STATE(Move);

};

#endif // _APP_Laser_H_