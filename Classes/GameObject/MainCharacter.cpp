#include "CommonHeaders.h"

#include "GameData.h"
#include "GameObject/MainCharacter.h"
#include "Ui/UiManager.h"
#include "Scene/SceneManager.h"
#include "GameObject/Bullet/Bullet.h"
#include "GameObject/Bullet/Laser.h"
#include "GameLogic/BackgroundLogic.h"
#include "GameLogic/SkillDefines.h"
#include "GameObject/Gpe/Replication/Replication.h"
#include "GameObject/GameObjectManager.h"
#include "Scene/SceneGame.h"
#include "GameObject/GameObjectFactory.h"

MainCharacter::MainCharacter(unsigned index)
    : GameObject(10.0f, eGOT_MainCharacter,10.0f)
    , m_index(index)
    , m_pMainSprite(NULL)
    , m_isGasOn(false)
    , m_isShooting(true)
    , m_shootTimer(0.0f)
    , m_shootInterval(0.0f)
    , m_speedY(0.0f)
    , m_gasSpeedY(0.0f)
    , m_speedX(0.0f)
    , m_gravityAccelarte(0.0f)
    , m_gravitySpeed(0.0f)
    , m_invisibleTime(0.0f), m_invisibleTimer(0.0f)
    , m_isLaunchingSkill(false)
    , m_moveYThreshold(0.0f)
    , m_enterScene(true)
    , m_bulletCount(0)
    , m_maxMp(0), m_mp(0)
    , m_bonusKillingScore(0.0f)
    , m_curSkill(eS_Null)
    , m_animationAction(NULL), m_animationFlyUpNum(0), m_animationFlyUpInterval(0.0f), m_animationFlyDownNum(0), m_animationFlyDownInterval(0.0f), m_animationTransformNum(0), m_animationTransformInterval(0.0f), m_animationStandNum(0), m_animationStandInterval(0.0f), m_playingTransformAnimation(false)
{
    char str[20];
    sprintf(str,"Mc/%d.png",m_index);

    //Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);
    Texture2D *texture2D = Director::getInstance()->getTextureCache()->addImage(str);
    auto spBatchNode = SpriteBatchNode::createWithTexture(texture2D);
    addChild(spBatchNode);

    sprintf(str,"Mc/%d.plist",m_index);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(str);
}

MainCharacter::~MainCharacter()
{
    if (m_animationAction)
    {
        m_animationAction->release();
        m_animationAction = nullptr;
    }

    char str[20];
    sprintf(str,"Mc/%d.plist",m_index);
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(str);
}

