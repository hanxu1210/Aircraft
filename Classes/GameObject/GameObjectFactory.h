#ifndef  _APP_GAMEOBJECTFACTORY_H_
#define  _APP_GAMEOBJECTFACTORY_H_

#include "GameObject/GameObjectEnum.h"
#include "utilities/Singleton.h"

class GameObject;
class Bullet;
class Monster;

class GameObjectFactory
{
    DECLARE_SINGLETON(GameObjectFactory);

public:
    void Init(Layer* pParentLayer);

    void Precreate();

    Bullet* CreateBullet(Node* pParent, EBulletType type, EFactionType faction);
    void RemoveBullet(GameObject* pBullet);

    Monster* CreateMonster(Node* pParent, EMonsterType type, unsigned variationId, unsigned groupId);
    void RemoveMonster(GameObject* pMonster);

private:
    typedef std::map< unsigned, std::vector<Bullet*> > TBulletsMap;
    TBulletsMap m_bulletRecycleTypeMap;
    typedef std::map< unsigned, std::vector<Monster*> > TMonstersMap;
    TMonstersMap m_monsterRecycleTypeMap;

    
    Layer* m_pLayer;
};
#endif //_APP_GAMEOBJECTFACTORY_H_