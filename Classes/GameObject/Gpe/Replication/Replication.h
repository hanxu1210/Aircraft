#ifndef  _APP_Replication_H_
#define  _APP_Replication_H_

#include "GameObject/Gpe/Gpe.h"

USING_NS_CC;
class MainCharacter;

class Replication : public Gpe
{
    typedef Gpe super;

public:
    Replication();
    virtual ~Replication();

    virtual void onEnter();
    virtual void onExit();

    virtual void StateUpdate(float deltaTime);

protected:
    DECLARE_FSM(Replication);
    DECLARE_VIRTUAL_STATE(Idle);
    void Shoot();

protected:
    MainCharacter* m_pMC;
    float m_shootTimer;
    unsigned m_bulletCount;
    
};

#endif // _APP_Replication_H_