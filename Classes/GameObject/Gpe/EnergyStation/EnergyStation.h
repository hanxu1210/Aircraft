#ifndef  _APP_EnergyStation_H_
#define  _APP_EnergyStation_H_

#include "GameObject/Gpe/Gpe.h"

USING_NS_CC;

class EnergyStation : public Gpe
{
    typedef Gpe super;
public:
    EnergyStation();
    virtual ~EnergyStation();

    virtual void onEnter();
    virtual void onExit();
    void StateUpdate( float deltaTime );
    virtual void Reset();

private:
    DECLARE_FSM(EnergyStation);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(Absorb);
    DECLARE_VIRTUAL_STATE(AddEnergy);
    DECLARE_VIRTUAL_STATE(Deactive);
    
private:
    GameObject* m_targetGameObject;
    
    // Absorb
    reflected float m_absorbSpeedFactor;

    // AddEnergy
    float m_addEnergyTimer;
    reflected float m_addEnergyInterval;

    float m_addEnergyTotalTimer;
    std::vector<float> m_posYVec;
    float m_absorbY;
};

#endif // _APP_EnergyStation_H_