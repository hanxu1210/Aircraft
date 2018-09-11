#include "CommonHeaders.h"

#include "GameData.h"
#include "GameObject/Bullet/Laser.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameObject/GameObjectManager.h"
#include "GameLogic/BackgroundLogic.h"
#include "GameObject/Monster/Monster.h"
#include "GameObject/MainCharacter.h"
#include "GameLogic/SkillDefines.h"

Laser::Laser(EBulletType type, EFactionType faction)
    : Bullet(type, faction)
{
    XAssert(m_type == eBT_Laser, "type must be laser");
}

Laser::~Laser()
{
}

void Laser::onEnter()
{
    GameObject::onEnter();

    m_canRecycle = false;

    std::string image = GameData::Singleton().GetValue(eGDS_Bullet, m_type, "Image");
    m_pMainSprite = Sprite::createWithSpriteFrameName(image.c_str());
    m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor()*2);
    m_pMainSprite->setScale(0.85);
    m_pMainSprite->setPositionX(380);
    addChild(m_pMainSprite, 10);

    INIT_FSM(Move);
}

void Laser::onExit()
{
    GameObject::onExit();
}

void Laser::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;

    GetFsm().Update();
}


float Laser::GetSpeed() const
{
    return m_speed;
}

IMPLEMENT_STATE_BEGIN(Laser, Move)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {  
        MainCharacter* pMC = MainPlayerLogic::Singleton().GetMC();
        setPosition(pMC->GetPos() + Vec2(75,0));

        if (GameObject* gameObject = CheckCollisionByGameObjectType(m_factionType == eFT_Good ? eGOT_Monster : eGOT_MainCharacter))
        {
            if (gameObject)
            {
                gameObject->ReduceHp(m_attackPoint);

                if (m_factionType == eFT_Good && gameObject->GetHp() <= 0)
                {
                    Monster* pMonster = (Monster*)(gameObject);
                    if (pMonster->NeedShakeCamera())
                        BackgroundLogic::Singleton().ShakeCamera(1.0f);
                }
// 
//                 if (m_pParticle)
//                     m_pParticle->removeFromParentAndCleanup(true);
//                 
//                 Unspawn();
            }
        }

        if (pMC->GetCurSkill() != eS_Laser)
            Unspawn();
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    void Laser::SetAttackPoint( unsigned attackPoint )
{
    m_attackPoint = attackPoint;
}



