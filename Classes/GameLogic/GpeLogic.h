#ifndef  _APP_GpeLogic_H_
#define  _APP_GpeLogic_H_

#include "utilities/FSM/FsmInclude.h"
#include "GameObject/GameObjectEnum.h"

USING_NS_CC;

class Gpe;

class GpeLogic : public Node
{
public:
    GpeLogic();
    virtual ~GpeLogic();

    virtual void onEnter();
    virtual void onExit();
    virtual void Reset();;
    void StateUpdate(float deltaTime);

private:
    DECLARE_FSM(GpeLogic);
    DECLARE_STATE(Idle);
    DECLARE_STATE(Activate);

    void CreateEnergyStation(float positionX);

private:
    typedef std::vector< Gpe* > TGpeVec;
    TGpeVec m_gpeVec;

};

#endif //_APP_GpeLogic_H_