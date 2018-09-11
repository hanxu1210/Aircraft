#include "CommonHeaders.h"

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "GameData.h"

IMPLEMENT_SINGLETON(GameObjectManager);

GameObjectManager::GameObjectManager()
    : m_updateExceptionSign(0)
    , m_groupKillScoreBonus(0.0f)
{
//     for (unsigned i = 0; i < eGOT_Count; ++i)
//     {
//         m_exceptionSignCount[i] = 0;
//     }    
    m_groupKillScoreBonus = GameData::Singleton().GetValueF(eGDS_Config, 0, "GroupKillScoreBonus");

}

GameObjectManager::~GameObjectManager()
{
    m_gameObjMap.clear();

    ClearPendingList();
}

// Ensure safety while unregister.
void GameObjectManager::Update(float dt)
{
    ClearPendingList();

    for (TGameObjectMap::iterator iter = m_gameObjMap.begin(); iter != m_gameObjMap.end(); ++iter)
    {
        for (TGameObjectList::iterator listIter = iter->second.begin(); listIter != iter->second.end(); ++listIter)
        {
            if (!BIT_CHECK(m_updateExceptionSign, (*listIter)->GetGameObjectType()))
            {
                (*listIter)->StateUpdate(dt);
            }
        }
    }

    // Unspawn
    for (TGameObjectMap::iterator iter = m_gameObjMap.begin(); iter != m_gameObjMap.end(); ++iter)
    {
        for (TGameObjectList::iterator listIter = iter->second.begin(); listIter != iter->second.end(); ++listIter)
        {
            if ((*listIter)->IsUnspawning())
            {
                (*listIter)->Delete();
            }
        }
    }
}

void GameObjectManager::Reset()
{
    m_updateExceptionSign = 0;
}

void GameObjectManager::RegisterGameObject( GameObject* pObj )
{
    if (!pObj)
        return;

    m_addPendingList.push_back(pObj);
}

void GameObjectManager::UnregisterGameObject( GameObject* pObj )
{
    if (!pObj)
        return;

    pObj->retain();
    m_delPendingList.push_back(pObj);
}

void GameObjectManager::GetGameObjectList( EGameObjectType objType, TGameObjectList& objectList )
{
    TGameObjectMap::iterator iter = m_gameObjMap.find(objType);

    if (iter != m_gameObjMap.end())
    {
        objectList = iter->second;
    }
}

void GameObjectManager::RegisterPauseUpdateGameObjectTypeAll()
{
    m_updateExceptionSign = 0xFFFFFFFF;
}

void GameObjectManager::UnregisterPauseUpdateGameObjectTypeAll()
{
    m_updateExceptionSign = 0;
}

void GameObjectManager::RegisterPauseUpdateGameObjectType( EGameObjectType type )
{
    //++m_exceptionSignCount[type];
    BIT_SET(m_updateExceptionSign, type);
}

void GameObjectManager::UnregisterPauseUpdateGameObjectType( EGameObjectType type )
{
    //CCAssert(m_exceptionSignCount[type] != 0, "Did u register this before?!");

    //if (--m_exceptionSignCount[type] == 0)
    //{
        BIT_CLEAR(m_updateExceptionSign, type);
    //}
}

bool GameObjectManager::CheckIsUnderPause( EGameObjectType type )
{
    return BIT_CHECK(m_updateExceptionSign, type) != 0;
}

//////////////////////////////////////////////////////////////////////////
// Private

void GameObjectManager::AddGameObject( GameObject* pObj )
{
    if (!pObj)
        return;

    EGameObjectType type = pObj->GetGameObjectType();
    TGameObjectMap::iterator iter = m_gameObjMap.find(type);

    if (iter != m_gameObjMap.end())
    {
        iter->second.push_back(pObj);
    }
    else
    {
        TGameObjectList list;
        list.push_back(pObj);
        m_gameObjMap.insert(std::pair<EGameObjectType, TGameObjectList>(type, list));
    }
}

void GameObjectManager::DelGameObject( GameObject* pObj )
{
    if (!pObj)
        return;

    EGameObjectType type = pObj->GetGameObjectType();
    TGameObjectMap::iterator iter = m_gameObjMap.find(type);

    if (iter != m_gameObjMap.end())
    {
        iter->second.remove(pObj);

        if (iter->second.empty())
            m_gameObjMap.erase(iter);
    }
//     if (pObj->getReferenceCount() == 1)
//         pObj->release();
}

void GameObjectManager::ClearPendingList()
{
    for (unsigned int i = 0; i < m_addPendingList.size(); ++i)
    {
        AddGameObject(m_addPendingList[i]);
    }

    for (unsigned int i = 0; i < m_delPendingList.size(); ++i)
    {
        DelGameObject(m_delPendingList[i]);
        m_delPendingList[i]->release();
    }

    m_addPendingList.clear();
    m_delPendingList.clear();
}

void GameObjectManager::RegisterMonsterGroup( unsigned groupId )
{
    TMonsterGroupCounterMap::iterator iter = m_monsterGroupCounterMap.find(groupId);
    if (iter != m_monsterGroupCounterMap.end())
    {
        iter->second.count = iter->second.count + 1;
        iter->second.totalNum = iter->second.count;
    }
    else
        m_monsterGroupCounterMap[groupId] = GroupInfo();

    CCLOG("RegisterMonsterGroup: %d. number is:%d", groupId, m_monsterGroupCounterMap[groupId].count);
}

GroupInfo GameObjectManager::UnregisterMonsterGroup( unsigned groupId)
{
    GroupInfo result;
    XAssert(m_monsterGroupCounterMap.find(groupId) != m_monsterGroupCounterMap.end(),"Invalid unregisterMonsterGroup: %d.", groupId);
    XAssert(m_monsterGroupCounterMap[groupId].count > 0, "monster group[id:%d] count is invalid.", groupId);
    unsigned count = m_monsterGroupCounterMap[groupId].count - 1;
    m_monsterGroupCounterMap[groupId].count = count;
    CCLOG("UnRegisterMonsterGroup: %d, number is %d.", groupId, count);
    result = m_monsterGroupCounterMap[groupId];
    if (count == 0)
        m_monsterGroupCounterMap.erase(groupId);
    return result;
}

float GameObjectManager::GetMonsterGroupKillScoreBonus() const
{
    return m_groupKillScoreBonus;
}