void MainCharacter::onEnter()
{
    GameObject::onEnter();
    
    GameData* pGameData = GameData::SingletonPtr();
    m_hp = pGameData->GetValueF(eGDS_MC, m_index, "Hp") + pGameData->m_mcExtraHpVec[pGameData->m_mcIndex];
    m_initPositionX = pGameData->GetValueF(eGDS_MC, m_index, "InitX");
    float initY = pGameData->GetValueF(eGDS_MC, m_index, "InitY");
    setPosition(m_initPositionX, initY);
    m_speedX = pGameData->GetValueF(eGDS_MC, m_index, "SpeedX");
    float height = pGameData->GetValueF(eGDS_MC, m_index, "CollisionRectHeight");
    float weight = pGameData->GetValueF(eGDS_MC, m_index, "CollisionRectWidth");
    m_collisionRect = Rect(-weight/2, -height/2, weight, height);
    m_shootInterval = pGameData->GetValueF(eGDS_MC, m_index, "ShootComboInterval");
    m_gasSpeedY = pGameData->GetValueF(eGDS_MC, m_index, "GasSpeedY");
    m_gravityAccelarte = pGameData->GetValueF(eGDS_MC, m_index, "GravityAccelarte");
    m_invisibleTime = pGameData->GetValueF(eGDS_MC, m_index, "InvisibleTime");
    m_invisibleTimer = m_invisibleTime;
    m_accelerateSpeedXAfterDash = pGameData->GetValueF(eGDS_MC, m_index, "AccelerateAfterDash");
    m_maxMp = pGameData->GetValueF(eGDS_MC, m_index, "MaxMp");
    m_skillMpCost = pGameData->GetValueF(eGDS_MC, m_index, "SkillMpCost");
    
    //Apply Bonus
    ApplyBonus();

    // Animation
    m_animationFlyUpNum = pGameData->GetValueF(eGDS_MC, m_index, "AnimationFlyUpNum");
    m_animationFlyUpInterval = pGameData->GetValueF(eGDS_MC, m_index, "AnimationFlyUpInterval");
    m_animationFlyDownNum = pGameData->GetValueF(eGDS_MC, m_index, "AnimationFlyDownNum");
    m_animationFlyDownInterval = pGameData->GetValueF(eGDS_MC, m_index, "AnimationFlyDownInterval");
    m_animationTransformNum = pGameData->GetValueF(eGDS_MC, m_index, "AnimationTransformNum");
    m_animationTransformInterval = pGameData->GetValueF(eGDS_MC, m_index, "AnimationTransformInterval");
    m_animationStandNum = pGameData->GetValueF(eGDS_MC, m_index, "AnimationStandNum");
    m_animationStandInterval = pGameData->GetValueF(eGDS_MC, m_index, "AnimationStandInterval");

    //Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, false); 
    m_moveYThreshold = VisibleRect::top().y;

    m_pMainSprite = Sprite::create();
    m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor() * 0.5f);
    addChild(m_pMainSprite, -2);

    m_pHpLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget("Label_HP"));
    m_pMpLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget("Label_Energy"));

    char str[20];
    sprintf(str,"%d",m_hp);
    m_pHpLabel->setString(str);

    sprintf(str, "%d/%d", (INT)m_mp, (INT)m_maxMp);
    m_pMpLabel->setString(str);
    
    // Play Particle
    m_fireParticle = ParticleSystemQuad::create("Fire.plist");
    m_fireParticle->setPositionType(ParticleSystem::PositionType::RELATIVE);
    m_fireParticleBatch = ParticleBatchNode::createWithTexture(m_fireParticle->getTexture());
    m_fireParticleBatch->addChild(m_fireParticle);
    m_fireParticleBatch->setPositionX(-50);
    m_fireParticleBatch->setScaleY(2);
    addChild(m_fireParticleBatch, 10);

    INIT_FSM(Fly);
}

void MainCharacter::onExit()
{
    GameObject::onExit();
}

void MainCharacter::Killed()
{
    GetFsm().SwitchState(STATE(Killed));
}

void MainCharacter::StateUpdate(float deltaTime)
{
    m_deltaTime = deltaTime;
    GetFsm().Update();
    CheckInvisible();
}

ESkill MainCharacter::GetMcSkillType()
{
    ESkill result = eS_WildOx;
    switch(m_index)
    {
    case 0:
        result = eS_WildOx;
        break;
    case 1:
        result = eS_Laser;
        break;
    case 2:
        result = eS_Replication;
        break;
    }
    return result;
}

void MainCharacter::SetCurSkill( ESkill skill )
{
    switch (m_curSkill)
    {
    case eS_WildOx:
        {
            float times = GameData::Singleton().GetValueF(eGDS_Skill, eS_WildOx, "ShootIntervalTimes");
            m_shootInterval /= times;
        }
        break;
    case eS_Replication:
        {
            for (unsigned i = 0; i < m_replicationVec.size(); ++i)
            {
                m_replicationVec[i]->Unspawn();
            }
            m_replicationVec.clear();
        }
        break;
    default:
        break;
    }

    m_curSkill = skill;

    switch (m_curSkill)
    {
    case eS_Laser:
    case eS_WildOx:
    case eS_Replication:
        GetFsm().SwitchState(STATE(Transform));
        break;
    case eS_Null:
        GetFsm().SwitchState(STATE(Fly));
        break;
    default:
        break;
    }
}

