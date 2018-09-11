#include "CommonHeaders.h"

#include "GameObject/Monster/Boss/Emperor.h"
#include "GameData.h"
#include "GameLogic/BackgroundLogic.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameObject/Items/Gold/Gold.h"
#include "GameObject/Bullet/BossLaser.h"
#include "GameObject/GameObjectManager.h"

EmperorGhost::EmperorGhost( EMonsterType type, unsigned variationId, unsigned groupId ) : super(type, variationId, groupId) 
    , m_type(type), m_variantionId(variationId)
{
    Texture2D *texture2D = Director::getInstance()->getTextureCache()->addImage("boss.png");
    auto spBatchNode = SpriteBatchNode::createWithTexture(texture2D);
    addChild(spBatchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss.plist");
}

EmperorGhost::~EmperorGhost()
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("boss.plist");
}

Emperor::Emperor(EMonsterType type, unsigned variationId, unsigned groupId)
    : super(type, variationId, groupId) 
    , m_moveOutLength(0.0f)
    , m_startMoveX(0.0f)
    , m_changePosDistance(0.0f)
    , m_startChangePosY(0.0f)
    , m_skillType(0)
    , m_animationAction(NULL)
{
    m_hp = GameData::Singleton().GetValueF(eGDS_Boss, 0, "Hp");
    m_maxHp = m_hp;
    m_speed = GameData::Singleton().GetValueF(eGDS_Boss, 0, "Speed");
    float height = GameData::Singleton().GetValueF(eGDS_Boss, 0, "CollisionRectHeight");
    float weight = GameData::Singleton().GetValueF(eGDS_Boss, 0, "CollisionRectWidth");
    m_collisionRect = Rect(-weight/2, -height/2, weight, height);
    std::string moveOutLengthStr = GameData::Singleton().GetValue(eGDS_Boss, 0, "MoveOutLength");
    std::vector<std::string> lengthVec = SplitString(moveOutLengthStr, ";");
    if (lengthVec[0] == lengthVec[1])
    {
        m_moveOutLength = atof(lengthVec[0].c_str());
    }
    else
    {
        float min = atof(lengthVec[0].c_str());
        float max = atof(lengthVec[1].c_str());
        m_moveOutLength = RandomFloat(min, max);
    }

    m_moveTime = GameData::Singleton().GetValueF(eGDS_Boss, 0, "NormalStateTime");
    m_preShootTime = GameData::Singleton().GetValueF(eGDS_Boss, 0, "PreShootTime");
    m_shootTime = GameData::Singleton().GetValueF(eGDS_Boss, 0, "ShootTime");
    m_skillProbability = GameData::Singleton().GetValueF(eGDS_Boss, 0, "SkillProbability");
    m_skillTypeProbability = GameData::Singleton().GetValueF(eGDS_Boss, 0, "SkillTypeProbability");
}

Emperor::~Emperor()
{
    if (m_animationAction)
    {
        m_animationAction->release();
    }
}

void Emperor::onEnter()
{
    Monster::onEnter();
    BackgroundLogic::Singleton().ShowEventTime("Boss Time");
}

void Emperor::onExit()
{
    Monster::onExit();
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("boss.plist");
}

//////////////////////////////////////////////////////////////////////////
void EmperorGhost::StateUpdate(float deltaTime)
{
    super::StateUpdate(deltaTime);
    CreateEmperor();
}

void EmperorGhost::CreateEmperor()
{
    if (VisibleRect::getVisibleRectWithSmallBuffer().containsPoint(GetPos()))
    {
        Monster* pMonster = new Emperor(eMT_Emperor, m_variationId, m_groupId);
        pMonster->autorelease();
        pMonster->setPosition(GetPos());
        getParent()->getParent()->addChild(pMonster, 3);

        Unspawn();

        BackgroundLogic::Singleton().SetMoveStatus(eBMS_Stop);
    }
}

