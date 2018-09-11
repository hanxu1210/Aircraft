#include "CommonHeaders.h"

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObjectFactory.h"

GameObject::GameObject()
    : m_hp(0)
    , m_maxHp(0)
    , m_type(eGOT_Invalid)
    , m_collisionRect()
    , m_isUnspawning(false)
    , m_isInvisible(false)
    , m_canRecycle(false)
{
}

GameObject::GameObject( unsigned hp, EGameObjectType type, float collisonRadius )
    : m_hp(hp)
    , m_maxHp(hp)
    , m_type(type)
    , m_isUnspawning(false)
    , m_isInvisible(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::Setup(unsigned hp, EGameObjectType type, float collisonRadius)
{
    m_hp = hp;
    m_maxHp = hp;
    m_type = type;
    m_isUnspawning = false;
    m_isInvisible = false;
}

void GameObject::onEnter()
{
    Node::onEnter();

    if (GameObjectManager::IsSingletonCreated())
        GameObjectManager::Singleton().RegisterGameObject(this);
}

void GameObject::onExit()
{
    if (GameObjectManager::IsSingletonCreated())
        GameObjectManager::Singleton().UnregisterGameObject(this);

    Node::onExit();
}

void GameObject::Unspawn()
{
    m_isUnspawning = true;
}

void GameObject::Delete()
{
    if (m_canRecycle)
    {
        switch(m_type)
        {
        case eGOT_Bullet:
            GameObjectFactory::Singleton().RemoveBullet(this);
            break;
        case eGOT_Monster:
            GameObjectFactory::Singleton().RemoveMonster(this);
            break;
            default:break;
        }
    }    

    removeFromParent();
}

void GameObject::StateUpdate(float deltaTime)
{
}

const Rect& GameObject::GetCollisionRect() const
{
    return m_collisionRect;
}

const EGameObjectType& GameObject::GetGameObjectType()
{
    return m_type;
}

int GameObject::GetDetailTypeIndex()
{
    return static_cast<int>(m_type);
}

bool GameObject::CheckCollision(GameObject* pGameObject)
{
    if (pGameObject)
    {
        Vec2 position = GetPos();
        Rect rect(
            position.x + m_collisionRect.getMinX(),
            position.y + m_collisionRect.getMinY(),
            m_collisionRect.size.width, m_collisionRect.size.height);

        Vec2 targetPosition = pGameObject->GetPos();
        Rect targetRect(
            targetPosition.x + pGameObject->GetCollisionRect().getMinX(),
            targetPosition.y + pGameObject->GetCollisionRect().getMinY(),
            pGameObject->GetCollisionRect().size.width, pGameObject->GetCollisionRect().size.height);
        return rect.intersectsRect(targetRect);
    }
    else
        return false;
}

GameObject* GameObject::CheckCollisionByGameObjectType( EGameObjectType gameObjectType )
{
    TGameObjectList objectList;
    if (GameObjectManager::IsSingletonCreated())
        GameObjectManager::Singleton().GetGameObjectList(gameObjectType, objectList);
    for (TGameObjectList::iterator iter = objectList.begin(); iter != objectList.end(); ++iter)
    {
        if ((*iter)->CheckCollision(this))
        {
            return *iter;
        }
    }
    return NULL;
}

void GameObject::ReduceHp( unsigned hp )
{
    if (!m_isInvisible)
    {
        m_hp -= hp;

        if (m_hp <= 0 && m_hp + hp > 0)
            Killed();
    }
}

void GameObject::IncreaseHp( unsigned hp )
{
    m_hp += hp;
}

INT GameObject::GetHp() const
{
    return m_hp;
}

INT GameObject::GetMaxHp() const
{
    return m_maxHp;
}

void GameObject::Killed()
{
}

bool GameObject::CheckUnspawn()
{
    if (!VisibleRect::getVisibleRectWithBuffer().containsPoint(GetPos()))
    {
        Unspawn();
        return true;
    }
    return false;
}

Vec2 GameObject::GetPos()
{
    if (getParent())
        return getParent()->convertToWorldSpace(getPosition());
    return
        getPosition();
}

bool GameObject::IsUnspawning() const
{
    return m_isUnspawning;
}

// void GameObject::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
// {
//     glLineWidth( 2.0f );
//     ccDrawRect(m_collisionRect.origin, Vec2(m_collisionRect.getMaxX(), m_collisionRect.getMaxY()));
// }


