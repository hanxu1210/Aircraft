#include "CommonHeaders.h"

#include "SceneCredits.h"
#include "Ui/UiManager.h"
#include "Scene/SceneManager.h"

//------------------------------------------------------------------
//
// CreditsLayer
//
//------------------------------------------------------------------
CreditsLayer::CreditsLayer(void)
    : m_timer(2.0f)
{
}

CreditsLayer::~CreditsLayer(void)
{
}

std::string CreditsLayer::title()
{
    return "CreditsScene";
}

void CreditsLayer::onEnter()
{
    Layer::onEnter();

    m_pUiRoot = UiManager::Singleton().SetupWidget(this, "UIProject/Json/Credits.json");

    EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CreditsLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(CreditsLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    Layout* pPanel = DynamicCast<Layout*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Panel_Root"));
    pPanel->setOpacity(0);
     pPanel->runAction(FadeTo::create(3, 255));
}

void CreditsLayer::Update( float dt )
{

}

bool CreditsLayer::onTouchBegan(Touch *pTouche, Event *pEvent)
{
    return true;
}

void CreditsLayer::onTouchEnded( Touch* /*pTouche*/, Event* /*pEvent*/ )
{
    SceneManager::CreateScene(MainMenu_Home);
}

//------------------------------------------------------------------
//
// SceneCredits
//
//------------------------------------------------------------------
void SceneCredits::runThisTest()
{
    Layer* pLayer = CreditsLayer::create();
    addChild(pLayer);

    Director::getInstance()->replaceScene(this);
}
