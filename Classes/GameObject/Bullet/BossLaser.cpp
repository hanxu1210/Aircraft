#include "CommonHeaders.h"

#include "GameData.h"
#include "GameObject/Bullet/BossLaser.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameObject/GameObjectManager.h"
#include "GameLogic/BackgroundLogic.h"
#include "GameObject/Monster/Monster.h"
#include "GameObject/MainCharacter.h"
#include "GameLogic/SkillDefines.h"

BossLaser::BossLaser(EBulletType type, EFactionType faction)
    : Bullet(type, faction)
{
    XAssert(m_type == eBT_BossLaser, "type must be BossLaser");
}

BossLaser::~BossLaser()
{
}

void BossLaser::onEnter()
{
    GameObject::onEnter();

    m_canRecycle = false;

    std::string image = GameData::Singleton().GetValue(eGDS_Bullet, m_type, "Image");
    m_pMainSprite = Sprite::createWithSpriteFrameName(image.c_str());
    m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor());
    m_pMainSprite->setScaleX(0.85f);
    m_pMainSprite->setPositionX(330);
    addChild(m_pMainSprite, 10);

    INIT_FSM(Idle);
}

void BossLaser::onExit()
{
    GameObject::onExit();
}

void BossLaser::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;

    GetFsm().Update();
}


float BossLaser::GetSpeed() const
{
    return m_speed;
}

IMPLEMENT_STATE_BEGIN(BossLaser, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        SWITCH_TO_STATE(Move);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    IMPLEMENT_STATE_BEGIN(BossLaser, Move)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {  
        if (GameObject* gameObject = CheckCollisionByGameObjectType(m_factionType == eFT_Good ? eGOT_Monster : eGOT_MainCharacter))
        {
            if (gameObject)
            {
                gameObject->ReduceHp(m_attackPoint);
            }
        }

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    void BossLaser::SetAttackPoint( unsigned attackPoint )
{
    m_attackPoint = attackPoint;
}



