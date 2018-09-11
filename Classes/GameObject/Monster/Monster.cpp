#include "CommonHeaders.h"

#include "GameObject/Monster/Monster.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameObject/Bullet/Bullet.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/Items/Gold/Gold.h"
#include "GameData.h"
#include "GameLogic/BackgroundLogic.h"
#include "GameObject/MainCharacter.h"
#include "GameObject/GameObjectFactory.h"

Monster::Monster(EMonsterType type, unsigned variationId, unsigned groupId)
    : GameObject(1.0f, eGOT_Monster,10.0f)
    , m_type(type)
    , m_variationId(variationId)
    , m_groupId(groupId)
    , m_pMainSprite(NULL)
    , m_speed(0.0f)
    , m_deltaTime(0.0f)
    , m_shake(false)
    , m_score(0)
    , m_pTintAction(NULL)
    , m_lastInGroup(false)
{
    Setup(type, variationId, groupId);
}

Monster::~Monster()
{
    if (m_pTintAction)
    {
        m_pTintAction->retain();
    }
}

void Monster::Setup(EMonsterType type, unsigned variationId, unsigned groupId)
{
    GameObject::Setup(10.0f, eGOT_Monster, 10.0f);

    m_type = type;
    m_variationId = variationId;
    m_groupId = groupId;
    m_speed = 0.0f;
    m_deltaTime = 0.0f;
    m_shake = false;
    m_score = 0;
    m_pTintAction = NULL;
    m_lastInGroup = false;

    m_canRecycle = true;

    GameData* pGameData = GameData::SingletonPtr();
    m_hp = pGameData->GetValueF(eGDS_Monster, m_variationId, "Hp");
    m_speed = pGameData->GetValueF(eGDS_Monster, m_variationId, "Speed");
    float height = pGameData->GetValueF(eGDS_Monster, m_variationId, "CollisionRectHeight");
    float weight = pGameData->GetValueF(eGDS_Monster, m_variationId, "CollisionRectWidth");
    m_collisionRect = Rect(-weight/2, -height/2, weight, height);
    m_score = pGameData->GetValueF(eGDS_Monster, m_variationId, "Score");
}

void Monster::onEnter()
{
    GameObject::onEnter();

    if (!m_pMainSprite)
    {
        std::string image = GameData::Singleton().GetValue(eGDS_Monster, m_variationId, "Image");
        m_pMainSprite = Sprite::createWithSpriteFrameName(image.c_str());
        m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor());
        addChild(m_pMainSprite);
    }
    INIT_FSM(Idle);
}

void Monster::onExit()
{
    GameObject::onExit();
}

void Monster::Killed()
{
    if (GetFsm().GetCurrentState() != STATE(Killed))
        GetFsm().SwitchState(STATE(Killed));
}

void Monster::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;
    GetFsm().Update();
    
    // Check Attack
    if (GameObject* gameObject = CheckCollisionByGameObjectType(eGOT_MainCharacter))
    {
        if (gameObject)
            gameObject->ReduceHp(1);
    }
    
    if (!GetFsm().IsCurrentState(STATE(Idle)))
        CheckUnspawn();
}

void Monster::Shoot(Vec2 offset, Node* pNode)
{
    if (pNode)
    {
        Bullet* pBullet = GameObjectFactory::Singleton().CreateBullet(pNode, eBT_Normal, eFT_Bad);
        pBullet->setPosition(getPosition() - offset);

        pBullet->setPosition(pBullet->getPosition() - pNode->getPosition());
    }
    else
    {
        Bullet* pBullet = GameObjectFactory::Singleton().CreateBullet(getParent(), eBT_Normal, eFT_Bad);
        pBullet->setPosition(getPosition() - offset);
    }
}

bool Monster::NeedShakeCamera()
{
    return m_lastInGroup;
    //return (GameData::Singleton().GetValue(eGDS_Monster, m_variationId, "Shake") == "Yes");
}

void Monster::Tint()
{
    if (m_pTintAction)
    {
        m_pMainSprite->stopAction(m_pTintAction);
        m_pTintAction->release();
        m_pTintAction = NULL;
        m_pMainSprite->setColor(Color3B(255,255,255));
    }
    ActionInterval* action = CCTintBy::create(0.1f, 0, -255, -255);
    ActionInterval* action_back = action->reverse();
    m_pTintAction = Sequence::create( action, action_back, action, action_back, NULL );
    m_pTintAction->retain();
    m_pMainSprite->runAction(m_pTintAction);
}

void Monster::ReduceHp( unsigned hp )
{
    GameObject::ReduceHp(hp);
    Tint();
}

IMPLEMENT_STATE_BEGIN(Monster, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

    STATE_UPDATE_BEGIN
    {
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(Monster, Killed)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        float score = m_score;
        GameObjectManager* pGameObjectManager = GameObjectManager::SingletonPtr();
        GroupInfo groupInfo = pGameObjectManager->UnregisterMonsterGroup(m_groupId);
        if (groupInfo.count == 0)
        {
            score += groupInfo.totalNum * m_score * pGameObjectManager->GetMonsterGroupKillScoreBonus();
            m_lastInGroup = true;
        }
        score *= (1 + MainPlayerLogic::Singleton().GetMC()->GetBonusKillingScore());

        MainPlayerLogic::Singleton().IncreaseScore(score);

        Gold* pItem = new Gold(RandomFloat() > 0.9f ? Gold::eGT_Big : Gold::eGT_Small);
        pItem->autorelease();

        pItem->setPosition(getPosition());
        getParent()->addChild(pItem);

        SoundManager::Singleton().PlayEffect("Sound/Effect_MonsterDie.mp3");

        ParticleSystem* crashParticle = ParticleSystemQuad::create("MonsterCrash.plist");
        ParticleBatchNode* crashParticleBatch = ParticleBatchNode::createWithTexture(crashParticle->getTexture());
        crashParticleBatch->addChild(crashParticle, 1);
        crashParticleBatch->setScale(0.5f);
        Node* pNode = BackgroundLogic::Singleton().GetNextSectionNode();
        crashParticleBatch->setPosition(GetPos() - pNode->getPosition());
        pNode->addChild(crashParticleBatch, 1);

        Unspawn();
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

