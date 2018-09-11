#include "CommonHeaders.h"

#include "GameObject/Monster/StreightPlane/StreightPlane.h"
#include "GameData.h"
#include "GameObject/GameObjectManager.h"

StreightPlane::StreightPlane(EMonsterType type, unsigned variationId, unsigned groupId)
    : super(type, variationId, groupId)
{    
}

StreightPlane::~StreightPlane()
{
}

void StreightPlane::onEnter()
{
    Monster::onEnter();
    GameObjectManager::Singleton().RegisterMonsterGroup(m_groupId);
}

void StreightPlane::onExit()
{
    Monster::onExit();
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE_BEGIN(StreightPlane, Idle)
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

IMPLEMENT_STATE_BEGIN(StreightPlane, Move)
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

IMPLEMENT_STATE_BEGIN(StreightPlane, MoveOut)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        setPositionX(getPositionX() - m_speed * m_deltaTime);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    void StreightPlane::PlayMonsterWalkAnimation()
{

}
