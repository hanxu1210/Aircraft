#include "CommonHeaders.h"

#include "GameObject/Items/Gold/Gold.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameData.h"
#include "GameObject/MainCharacter.h"

Gold::Gold(EGoldType type)
    : m_type(type)
{
}

Gold::~Gold()
{

}

void Gold::onEnter()
{
    Item::onEnter();

    m_pMainSprite = Sprite::createWithSpriteFrameName(m_type == eGT_Small ? "Coin1.png" : "Coin2.png");
    m_pMainSprite->setScale(Director::getInstance()->getContentScaleFactor());
    addChild(m_pMainSprite);
}

void Gold::onExit()
{
    Item::onExit();
}

int Gold::GetDetailTypeIndex()
{
    return static_cast<int>(eIT_Gold);
}

IMPLEMENT_STATE_BEGIN(Gold, Move)
{
    STATE_CONSTRUCTOR_BEGIN
    {
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        MainCharacter* pMc = MainPlayerLogic::Singleton().GetMC();
        if (pMc)
        {
            Vec2 mcPos = pMc->GetPos();
            Vec2 offset = mcPos - GetPos();
            if (offset.getLengthSq() < 150.0f*150.0f)
            {
                float speed = (pMc->GetSpeedX() + pMc->GetSpeedY()) * 2.0f;
                offset.normalize();
                setPosition(getPosition() + offset * speed * m_deltaTime);
            }

            bool isHit = false;
            if (GameObject* gameObject = CheckCollisionByGameObjectType(eGOT_MainCharacter))
            {
                if (gameObject)
                    isHit = true;
            }
            TRANSIT_TO_STATE( isHit, NoTransitionAction, Hit );
        }
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(Gold, Hit)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        SoundManager::Singleton().PlayEffect("Sound/Effect_Item.mp3");

        MainCharacter* pMC = MainPlayerLogic::Singleton().GetMC();
        if (!pMC->IncreaseMp(m_type == eGT_Small ? 1 : 3))
            MainPlayerLogic::Singleton().IncreaseScore(100);
        
        ParticleSystem* crashParticle = ParticleSystemQuad::create("getMagic.plist");
        ParticleBatchNode* crashParticleBatch = ParticleBatchNode::createWithTexture(crashParticle->getTexture());
        crashParticleBatch->addChild(crashParticle);
        pMC->addChild(crashParticleBatch, 10);

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