ESkill MainCharacter::GetCurSkill() const
{
    return m_curSkill;
}

void MainCharacter::AccelerateSpeedX()
{
    m_speedX *= m_accelerateSpeedXAfterDash;
}

float MainCharacter::GetSpeedX()
{
    return m_speedX;
}

float MainCharacter::GetSpeedY()
{
    return m_speedY;
}

void MainCharacter::GasBottonPushDown()
{
    m_isGasOn = true;
}

void MainCharacter::GasBottonRelease()
{
    m_isGasOn = false;
}

void MainCharacter::GasBottonCancel()
{
    m_isGasOn = false;
}

void MainCharacter::Shoot()
{
    if (m_curSkill != eS_Laser)
    {
        EBulletType bulletType = eBT_TitanNormal;
        switch(m_index)
        {
        case 0:
            bulletType = eBT_TitanNormal;
            break;
        case 1:
            bulletType = eBT_StormNormal;
            break;
        case 2:
            bulletType = eBT_CharmNormal;
            break;
        default:
            break;
        }
        
        Bullet* pBullet = GameObjectFactory::Singleton().CreateBullet(getParent(), bulletType, eFT_Good);
        pBullet->setPosition(getPosition() + Vec2(75,0));
    }
    else
    {
        Laser* pBullet = new Laser(eBT_Laser, eFT_Good);
        pBullet->autorelease();
        //pBullet->SetAttackPoint(GameData::Singleton().m_extraAttackPoint);
        getParent()->addChild(pBullet);
    }
    
}

void MainCharacter::CheckInvisible()
{
    if (m_pMainSprite && m_isInvisible)
    {
        m_invisibleTimer -= m_deltaTime;
        if (m_invisibleTimer > 0.0f)
        {
            static float time = 0.0f;
            time -= m_deltaTime;
            if (time < 0.0f)
            {
                m_pMainSprite->setVisible(!m_pMainSprite->isVisible());
                time = 0.1f;
            }
        }
        else
        {
            m_invisibleTimer = m_invisibleTime;
            m_isInvisible = false;
            m_pMainSprite->setVisible(true);
        }

    }
}

void MainCharacter::PlayAnimation(EMCAnimationState state, bool repeat, bool reverse, const std::function<void()>& func)
{
    unsigned animationNum = 0;
    float animationInterval = 0.0f;
    switch (state)
    {
    case eMCAS_FlyUp:
        {
            animationNum = m_animationFlyUpNum;
            animationInterval = m_animationFlyUpInterval;
        }
        break;
    case eMCAS_FlyUpLoop:
        {
            animationNum = m_animationFlyUpNum*0.5f;
            animationInterval = m_animationFlyUpInterval;
        }
        break;
    case eMCAS_FlyDown:
        {
            animationNum = m_animationFlyDownNum;
            animationInterval = m_animationFlyDownInterval;
        }
        break;
    case eMCAS_FlyDownLoop:
        {
            animationNum = m_animationFlyDownNum*0.5f;
            animationInterval = m_animationFlyDownInterval;
        }
        break;
    case eMCAS_Transform:
        {
            animationNum = m_animationTransformNum;
            animationInterval = m_animationTransformInterval;
        }
        break;
    case eMCAS_Stand:
        {
            animationNum = m_animationStandNum;
            animationInterval = m_animationStandInterval;
        }
        break;
    default:
        XAssert(false, "Unexpect animation state.");
    }

    Animation* pAnim = Animation::create();
    char str[20];
    for (unsigned i = 0; i < animationNum; ++i)
    {
        if (state == eMCAS_FlyUp || state == eMCAS_FlyUpLoop)
        {
            sprintf(str,"MC_Up%d.png", (state == eMCAS_FlyUp ? i : (i+animationNum)));
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            pAnim->addSpriteFrame(frame);
        }
        else if(state == eMCAS_FlyDown || state == eMCAS_FlyDownLoop)
        {
            sprintf(str,"MC_Down%d.png", (state == eMCAS_FlyDown ? i : (i+animationNum)));
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            pAnim->addSpriteFrame(frame);
        }
        else
        {
            sprintf(str,"MC_%d.png", i);
            SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            pAnim->addSpriteFrame(frame);
        }
    }

    pAnim->setDelayPerUnit(animationInterval);
    pAnim->setRestoreOriginalFrame(false);
    
    if (m_animationAction)
    {
        m_pMainSprite->stopAction(m_animationAction);
        m_animationAction->release();
        m_animationAction = NULL;
    }

    ActionInterval* pCCAnimate = Animate::create(pAnim);

    if (repeat)
    {    
        m_animationAction = RepeatForever::create(pCCAnimate);
    }
    else
    {
        if (func)
            m_animationAction = Sequence::create(reverse ? pCCAnimate->reverse() : pCCAnimate, CallFunc::create(func), NULL);
        else
            m_animationAction = reverse ? pCCAnimate->reverse() : pCCAnimate;
    }
    m_animationAction->retain();
    m_pMainSprite->runAction(m_animationAction);
}

