#include "CommonHeaders.h"

#include "GameObject/GameObjectFactory.h"
#include "GameObject/GameObject.h"
#include "GameObject/Bullet/Bullet.h"
#include "GameObject/Monster/Monster.h"
#include "GameObject/Monster/DashPlane/DashPlane.h"
#include "GameObject/Monster/TurnBackPlane/TurnBackPlane.h"
#include "GameObject/Monster/StreightPlane/StreightPlane.h"
#include "GameObject/Monster/StayPlane/StayPlane.h"
#include "GameObject/Monster/Boss/Emperor.h"

IMPLEMENT_SINGLETON(GameObjectFactory);

GameObjectFactory::GameObjectFactory()
{
    Precreate();
}

GameObjectFactory::~GameObjectFactory()
{
    TBulletsMap::iterator iterBullet = m_bulletRecycleTypeMap.begin();
    for(; iterBullet != m_bulletRecycleTypeMap.end(); ++iterBullet)
    {
        for (unsigned i = 0; i < iterBullet->second.size(); ++i)
        {
            iterBullet->second[i]->release();
        }
    }
    m_bulletRecycleTypeMap.clear();
    
    TMonstersMap::iterator iterMonster = m_monsterRecycleTypeMap.begin();
    for(; iterMonster != m_monsterRecycleTypeMap.end(); ++iterMonster)
    {
        for (unsigned i = 0; i < iterMonster->second.size(); ++i)
        {
            iterMonster->second[i]->release();
        }
    }
    m_monsterRecycleTypeMap.clear();
}

void GameObjectFactory::Init( Layer* pParentLayer )
{
//     m_pLayer = Layer::create();
//     m_pLayer->setVisible(false);
// 
//     pParentLayer->addChild(m_pLayer);
}

Bullet* GameObjectFactory::CreateBullet(Node* pParent, EBulletType type, EFactionType faction)
{
    Bullet* pBullet = NULL;
    TBulletsMap::iterator iter = m_bulletRecycleTypeMap.find(type);
    if (iter != m_bulletRecycleTypeMap.end() && iter->second.size() > 0) // can find recycled
    {
        pBullet = iter->second.back();

        pBullet->Setup(type, faction);
        iter->second.pop_back();

        if (pParent)
        {
            pParent->addChild(pBullet);
        }
        pBullet->release();
    }
    else
    {
        pBullet = new Bullet(type, faction);
        pBullet->autorelease();
        if (pParent)
        {
            pParent->addChild(pBullet);
        }  
    }

    return pBullet;
}

void GameObjectFactory::RemoveBullet( GameObject* pBullet )
{
    pBullet->retain();
    EBulletType type = ((Bullet*)(pBullet))->GetType();
    m_bulletRecycleTypeMap[type].push_back((Bullet*)pBullet);
}

Monster* GameObjectFactory::CreateMonster(Node* pParent, EMonsterType type, unsigned variationId, unsigned groupId )
{
    Monster* pMonster = NULL;
    TMonstersMap::iterator iter = m_monsterRecycleTypeMap.find(type);
    if (iter != m_monsterRecycleTypeMap.end() && iter->second.size() > 0) // can find recycled
    {
        pMonster = iter->second.back();

        if (pMonster->GetType() == eMT_StayPlane)
        {
            XAssert(true, "");
        }
        pMonster->Setup(type, variationId, groupId);
        iter->second.pop_back();

        if (pParent)
        {
            pParent->addChild(pMonster);
        }

        pMonster->release();
    }
    else
    {
        switch (type)
        {
        case eMT_DashPlane:
            {
                pMonster = new DashPlane(type, variationId, groupId);
            }
            break;
        case eMT_TurnBackPlane:
            {
                pMonster = new TurnBackPlane(type, variationId, groupId);
            }
            break;
        case eMT_StreightPlane:
            {
                pMonster = new StreightPlane(type, variationId, groupId);
            }
            break;
        case eMT_StayPlane:
            {
                pMonster = new StayPlanGhost(type, variationId, groupId);
            }
            break;
        case eMT_EmperorGhost:
            {
                pMonster = new EmperorGhost(type, variationId, groupId);
            }
            break;
        default:
            XAssert(false, "");
            break;
        }
        if (pMonster)
        {
            pMonster->autorelease();

            if (pParent)
            {
                pParent->addChild(pMonster);
            }           
        }
    }

    return pMonster;
}

void GameObjectFactory::RemoveMonster( GameObject* pMonster )
{
    EMonsterType type = ((Monster*)(pMonster))->GetType();

    pMonster->retain();
    m_monsterRecycleTypeMap[type].push_back((Monster*)pMonster);
}

void GameObjectFactory::Precreate()
{
    std::vector<Monster*> monsterVec;
    for (unsigned i = 0; i < 10; ++i)
    {
        Monster* pMonster = NULL;
        pMonster = CreateMonster(nullptr, eMT_DashPlane, 0, 0);
        monsterVec.push_back(pMonster);
        pMonster = CreateMonster(nullptr, eMT_StreightPlane, 0, 0);
        monsterVec.push_back(pMonster);
        pMonster = CreateMonster(nullptr, eMT_TurnBackPlane, 0, 0);
        monsterVec.push_back(pMonster);
        pMonster = CreateMonster(nullptr, eMT_StayPlane, 0, 0);
        monsterVec.push_back(pMonster);
    }

    for (unsigned i = 0; i < monsterVec.size(); ++i)
    {
        RemoveMonster(monsterVec[i]);
    }
}
