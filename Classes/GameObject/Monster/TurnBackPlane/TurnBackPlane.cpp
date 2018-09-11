#include "CommonHeaders.h"

#include "GameObject/Monster/TurnBackPlane/TurnBackPlane.h"
#include "GameData.h"
#include "GameObject/GameObjectManager.h"

TurnBackPlane::TurnBackPlane(EMonsterType type, unsigned variationId, unsigned groupId)
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

TurnBackPlane::~TurnBackPlane()
{
}

void TurnBackPlane::onEnter()
{
    Monster::onEnter();
    GameObjectManager::Singleton().RegisterMonsterGroup(m_groupId);
}

void TurnBackPlane::onExit()
{
    Monster::onExit();
}

void TurnBackPlane::PlayMonsterWalkAnimation()
{
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE_BEGIN(TurnBackPlane, Idle)
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

IMPLEMENT_STATE_BEGIN(TurnBackPlane, Move)
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

IMPLEMENT_STATE_BEGIN(TurnBackPlane, MoveOut)
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
            Shoot(Vec2(0.5f * (m_collisionRect.getMaxX() - m_collisionRect.getMinX()), 0.0f));
            TRANSIT_TO_STATE( true, NoTransitionAction, RotateBack );
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(TurnBackPlane, RotateBack)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        //m_changePosDistance = RandomFloat(200.0f, 250.f);
        //m_changePosDistance *= (RandomFloat() > 0 ? 1 : -1);
        //m_startChangePosY = getPositionY();
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        Size s = Director::getInstance()->getWinSize();

        ccBezierConfig bezier;
        bezier.controlPoint_1 = Vec2(100, 300/2);
        bezier.controlPoint_2 = Vec2(200, -300/2);
        bezier.endPosition = Vec2(240,160);

        ActionInterval*  bezierForward = CCBezierBy::create(100, bezier);
        ActionInterval*  bezierBack = bezierForward->reverse();    
        Action*  rep = RepeatForever::create(Sequence::create( bezierForward, bezierBack, NULL));
        runAction(rep);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END