void MainCharacter::ReduceHp( unsigned hp )
{
    if (!m_isLaunchingSkill && !m_isInvisible)
    {
        GameObject::ReduceHp(hp);

        if (m_hp > 0)
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_McHurt.mp3");
            SoundManager::Singleton().vibrate();
            
            ParticleSystem* crashParticle = ParticleSystemQuad::create("damage.plist");
            ParticleBatchNode* crashParticleBatch = ParticleBatchNode::createWithTexture(crashParticle->getTexture());
            crashParticleBatch->addChild(crashParticle);
            addChild(crashParticleBatch, 10);
        }
        
        char str[20];
        sprintf(str,"%d",m_hp);
        m_pHpLabel->setString(str);
        m_isInvisible = true;

        if (m_hp <= 3)
            BackgroundLogic::Singleton().StartHurtEffect(3);
    }
}

bool MainCharacter::ReduceMp( int reduceEnergy )
{
    if (m_mp != 0)
    {
        m_mp -= reduceEnergy;
        clampf(m_mp, 0, m_maxMp);

        char str[20];
        sprintf(str, "%d/%d", (INT)m_mp, (INT)m_maxMp);
        m_pMpLabel->setString(str);

        return true;
    }

    return false;
}

bool MainCharacter::IncreaseMp( int increaseEnergy )
{
    if (m_mp == m_maxMp)
        return false;

    m_mp += increaseEnergy;
    clampf(m_mp, 0, m_maxMp);

    char str[20];
    sprintf(str, "%d/%d", (INT)m_mp, (INT)m_maxMp);
    m_pMpLabel->setString(str);

    return true;   
}

bool MainCharacter::IsMpFull()
{
    XAssert(m_mp <= m_maxMp, "current Mp must less than maxMp.");

    return m_mp >= m_maxMp;
}

INT MainCharacter::GetMp() const
{
    return m_mp;
}

