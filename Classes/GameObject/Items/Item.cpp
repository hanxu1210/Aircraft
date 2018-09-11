#include "CommonHeaders.h"

#include "GameObject/Items/Item.h"
#include "GameObject/GameObjectManager.h"

Item::Item()
    : GameObject(10.0f, eGOT_Item,10.0f)
    , m_pMainSprite(NULL)
    , m_speed(0.0f)
    , m_deltaTime(0.0f)
    , m_isHit(false)
{
}

Item::~Item()
{
}

void Item::onEnter()
{
    GameObject::onEnter();
  
    float height = 30;
    float weight = 30;
    m_collisionRect = Rect(-weight/2, -height/2, weight, height);

    INIT_FSM(Move);
}

void Item::onExit()
{
    GameObject::onExit();
}

void Item::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;

    GetFsm().Update();
    CheckUnspawn();
    if (m_isHit)
    {
        GetFsm().SwitchState(STATE(Hit));
    }
}

bool Item::CheckUnspawn()
{
    if (GetPos().x < VisibleRect::getVisibleRectWithBuffer().getMinX())
    {
        Unspawn();
        return true;
    }
    return false;
}

IMPLEMENT_STATE_BEGIN(Item, Move)
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

    IMPLEMENT_STATE_BEGIN(Item, Hit)
{
    STATE_CONSTRUCTOR_BEGIN
    {
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