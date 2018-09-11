#include "CommonHeaders.h"

#include "GameObject/MagicMatrix/MagicCircle.h"
#include "GameObject/MagicMatrix/MagicPoint.h"

static const char* MagicCircleRes[] =
{
    "pattern1.png",
    "pattern2.png"
};

MagicCircle::MagicCircle()
    : GameObject(0.0f, eGOT_MagicMatrix,10.0f)
    , m_associateGType(eGT_Invalid)
    , m_deltaTime(0.0f)
    , m_elapsedTime(0.0f)
    , m_durationTime(0.0f)
    , m_pMainSprite(NULL)
    , m_pStreak(NULL)
    , m_pParticle(NULL)
    , m_particlePointNum(0)
{
}

MagicCircle::MagicCircle( const TPointVector& magicPointVector, const TPointVector& magicPointLogicVector, float durationTime, EGeneralType type )
    : GameObject(0.0f, eGOT_MagicMatrix,10.0f)
    , m_associateGType(type)
    , m_magicPointVector(magicPointVector)
    , m_magicPointLogicVector(magicPointLogicVector)
    , m_deltaTime(0.0f)
    , m_elapsedTime(0.0f)
    , m_durationTime(durationTime)
    , m_pMainSprite(NULL)
    , m_pStreak(NULL)
    , m_particlePointNum(0)
{
}

MagicCircle::~MagicCircle()
{
    m_magicPointVector.clear();
}

void MagicCircle::onEnter()
{
    GameObject::onEnter();

    //////////////////////////////////////////////////////////////////////////
    //Test particle
    m_pParticle = ParticleGalaxy::create();
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("stars.png");
    m_pParticle->setTexture(pTexture);
    m_pParticle->setLife(0.5f);
    m_pParticle->setScale(1.0f);
    m_pParticle->setPosition(VisibleRect::center());

    getParent()->addChild(m_pParticle, 30);
    //addChild(m_pParticle, 10);
    //////////////////////////////////////////////////////////////////////////

    EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MagicCircle::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MagicCircle::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MagicCircle::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    AddGraphics();

    INIT_FSM(Idle);
}

void MagicCircle::onExit()
{
    GameObject::onExit();
}

void MagicCircle::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;
    m_elapsedTime += deltaTime;
    GetFsm().Update();
}

bool MagicCircle::onTouchBegan( Touch *pTouch, Event *pEvent )
{
    if (m_pStreak)
        m_pStreak->removeFromParentAndCleanup(true);

    m_pStreak = MotionStreak::create(1.0f, 3.0f, 10.0f, Color3B::WHITE, "streak.png");
    getParent()->addChild(m_pStreak);

    return true;
}

void MagicCircle::onTouchMoved( Touch *pTouche, Event *pEvent )
{
    m_pStreak->setPosition(pTouche->getLocation());
}

void MagicCircle::onTouchEnded( Touch* /*pTouche*/, Event* /*pEvent*/ )
{
    if (GetFsm().IsCurrentState(STATE(Operating)) && m_elapsedTime <= 2.0f && getChildrenCount() > 0)
    {
        GetFsm().SwitchState(STATE(Failed));
    }
}

bool MagicCircle::IsSucceed()
{
    return GetFsm().IsCurrentState(STATE(Succeed));
}

bool MagicCircle::IsFailed()
{
    return GetFsm().IsCurrentState(STATE(Failed));
}

void MagicCircle::AddGraphics()
{
    m_pMainSprite = Sprite::createWithSpriteFrameName(MagicCircleRes[m_associateGType]);
    m_pMainSprite->setScale(2.0 * Director::getInstance()->getContentScaleFactor());
    addChild(m_pMainSprite);
}

void MagicCircle::MoveParticle()
{
    if (m_pParticle)
    {
        Vec2 direction = (m_magicPointVector[m_particlePointNum] - m_pParticle->getPosition()).getNormalized();
        Vec2 newPos =  m_pParticle->getPosition() + direction * 800.0f * m_deltaTime;
        m_pParticle->setPosition(newPos);

        if ((newPos - m_magicPointVector[m_particlePointNum]).getLengthSq() < 12.0f)
        {
            if (m_particlePointNum < m_magicPointVector.size() - 1)
            {
                ++m_particlePointNum;
            }
            else
            {
                m_pParticle->removeFromParentAndCleanup(true);
                m_pParticle = NULL;
            }
        }
    }
}

IMPLEMENT_STATE_BEGIN(MagicCircle, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        SWITCH_TO_STATE(Operating);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END


    IMPLEMENT_STATE_BEGIN(MagicCircle, Operating)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        for (unsigned i = 0; i < m_magicPointLogicVector.size(); ++i)
        {
            MagicPoint* pPoint = new MagicPoint();
            pPoint->autorelease();
            pPoint->setPosition(m_magicPointLogicVector[i] - VisibleRect::center());
            addChild(pPoint);            
        }

        m_elapsedTime = 0.0f;

        m_pParticle->setPosition(m_magicPointVector[m_particlePointNum]);
        ++m_particlePointNum;
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        MoveParticle();

        if (m_elapsedTime > 2.0f)
        {
            SWITCH_TO_STATE(Failed);
        }
        else if (getChildrenCount() == 1) // Hack: sprite left.
        {
            SWITCH_TO_STATE(Succeed);
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END


    IMPLEMENT_STATE_BEGIN(MagicCircle, Succeed)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        SoundManager::Singleton().PlayEffect("Sound/Effect_Skill.mp3");
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        SWITCH_TO_STATE(Dead);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END


    IMPLEMENT_STATE_BEGIN(MagicCircle, Failed)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        SWITCH_TO_STATE(Dead);
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    IMPLEMENT_STATE_BEGIN(MagicCircle, Dead)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        if (m_pParticle)
        {
            m_pParticle->removeFromParentAndCleanup(true);
            m_pParticle = NULL;
        }

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