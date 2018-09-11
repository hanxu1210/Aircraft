#include "CommonHeaders.h"

#include "GameObject/GameObjectManager.h"
#include "GameObject/Generals/OptimusPrime/OptimusPrime.h"
#include "Ui/UiManager.h"

OptimusPrime::OptimusPrime()
{
}

OptimusPrime::~OptimusPrime()
{
}

void OptimusPrime::onEnter()
{
    General::onEnter();

    float height = 300;
    float weight = 500;
    m_collisionRect = Rect(-weight/2, -height/2, weight, height);

    m_type = eGT_OptimusPrime;

    m_pMainSprite = Sprite::createWithSpriteFrameName("Hero01.png");
    m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor());
    m_pMainSprite->setVisible(false);
    addChild(m_pMainSprite);

    m_pProfileImage = Sprite::createWithSpriteFrameName("HeroHead01.png");
    m_pProfileImage->setScale(Director::getInstance()->getContentScaleFactor());
    m_pProfileImage->setVisible(false);
    addChild(m_pProfileImage);

    m_magicPointVec.push_back(Vec2( -250.0f, 0.0f) + VisibleRect::center());
    m_magicPointVec.push_back(Vec2(0.0f, 0.0f) + VisibleRect::center());
    m_magicPointVec.push_back(Vec2(250.0f, 0.0f) + VisibleRect::center());
    m_magicPointVec.push_back(Vec2(0.0f, 250.0f) + VisibleRect::center());
    m_magicPointVec.push_back(Vec2(0.0f, -250.0f) + VisibleRect::center());

    m_magicPointLogicVec.push_back(Vec2( -50.0f, 0.0f) + VisibleRect::center());
    m_magicPointLogicVec.push_back(Vec2(0.0f, 0.0f) + VisibleRect::center());
    m_magicPointLogicVec.push_back(Vec2(50.0f, 0.0f) + VisibleRect::center());
    m_magicPointLogicVec.push_back(Vec2(0.0f, 50.0f) + VisibleRect::center());
    m_magicPointLogicVec.push_back(Vec2(0.0f, -50.0f) + VisibleRect::center());

    //UiManager::Singleton().AddChildWidget(m_pProfileImage);
}

void OptimusPrime::onExit()
{
    m_magicPointVec.clear();
    General::onExit();
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_STATE_BEGIN(OptimusPrime, CastSkill)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_pMainSprite->setVisible(true);

        m_elapasedTime = 0.0f;

        setPosition(VisibleRect::left());
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        Vec2 newPos = getPosition() + (Vec2(1,0) * 200.0f) * m_deltaTime;
        setPosition(newPos);

        // Attack
        if (GameObject* gameObject = CheckCollisionByGameObjectType(eGOT_Monster))
        {
            if (gameObject)
                gameObject->ReduceHp(10.0f);
        }

        TRANSIT_TO_STATE( !VisibleRect::getVisibleRectWithBuffer().containsPoint(newPos), NoTransitionAction, Dead );
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END
