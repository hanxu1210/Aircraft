#include "CommonHeaders.h"

#include "GameData.h"
#include "GameObject/Bullet/Bullet.h"
#include "GameObject/GameObjectManager.h"
#include "GameLogic/BackgroundLogic.h"
#include "GameObject/Monster/Monster.h"

USING_NS_CC;

Bullet::Bullet(EBulletType type, EFactionType faction)
    : GameObject(10.0f, eGOT_Bullet, 10.0f)
    , m_type(type)
    , m_pMainSprite(NULL)
    , m_pParticle(NULL)
    , m_targetPos(Vec2::ZERO)
    , m_direction(Vec2::ZERO)
    , m_speed(0.0f)
    , m_deltaTime(0.0f)
    , m_factionType(faction)
    , m_attackPoint(1)
{
    Setup(type, faction);
}

Bullet::~Bullet()
{
}

void Bullet::Setup(EBulletType type, EFactionType faction)
{
    GameObject::Setup(10.0f, eGOT_Bullet, 10.0f);
    m_type = type;
    m_pParticle = NULL;
    m_targetPos = Vec2::ZERO;
    m_direction = Vec2::ZERO;
    m_speed = 0.0f;
    m_deltaTime = 0.0f;
    m_factionType = faction;
    m_attackPoint = 1;

    m_canRecycle = true;

    GameData* pGameData = GameData::SingletonPtr();
    m_speed = pGameData->GetValueF(eGDS_Bullet, m_type, "Speed");
    float height = pGameData->GetValueF(eGDS_Bullet, m_type, "CollisionRectHeight");
    float weight = pGameData->GetValueF(eGDS_Bullet, m_type, "CollisionRectWidth");
    float x = pGameData->GetValueF(eGDS_Bullet, m_type, "CollisionRectX");
    float y = pGameData->GetValueF(eGDS_Bullet, m_type, "CollisionRectY");
    m_collisionRect = Rect(x, y, weight, height);
    m_factionType = (EFactionType)((unsigned)(pGameData->GetValueF(eGDS_Bullet, m_type, "Faction")));
    m_attackPoint = (unsigned)pGameData->GetValueF(eGDS_Bullet, m_type, "AttackPoint") + pGameData->m_mcExtraAttackPointVec[pGameData->m_mcIndex];
}

void Bullet::onEnter()
{
    GameObject::onEnter();

    if (!m_pMainSprite)
    {
        std::string image = GameData::Singleton().GetValue(eGDS_Bullet, m_type, "Image");
        m_pMainSprite = Sprite::createWithSpriteFrameName(image.c_str());
        m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor());
        addChild(m_pMainSprite, 10);
    }
    INIT_FSM(Idle);
}

void Bullet::onExit()
{

    GameObject::onExit();
}

void Bullet::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;

    GetFsm().Update();

    CheckUnspawn();
}

bool Bullet::CheckUnspawn()
{
    if (super::CheckUnspawn())
    {
        if (m_pParticle)
            m_pParticle->removeFromParentAndCleanup(true);
        return true;
    }   
    return false;
}

float Bullet::GetSpeed() const
{
    return m_speed;
}

IMPLEMENT_STATE_BEGIN(Bullet, Idle)
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

    IMPLEMENT_STATE_BEGIN(Bullet, Move)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {  
        setPositionX(getPositionX() + m_speed * m_deltaTime);

        // check if it's inside screen. Once bullet collision is out of screen, don't check collision, but sprite is still showed.
        Rect visibleRect = VisibleRect::getVisibleRect();
        Rect rect(visibleRect.origin.x + m_collisionRect.origin.x, visibleRect.origin.y + m_collisionRect.origin.y,
            visibleRect.size.width + m_collisionRect.size.width, visibleRect.size.height + m_collisionRect.size.height);

        if (rect.containsPoint(GetPos()))
        {
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

                    if (m_pParticle)
                        m_pParticle->removeFromParentAndCleanup(true);

                    SoundManager::Singleton().PlayEffect("Sound/Effect_BulletExploded.WAV");
                    
                    Unspawn();
                }
            }
        }
        else
        {
            CCLOG("bullet is out of screen");
        }

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    void Bullet::SetAttackPoint( unsigned attackPoint )
{
    m_attackPoint = attackPoint;
}