IMPLEMENT_STATE_BEGIN(Emperor, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        if (VisibleRect::getVisibleRectWithSmallBuffer().containsPoint(GetPos()))
            TRANSIT_TO_STATE( true, NoTransitionAction, Move );
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(Emperor, Move)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        PlayAnimation(0, std::bind(&Emperor::PlayAnimationCallback, this));
    }
    STATE_CONSTRUCTOR_END

    STATE_UPDATE_BEGIN
    {      
        TRANSIT_TO_STATE( true, NoTransitionAction, MoveOut );
    }
    STATE_UPDATE_END

    STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(Emperor, MoveOut)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_startMoveX = getPositionX();
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        setPositionX(getPositionX() - m_speed * m_deltaTime);
        if (fabs(getPositionX() - m_startMoveX) > m_moveOutLength)
        {
            TRANSIT_TO_STATE( true, NoTransitionAction, ChangePos );
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END


void Emperor::GetMoveTarget()
{
    INT sign = RandomFloat() > 0 ? 1 : -1;
    while(true)
    {
        m_changePosDistance = RandomFloat(200.0f, 250.f);
        m_changePosDistance *= sign;
        sign *= -1;
        float newPosY = getPositionY() + m_changePosDistance;
        if (newPosY > 0 && newPosY < VisibleRect::top().y)
            break;
    }


    m_startChangePosY = getPositionY();
}

void Emperor::PlayAnimation(unsigned state, const std::function<void()>& func)
{
    Animation* pAnim = Animation::create();
    char str[50];
    unsigned size = state == 0 ? 1 : 16;
    for (unsigned i = 0; i < size; ++i)
    {
        sprintf(str,"Emperor/Emperor%d.png", i);
        SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        pAnim->addSpriteFrame(frame);
    }

    pAnim->setDelayPerUnit(0.1f);
    pAnim->setRestoreOriginalFrame(false);

    if (m_animationAction)
    {
        m_pMainSprite->stopAction(m_animationAction);
        m_animationAction->release();
        m_animationAction = NULL;
    }

    ActionInterval* pCCAnimate = Animate::create(pAnim);

    m_animationAction = Sequence::create(pCCAnimate, CallFunc::create(func), NULL);

    m_animationAction->retain();
    m_pMainSprite->runAction(m_animationAction);
}

void Emperor::PlayAnimationCallback()
{

}

IMPLEMENT_STATE_BEGIN(Emperor, ChangePos)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_moveTimer = m_moveTime;
        
        GetMoveTarget();
        Shoot(Vec2(0.5*(m_collisionRect.getMaxX()-m_collisionRect.getMinX()), 0), BackgroundLogic::Singleton().GetNextSectionNode());
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {   
        m_moveTimer -= m_deltaTime;
        if ( m_moveTimer < 0.0f)
        {
            if (m_skillProbability > RandomFloat(0.0f, 100.0f))
            {
                TRANSIT_TO_STATE( true, NoTransitionAction, PreShoot );
            }
            else
            {
                TRANSIT_TO_STATE( true, NoTransitionAction, ChangePos );
            }
        }
        else
        {
            bool reach = (fabs(getPositionY() - m_startChangePosY) > fabs(m_changePosDistance));
            if (reach)
            {
                GetMoveTarget();
                Shoot(Vec2(0.5*(m_collisionRect.getMaxX()-m_collisionRect.getMinX()), 0), BackgroundLogic::Singleton().GetNextSectionNode());
            }
            else
            {
                setPositionY(getPositionY() + (m_changePosDistance > 0 ? 1: -1) * m_speed * m_deltaTime);
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


IMPLEMENT_STATE_BEGIN(Emperor, Killed)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        MainPlayerLogic::Singleton().IncreaseScore(100);

        Gold* pItem = new Gold(RandomFloat() > 0 ? Gold::eGT_Big : Gold::eGT_Small);
        pItem->autorelease();
        Node* node = BackgroundLogic::Singleton().GetNextSectionNode();
        pItem->setPosition(getPosition() - node->getPosition());
        node->addChild(pItem);

        SoundManager::Singleton().PlayEffect("Sound/Effect_MonsterDie.mp3");
        
        ParticleSystem* crashParticle = ParticleSystemQuad::create("MonsterCrash.plist");
        crashParticle->retain();
        ParticleBatchNode* crashParticleBatch = ParticleBatchNode::createWithTexture(crashParticle->getTexture());
        crashParticleBatch->addChild(crashParticle, 1);
        crashParticleBatch->setScale(0.5f);
        Node* pNode = BackgroundLogic::Singleton().GetNextSectionNode();
        crashParticleBatch->setPosition(GetPos() - pNode->getPosition());
        pNode->addChild(crashParticleBatch, 1);

        Unspawn();

        BackgroundLogic::Singleton().SetMoveStatus(eBMS_Normal);
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

IMPLEMENT_STATE_BEGIN(Emperor, PreShoot)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_preShootTimer = m_preShootTime;
        PlayAnimation(1, std::bind(&Emperor::PlayAnimationCallback, this));
        m_skillType = m_skillTypeProbability > RandomFloat(0.0f, 100.0f) ? 0 : 1;
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        m_preShootTimer -= m_deltaTime;
        if ( m_preShootTimer < 0.0f)
            TRANSIT_TO_STATE( true, NoTransitionAction, Shoot );
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(Emperor, Shoot)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_isInvisible = true;
        if (m_skillType == 0)
        {
            // Type 1
            BossLaser* pBossLaser = new BossLaser(eBT_BossLaser, eFT_Bad);
            pBossLaser->autorelease();
            pBossLaser->setPosition(100.0f,150.0f);
            getParent()->addChild(pBossLaser, 10);
            m_pBossLaserVec.push_back(pBossLaser);

            pBossLaser = new BossLaser(eBT_BossLaser, eFT_Bad);
            pBossLaser->autorelease();
            pBossLaser->setPosition(100.0f,618.0f);
            getParent()->addChild(pBossLaser, 10);
            m_pBossLaserVec.push_back(pBossLaser);
        }
        else
        {
            // Type 2
            BossLaser* pBossLaser = new BossLaser(eBT_BossLaser, eFT_Bad);
            pBossLaser->autorelease();
            pBossLaser->setPosition(100.0f,384.0f);
            getParent()->addChild(pBossLaser, 10);
            m_pBossLaserVec.push_back(pBossLaser);
        }
        
        m_shootTimer = m_shootTime;
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        m_shootTimer -= m_deltaTime;
        if ( m_shootTimer < 0.0f)
            TRANSIT_TO_STATE( true, NoTransitionAction, ChangePos );

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
        for (unsigned i = 0; i < m_pBossLaserVec.size(); ++i)
        {
            m_pBossLaserVec[i]->Unspawn();
        }
        m_pBossLaserVec.clear();

        m_isInvisible = false;

        PlayAnimation(0, std::bind(&Emperor::PlayAnimationCallback, this));
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END
