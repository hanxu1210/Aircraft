#include "CommonHeaders.h"

#include "GameObject/Monster/StayPlane/StayPlane.h"
#include "GameData.h"
#include "GameLogic/BackgroundLogic.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameObject/Items/Gold/Gold.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/MainCharacter.h"

StayPlane::StayPlane(EMonsterType type, unsigned variationId, unsigned groupId)
    : super(type, variationId, groupId)
    , m_moveOutLength(0.0f)
    , m_startMoveX(0.0f)
    , m_changePosDistance(0.0f)
    , m_startChangePosY(0.0f)
{
    std::string moveOutLengthStr = GameData::Singleton().GetValue(eGDS_Monster, m_variationId, "MoveOutLength");
    std::vector<std::string> lengthVec = SplitString(moveOutLengthStr, ";");
    if (lengthVec[0] == lengthVec[1])
    {
        m_moveOutLength = atof(lengthVec[0].c_str());
    }
    else
    {
        float min = atof(lengthVec[0].c_str());
        float max = atof(lengthVec[1].c_str());
        m_moveOutLength = RandomFloat(min, max);
    }
}

StayPlane::~StayPlane()
{
}

void StayPlane::onEnter()
{
    Monster::onEnter();
    
    m_canRecycle = false;

    GameObjectManager::Singleton().RegisterMonsterGroup(m_groupId);
}

void StayPlane::onExit()
{
    Monster::onExit();
}

void StayPlane::PlayMonsterWalkAnimation()
{
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_STATE_BEGIN(StayPlanGhost, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        if (VisibleRect::getVisibleRectWithSmallBuffer().containsPoint(GetPos()))
        {
            Monster* pMonster = new StayPlane(m_type, m_variationId, m_groupId);
            pMonster->autorelease();

            pMonster->setPosition(GetPos());
            //parttenNode->Background
            getParent()->getParent()->addChild(pMonster, 3);
            
            Unspawn();
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(StayPlane, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        if (VisibleRect::getVisibleRectWithSmallBuffer().containsPoint(GetPos()))
            TRANSIT_TO_STATE( true, NoTransitionAction, Move );
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(StayPlane, Move)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        PlayMonsterWalkAnimation();

    }
    STATE_CONSTRUCTOR_END

    STATE_UPDATE_BEGIN
    {      
        TRANSIT_TO_STATE( true, NoTransitionAction, MoveOut );
    }
    STATE_UPDATE_END

    STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(StayPlane, MoveOut)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_startMoveX = getPositionX();
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        setPositionX(getPositionX() - m_speed * m_deltaTime);
        
        if (fabs(getPositionX() - m_startMoveX) > m_moveOutLength)
        {
            TRANSIT_TO_STATE( true, NoTransitionAction, ChangePos );
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(StayPlane, ChangePos)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        INT sign = RandomFloat() > 0 ? 1 : -1;
        while(true)
        {
            m_changePosDistance = RandomFloat(200.0f, 250.f);
            m_changePosDistance *= sign;
            sign *= -1;
            float newPosY = getPositionY() + m_changePosDistance;
            if (newPosY > 0 && newPosY < VisibleRect::top().y)
                break;
        }
        
        Shoot(Vec2(0.5*(m_collisionRect.getMaxX()-m_collisionRect.getMinX()), 0), BackgroundLogic::Singleton().GetNextSectionNode());

        m_startChangePosY = getPositionY();
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        //float backgroundSpeed = BackgroundLogic::Singleton().GetCurrentSpeed();

        //setPositionX(getPositionX() + backgroundSpeed * m_deltaTime);

        bool reach = (fabs(getPositionY() - m_startChangePosY) > fabs(m_changePosDistance));
        if (reach)
        {
            TRANSIT_TO_STATE( true, NoTransitionAction, Rest );
        }
        else
        {
            setPositionY(getPositionY() + (m_changePosDistance > 0 ? 1: -1) * m_speed * m_deltaTime);
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(StayPlane, Rest)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        TRANSIT_TO_STATE( true, NoTransitionAction, ChangePos );
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(StayPlane, Killed)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        float score = m_score;
        GameObjectManager* pGameObjectManager = GameObjectManager::SingletonPtr();
        GroupInfo groupInfo = pGameObjectManager->UnregisterMonsterGroup(m_groupId);
        if (groupInfo.count == 0)
        {
            score += groupInfo.totalNum * m_score * pGameObjectManager->GetMonsterGroupKillScoreBonus();
        }
        score *= (1 + MainPlayerLogic::Singleton().GetMC()->GetBonusKillingScore());

        MainPlayerLogic::Singleton().IncreaseScore(score);

        Gold* pItem = new Gold(RandomFloat() > 0 ? Gold::eGT_Big : Gold::eGT_Small);
        pItem->autorelease();

        Node* node = BackgroundLogic::Singleton().GetNextSectionNode();
        pItem->setPosition(getPosition() - node->getPosition());
        node->addChild(pItem);

        SoundManager::Singleton().PlayEffect("Sound/Effect_MonsterDie.mp3");
        
        ParticleSystem* crashParticle = ParticleSystemQuad::create("MonsterCrash.plist");
        ParticleBatchNode* crashParticleBatch = ParticleBatchNode::createWithTexture(crashParticle->getTexture());
        crashParticleBatch->addChild(crashParticle, 1);
        crashParticleBatch->setScale(0.5f);
        Node* pNode = BackgroundLogic::Singleton().GetNextSectionNode();
        crashParticleBatch->setPosition(GetPos() - pNode->getPosition());
        pNode->addChild(crashParticleBatch, 1);

        Unspawn();
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
