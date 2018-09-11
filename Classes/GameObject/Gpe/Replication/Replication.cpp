#include "CommonHeaders.h"

#include "GameData.h"
#include "GameObject/Gpe/Replication/Replication.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameObject/Bullet/Bullet.h"
#include "GameObject/MainCharacter.h"

Replication::Replication()
    : m_pMC(NULL)
    , m_shootTimer(0.0f)
    , m_bulletCount(0)
{
    m_pMC = MainPlayerLogic::Singleton().GetMC();
}

Replication::~Replication()
{
}

void Replication::onEnter()
{
    GameObject::onEnter();

    INIT_FSM(Idle);
}

void Replication::onExit()
{
    GameObject::onExit();
}

void Replication::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;
    GetFsm().Update();
}

void Replication::Shoot()
{
    Bullet* pBullet = new Bullet(eBT_CharmNormal, eFT_Good);
    pBullet->autorelease();
    pBullet->setPosition(GetPos() + Vec2(75,0));
    m_pMC->getParent()->addChild(pBullet);

}

IMPLEMENT_STATE_BEGIN(Replication, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_pMainSprite = Sprite::createWithSpriteFrameName("Replication0.png");
        m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor() * 0.5f);
        addChild(m_pMainSprite);

        //
        Animation* pAnim = Animation::create();
        char str[50];
        for (unsigned i = 0; i < 2; ++i)
        {
            sprintf(str,"Replication%d.png", i);
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            pAnim->addSpriteFrame(frame);
        }

        pAnim->setDelayPerUnit(0.1f);
        pAnim->setRestoreOriginalFrame(false);

        ActionInterval* pCCAnimate = Animate::create(pAnim);

        pCCAnimate = RepeatForever::create(pCCAnimate);

        m_pMainSprite->runAction(pCCAnimate);

        //
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        m_shootTimer -= m_deltaTime;
        if (m_shootTimer < 0)
        {
            Shoot();
            m_bulletCount++;
            if (m_bulletCount >= 3)
            {
                m_bulletCount = 0;
                m_shootTimer = m_pMC->GetShootInterval() * 1.0f;
            }
            else
                m_shootTimer = m_pMC->GetShootInterval() * 0.3f;
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END