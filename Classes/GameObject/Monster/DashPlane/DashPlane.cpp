#include "CommonHeaders.h"

#include "GameObject/Monster/DashPlane/DashPlane.h"
#include "GameData.h"
#include "GameObject/GameObjectManager.h"

DashPlane::DashPlane(EMonsterType type, unsigned variationId, unsigned groupId)
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

DashPlane::~DashPlane()
{
}

void DashPlane::onEnter()
{
    Monster::onEnter();
    GameObjectManager::Singleton().RegisterMonsterGroup(m_groupId);
}

void DashPlane::onExit()
{
    Monster::onExit();
}

void DashPlane::PlayMonsterWalkAnimation()
{
    /*float scaleFactor = Director::getInstance()->getContentScaleFactor();

    float x = 32 / scaleFactor;
    float y = 48 / scaleFactor;

    CCSpriteFrame *frame0=CCSpriteFrame::create("DashPlane.png",CCRectMake(0, 0, x, y)); 
    CCSpriteFrame *frame1=CCSpriteFrame::create("DashPlane.png",CCRectMake(x, 0, x, y)); 
    CCSpriteFrame *frame2=CCSpriteFrame::create("DashPlane.png",CCRectMake(x*2, 0, x, y)); 
    CCSpriteFrame *frame3=CCSpriteFrame::create("DashPlane.png",CCRectMake(x*3, 0, x, y)); 

    CCArray *animFrames = new CCArray; 
    animFrames->addObject(frame0); 
    animFrames->addObject(frame1); 
    animFrames->addObject(frame2); 
    animFrames->addObject(frame3); 

    Animation *pAnim = Animation::createWithSpriteFrames(animFrames, 0.1f); 
    animFrames->release();

    m_pMainSprite->runAction(RepeatForever::create(Animate::create(pAnim)));
    */
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE_BEGIN(DashPlane, Idle)
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

IMPLEMENT_STATE_BEGIN(DashPlane, Move)
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

IMPLEMENT_STATE_BEGIN(DashPlane, MoveOut)
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

IMPLEMENT_STATE_BEGIN(DashPlane, ChangePos)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_changePosDistance = RandomFloat(200.0f, 250.f);
        m_changePosDistance *= (RandomFloat() > 0 ? 1 : -1);
        m_startChangePosY = getPositionY();
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        bool reach = (fabs(getPositionY() - m_startChangePosY) > fabs(m_changePosDistance));
        if (reach)
        {
            TRANSIT_TO_STATE( true, NoTransitionAction, Dash );
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

IMPLEMENT_STATE_BEGIN(DashPlane, Dash)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        setPositionX(getPositionX() - 3 * m_speed * m_deltaTime);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END