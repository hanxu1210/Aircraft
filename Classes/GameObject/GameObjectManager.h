#ifndef  _APP_GAMEOBJECTMANAGER_H_
#define  _APP_GAMEOBJECTMANAGER_H_

#include "GameObject/GameObjectEnum.h"
#include "utilities/Singleton.h"

class GameObject;

typedef std::list<GameObject*> TGameObjectList;
typedef std::vector<GameObject*> TGameObjectVector;
typedef std::map< EGameObjectType, TGameObjectList > TGameObjectMap;
struct GroupInfo
{
    GroupInfo() : count(1), totalNum(1) {}
    unsigned count;
    // use to calculate total num
    unsigned totalNum;
};

// Used for update gameObject state. Manage obj map.
class GameObjectManager
{
    DECLARE_SINGLETON(GameObjectManager);

public:
    void Update(float dt);

    void Reset();

    void RegisterGameObject(GameObject* pObj);
    void UnregisterGameObject(GameObject* pObj);

    void GetGameObjectList(EGameObjectType objType, TGameObjectList& objectList);

    void RegisterPauseUpdateGameObjectTypeAll();
    void UnregisterPauseUpdateGameObjectTypeAll();

    void RegisterPauseUpdateGameObjectType(EGameObjectType type);
    void UnregisterPauseUpdateGameObjectType(EGameObjectType type);

    bool CheckIsUnderPause(EGameObjectType type);

    void RegisterMonsterGroup(unsigned groupId);
    GroupInfo UnregisterMonsterGroup(unsigned groupId);

    float GetMonsterGroupKillScoreBonus() const;

private:
    void AddGameObject(GameObject* pObj);
    void DelGameObject(GameObject* pObj);

    void ClearPendingList();
    TGameObjectMap m_gameObjMap;
    TGameObjectVector m_addPendingList;
    TGameObjectVector m_delPendingList;

    int m_updateExceptionSign;


    // Group Id, Info
    typedef std::map< unsigned, GroupInfo> TMonsterGroupCounterMap;
    TMonsterGroupCounterMap m_monsterGroupCounterMap;
    float m_groupKillScoreBonus;


};
#endif //_APP_GAMEOBJECTMANAGER_H_