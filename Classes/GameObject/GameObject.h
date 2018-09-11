#ifndef  _APP_GameObject_H_
#define  _APP_GameObject_H_

#include "utilities/FSM/FsmInclude.h"
#include "GameObject/GameObjectEnum.h"

USING_NS_CC;

class GameObject : public Node
{
public:
    GameObject();
    GameObject(unsigned hp, EGameObjectType type, float collisonRadius);
    virtual ~GameObject();

    void Setup(unsigned hp, EGameObjectType type, float collisonRadius);

    virtual void onEnter();
    virtual void onExit();

    virtual void Killed();
    void Unspawn();
    bool IsUnspawning() const;
    virtual void Delete();

    virtual void StateUpdate(float deltaTime);

    const EGameObjectType& GetGameObjectType();
    virtual int GetDetailTypeIndex();

    GameObject* CheckCollisionByGameObjectType(EGameObjectType gameObjectType);

    virtual void ReduceHp(unsigned hp);
    virtual void IncreaseHp(unsigned hp);
    INT GetHp() const;
    INT GetMaxHp() const;

    const Rect& GetCollisionRect() const;

    virtual bool CheckUnspawn();

    Vec2 GetPos();

    //void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

private:
    virtual bool CheckCollision(GameObject* pGameObject);

protected:
    INT m_hp;
    INT m_maxHp;
    EGameObjectType m_type;
    
    Rect m_collisionRect;
    bool m_isUnspawning;
    bool m_isInvisible;

    bool m_canRecycle;
};

#endif // _APP_MainCharacter_H_