INT MainCharacter::GetSkillMpCost() const
{
    return m_skillMpCost;
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE_BEGIN(MainCharacter, Idle)
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

void MainCharacter::FlyUpdate()
{
    if (getPositionX() != m_initPositionX)
    {
        m_dragBackSpeed = 20.0f;

        setPositionX(getPositionX() - (getPositionX() > m_initPositionX ? 1 : -1) * m_dragBackSpeed * m_deltaTime);
    }

    float m_lastSpeedY = m_speedY;

    if (m_isGasOn)
    {
        if (m_speedY < 0.0f)
            m_speedY = 0.0f;

        m_speedY += m_gasSpeedY * m_deltaTime;
    }

    m_speedY -= m_gravityAccelarte * m_deltaTime;

    setPositionY(getPositionY() + m_speedY * m_deltaTime);

    if (getPositionY() < VisibleRect::bottom().y + 40)
    {
        setPositionY(VisibleRect::bottom().y + 40);
        m_speedY = 0.0f;
    }
    else if(getPositionY() > m_moveYThreshold)
    {
        setPositionY(m_moveYThreshold);
        m_speedY = 0.0f;
    }

    if (!m_playingTransformAnimation)
    {
        if (m_lastSpeedY <= 0.0f && m_speedY > 0.0f)
        {
            
            PlayAnimation(eMCAS_FlyUp, false, false, std::bind(&MainCharacter::FlyUpAnimationCallback, this));
        }
        else if(m_lastSpeedY >= 0.0f && m_speedY < 0.0f)
        {
            PlayAnimation(eMCAS_FlyDown, false, false, std::bind(&MainCharacter::FlyDownAnimationCallback, this));
        }
    }

    if (m_isShooting)
    {
        if (m_curSkill != eS_Laser)
        {
            m_shootTimer -= m_deltaTime;
            if (m_shootTimer < 0)
            {
                Shoot();
                m_bulletCount++;
                if (m_bulletCount >= 3)
                {
                    m_bulletCount = 0;
                    m_shootTimer = m_shootInterval * 1.0f;
                }
                else
                    m_shootTimer = m_shootInterval * 0.3f;
            }
        }
    }
}

void MainCharacter::FlyUpAnimationCallback()
{
    if (m_speedY > 0.0f)
    {
        std::function<void()> nullFun;
        PlayAnimation(eMCAS_FlyUpLoop, true, false, nullFun);
    }
}

void MainCharacter::FlyDownAnimationCallback()
{
    if (m_speedY <= 0.0f)
    {
        std::function<void()> nullFun;
        PlayAnimation(eMCAS_FlyDownLoop, true, false, nullFun);
    }
}

void MainCharacter::PreFlyAnimationCallback()
{
    m_playingTransformAnimation = false;

    if (m_speedY > 0.0f)
    {
        PlayAnimation(eMCAS_FlyUp, false, false, std::bind(&MainCharacter::FlyUpAnimationCallback, this));
    }
    else
    {
        PlayAnimation(eMCAS_FlyDown, false, false, std::bind(&MainCharacter::FlyDownAnimationCallback, this));
    }
}

void MainCharacter::FinishTransfromAnimationCallback()
{
    Node* pNode = Director::getInstance()->getRunningScene()->getChildByTag(1984);
    if (pNode)
    {
        GameLayer* pGameLayer = (GameLayer*)pNode;
        pGameLayer->Resume(true);
    }
    GameObjectManager::Singleton().UnregisterPauseUpdateGameObjectTypeAll();

    // Apply skill effect
    switch (m_curSkill)
    {
    case eS_Laser:
        {
            Shoot();
        }
        break;
    case eS_WildOx:
        {
            float times = GameData::Singleton().GetValueF(eGDS_Skill, eS_WildOx, "ShootIntervalTimes");
            m_shootInterval *= times;
        }
        break;
    case eS_Replication:
        {
            Replication* pReplication = new Replication();
            pReplication->autorelease();
            pReplication->setPositionY(100);
            addChild(pReplication);
            m_replicationVec.push_back(pReplication);

            pReplication = new Replication();
            pReplication->autorelease();
            pReplication->setPositionY(-100);
            addChild(pReplication);
            m_replicationVec.push_back(pReplication);
        }
        break;
        default:break;
    }
}

void MainCharacter::ApplyBonus()
{
    GameData* pGameData = GameData::SingletonPtr();
    std::vector<int>& bonusIndexVec = pGameData->m_bonusIndexVec;
    for (int i = 0; i < (int)bonusIndexVec.size(); ++i)
    {
        int index = bonusIndexVec[i];
        float value = pGameData->GetValueF(eGDS_Bonus, index, "Value1");
        if (index == 0) // Random
        {
            int count = pGameData->GetSize(eGDS_Bonus);
            index = RandomFloat(1, count);
        }

        switch(index)
        {
        case 1: // Dash1
        case 2: // Dash2
            {
                m_invisibleTimer = value;
                m_isInvisible = true;
            }
            break;
        case 3: //SkillPoint1
        case 4: //SkillPoint2
            {
                m_mp = value;
            }
            break;
        case 5: //ScoreBonus
            m_bonusKillingScore = value;
            break;
        case 6: //TopLevel
            {
                m_hp = 100;
                pGameData->m_mcExtraAttackPointVec[pGameData->m_mcIndex] = 100;
            }
            break;


        }
    }
}

IMPLEMENT_STATE_BEGIN(MainCharacter, Fly)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        if (m_enterScene)// play only at start, skip if it's switched from Idle(add oil) for example.
        {
            m_enterScene = false;

            SoundManager::Singleton().PlayEffect("Sound/Effect_TransformBack.WAV");

            PlayAnimation(eMCAS_Transform, false, true, std::bind(&MainCharacter::PreFlyAnimationCallback, this));
            m_playingTransformAnimation = true;
        }

        m_speedY = 0.0f;
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        FlyUpdate();
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(MainCharacter, Transform)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        SoundManager::Singleton().PlayEffect("Sound/Effect_Transform.WAV");
        
        PlayAnimation(eMCAS_Transform, false, false, std::bind(&MainCharacter::FinishTransfromAnimationCallback, this));
        m_playingTransformAnimation = true;
        m_speedY = 0.0f;

        Node* pNode = Director::getInstance()->getRunningScene()->getChildByTag(1984);
        if (pNode)
        {
            m_pMainSprite->setVisible(true);
            GameLayer* pGameLayer = (GameLayer*)pNode;
            pGameLayer->Pause(false, m_animationAction->getTarget());
        }
        GameObjectManager::Singleton().RegisterPauseUpdateGameObjectTypeAll();
        GameObjectManager::Singleton().UnregisterPauseUpdateGameObjectType(eGOT_MainCharacter);

    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        if (m_curSkill == eS_WildOx)
            m_isInvisible = true;
        
        FlyUpdate();
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
        SoundManager::Singleton().PlayEffect("Sound/Effect_TransformBack.WAV");

        PlayAnimation(eMCAS_Transform, false, true, std::bind(&MainCharacter::FinishTransfromAnimationCallback, this));
        m_playingTransformAnimation = false;
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(MainCharacter, Killed)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_killTimer = 2.3f;

        SoundManager::Singleton().PlayEffect("Sound/Effect_McDie.mp3");
        
        m_fireParticle->stopSystem();
        
        ParticleSystem* crashParticle = ParticleSystemQuad::create("MCCrash.plist");
        crashParticle->retain();
        ParticleBatchNode* crashParticleBatch = ParticleBatchNode::createWithTexture(crashParticle->getTexture());
        crashParticleBatch->addChild(crashParticle);
        addChild(crashParticleBatch, 10);

        removeChild(m_pMainSprite);
        m_pMainSprite = NULL;
    }
    STATE_CONSTRUCTOR_END

    STATE_UPDATE_BEGIN
    {      
        m_killTimer -= m_deltaTime;
        if (m_killTimer < 0)
            SceneManager::CreateScene(MainMenu_GameOver);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

void MainCharacter::TransactToIdle()
{
    GetFsm().SwitchState(STATE(Idle));
}

void MainCharacter::TransactToFly()
{
    GetFsm().SwitchState(STATE(Fly));
}

void MainCharacter::SetIsLaunchingSkill( bool enable )
{
    m_isLaunchingSkill = enable;
}

float MainCharacter::GetShootInterval()
{
    return m_shootInterval;
}

float MainCharacter::GetBonusKillingScore()
{
    return m_bonusKillingScore;
}


