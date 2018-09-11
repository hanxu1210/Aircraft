#include "CommonHeaders.h"

#include "GameObject/Gpe/EnergyStation/EnergyStation.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameLogic/BackgroundLogic.h"
#include "GameObject/MainCharacter.h"

EnergyStation::EnergyStation()
    : super(eGT_EnergyStation)
    , m_targetGameObject(NULL)
    , m_absorbSpeedFactor(2.0f)
    , m_addEnergyTimer(0.0f)
    , m_addEnergyInterval(0.05f)
    , m_absorbY(0.0f)
{
    m_posYVec.push_back(50.0f);
    m_posYVec.push_back(200.0f);
    m_posYVec.push_back(350.0f);
    m_posYVec.push_back(500.0f);
    m_posYVec.push_back(650.0f);
}

EnergyStation::~EnergyStation()
{

}

void EnergyStation::onEnter()
{
    super::onEnter();

    for (int i = 0; i < m_posYVec.size(); ++i)
    {
        Sprite* pSprite = Sprite::createWithSpriteFrameName("EnergyStation.png");
        pSprite->setScale(0.5f * Director::getInstance()->getContentScaleFactor());
        pSprite->setPositionY(m_posYVec[i]);
        addChild(pSprite);
    }
    
    float height = 640;
    float weight = 100;
    m_collisionRect = Rect(0, 0, weight, height);

    INIT_FSM(Idle);
}

void EnergyStation::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;
    GetFsm().Update();
}

void EnergyStation::onExit()
{
    super::onExit();
}

void EnergyStation::Reset()
{
    GetFsm().SetCurrentState(STATE(Idle));
}

IMPLEMENT_STATE_BEGIN(EnergyStation, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

    STATE_UPDATE_BEGIN
    {
        m_targetGameObject = CheckCollisionByGameObjectType(eGOT_MainCharacter);
        if (m_targetGameObject)
        {
            TRANSIT_TO_STATE( true, NoTransitionAction, Absorb);
        }

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    IMPLEMENT_STATE_BEGIN(EnergyStation, Absorb)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        SoundManager::Singleton().PlayEffect("Sound/Effect_Refuel.mp3");
        
        BackgroundLogic::Singleton().SetMoveStatus(eBMS_Stop);
        MainPlayerLogic::Singleton().GetMC()->TransactToIdle();
        
        BackgroundLogic::Singleton().ShowEventTime("Bonus Time");
        
        Vec2 targetPos = m_targetGameObject->GetPos();
        float minOffset = std::numeric_limits<float>::max();
        for (int i = 0; i < m_posYVec.size(); ++i)
        {
            float offset = std::abs(targetPos.y - m_posYVec[i]);
            if (offset < minOffset)
            {
                minOffset = offset;
                m_absorbY = m_posYVec[i];
            }
        }
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        Vec2 pos = GetPos();
        pos.y = m_absorbY;
        Vec2 offset = pos - m_targetGameObject->GetPos();
        float distance = offset.getLength();
        offset = offset.getNormalized();
        Vec2 newPos = m_targetGameObject->getPosition() + offset * distance * m_deltaTime * m_absorbSpeedFactor;
        
        if ((pos - newPos).getLength() > 10)
            m_targetGameObject->setPosition(newPos);
        else
            TRANSIT_TO_STATE( true, NoTransitionAction, AddEnergy );
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(EnergyStation, AddEnergy)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_addEnergyTotalTimer = 2;
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        m_addEnergyTimer -= m_deltaTime;
        m_addEnergyTotalTimer -= m_deltaTime;
        if (m_addEnergyTimer < 0)
        {
            MainPlayerLogic::Singleton().IncreaseScore(1);
        }
        else
            m_addEnergyTimer = m_addEnergyInterval;

        if (m_addEnergyTotalTimer < 0)
        {
            m_addEnergyTotalTimer = 2;
            TRANSIT_TO_STATE( true, NoTransitionAction, Deactive );
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
        //BackgroundLogic::Singleton().ShowBonusTimeText(false);
        BackgroundLogic::Singleton().SetMoveStatus(eBMS_Dash);
        MainPlayerLogic::Singleton().GetMC()->TransactToFly();
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END
    
IMPLEMENT_STATE_BEGIN(EnergyStation, Deactive)
{
    STATE_CONSTRUCTOR_BEGIN
    {
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