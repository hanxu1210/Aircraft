#include "CommonHeaders.h"

#include "GameLogic/GpeLogic.h"
#include "GameObject/Gpe/EnergyStation/EnergyStation.h"

GpeLogic::GpeLogic()
{
}

GpeLogic::~GpeLogic()
{   
}

void GpeLogic::onEnter()
{
    Node::onEnter();

    INIT_FSM(Idle);
}

void GpeLogic::onExit()
{
    Node::onExit();
}

void GpeLogic::StateUpdate( float deltaTime )
{
    GetFsm().Update();
}

void GpeLogic::CreateEnergyStation( float positionX )
{
    EnergyStation* pEnergyStation= new EnergyStation();
    pEnergyStation->autorelease();
    pEnergyStation->setPosition(Vec2(positionX, VisibleRect::center().y));
    addChild(pEnergyStation);
    m_gpeVec.push_back(pEnergyStation);
}

void GpeLogic::Reset()
{
    for (TGpeVec::iterator iter = m_gpeVec.begin(); iter != m_gpeVec.end(); ++iter)
    {
        (*iter)->Reset();
    }
}

IMPLEMENT_STATE_BEGIN(GpeLogic, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        CreateEnergyStation(512.0f);
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

