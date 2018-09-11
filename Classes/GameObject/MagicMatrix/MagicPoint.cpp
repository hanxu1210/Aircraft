#include "CommonHeaders.h"

#include "GameObject/MagicMatrix/MagicPoint.h"
#include "Ui/UiManager.h"

MagicPoint::MagicPoint()
    : GameObject(0.0f, eGOT_MagicMatrix,15.0f)
    , m_collisionRadius(50.0f)
{
}

MagicPoint::~MagicPoint()
{
}

void MagicPoint::onEnter()
{
    GameObject::onEnter();
    
    EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchMoved = CC_CALLBACK_2(MagicPoint::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MagicPoint::onExit()
{
    GameObject::onExit();
}

void MagicPoint::onTouchMoved( Touch *pTouch, Event *pEvent )
{
    Vec2 location = pTouch->getLocation();
    float dis = location.getDistance(GetPos()); //ccpDistance(location, GetPos());
    if ( dis < m_collisionRadius )
    {
        Unspawn();
    }
}