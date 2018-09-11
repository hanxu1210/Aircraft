#include "CommonHeaders.h"

#include "SceneGame.h"

#include "GameObject/GameObjectManager.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "GameLogic/BackgroundLogic.h"
#include "GameLogic/GeneralGroupLogic.h"
#include "Scene/SceneManager.h"
#include "GameData.h"
#include "GameObject/GameObjectFactory.h"

#include "GameObject/MainCharacter.h"

//------------------------------------------------------------------
//
// GameLayer
//
//------------------------------------------------------------------

GameLayer::GameLayer(void)
    : m_updated(false)
    , m_tipTimer(1.5f)
    , m_pGeneralGroupLogic(NULL)
    , m_resumeTimer(3.0f)
    , m_oneSecondTimer(0.0f)
    , m_countDownValue(0)
    , m_isResuming(false)
    , m_isPuased(false)
    , m_pActionSet(NULL)
{
   
}

GameLayer::~GameLayer(void)
{
}

void GameLayer::Init()
{
    SoundManager::Singleton().playBackgroundMusic("Sound/BGM_Game.mp3", true);

    GameObjectFactory::CreateSingleton();
    GameObjectFactory::Singleton().Init(this);
    GameObjectManager::CreateSingleton();

    MainPlayerLogic::CreateSingleton();
    MainPlayerLogic::Singleton().Init(this);

    BackgroundLogic::CreateSingleton();
    BackgroundLogic::Singleton().Init(this);

    // Init general logic.
    std::vector< EGeneralType > generalVec;
    generalVec.push_back(eGT_OptimusPrime);
    generalVec.push_back(eGT_Hornet);
    m_pGeneralGroupLogic = new GeneralGroupLogic(generalVec);
    m_pGeneralGroupLogic->autorelease();
    addChild(m_pGeneralGroupLogic, 1);

    m_inGameMenuPanel = DynamicCast<Layout*>(UiManager::Singleton().GetWidget(m_pUiRoot, "InGameMenuPanel"));
    m_inGameMenuPanel->setVisible(false);

    m_pauseButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "PauseButton"));
    m_pauseButton->addTouchEventListener(CC_CALLBACK_2(GameLayer::OnTouchPauseButton, this));

    m_resumeButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "ResumeButton"));
    m_resumeButton->addTouchEventListener(CC_CALLBACK_2(GameLayer::OnTouchResumeButton, this));

    m_mainMenuButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "MainMenuButton"));
    m_mainMenuButton->addTouchEventListener(CC_CALLBACK_2(GameLayer::OnTouchMainMenuButton, this));

    m_countDownLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "CountDownLabel"));
    m_countDownLabel->setString("3");
    m_countDownLabel->setVisible(false);

    EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    SoundManager::Singleton().preloadEffect("Sound/Effect_RefuelDash.mp3");
    SoundManager::Singleton().preloadEffect("Sound/Effect_BulletExploded.WAV");
    SoundManager::Singleton().preloadEffect("Sound/Effect_MonsterDie.mp3");
    SoundManager::Singleton().preloadEffect("Sound/Effect_Refuel.mp3");
    SoundManager::Singleton().preloadEffect("Sound/Effect_Item.mp3");
    SoundManager::Singleton().preloadEffect("Sound/Effect_McHurt.mp3");
    SoundManager::Singleton().preloadEffect("Sound/Effect_TransformBack.WAV");
    SoundManager::Singleton().preloadEffect("Sound/Effect_Transform.WAV");
    SoundManager::Singleton().preloadEffect("Sound/Effect_McDie.mp3");
    SoundManager::Singleton().preloadEffect("Sound/Effect_MonsterDie.mp3");
}

void GameLayer::onEnter()
{
    Layer::onEnter();

    // UI setup
    m_pUiRoot = UiManager::Singleton().SetupWidget(this, "UIProject/Json/Game.json");
    Director::getInstance()->getScheduler()->schedule(schedule_selector(GameLayer::Update), this, 0, false);
}

void GameLayer::onExit()
{
    SoundManager::Singleton().unloadAllEffect();
    SoundManager::Singleton().stopBackgroundMusic();
    
    MainPlayerLogic::Singleton().Uninit();
    MainPlayerLogic::DestroySingleton();
    
    BackgroundLogic::Singleton().Uninit();
    BackgroundLogic::DestroySingleton();

    GameObjectManager::DestroySingleton();

    GameObjectFactory::DestroySingleton();

    Layer::onExit();
}

void GameLayer::Update(float dt)
{
    if (!m_updated)
    {
        Init();

        m_updated = true;
    }
    
    if (m_tipTimer > 0.0f) 
    {
        m_tipTimer -= dt;
     
        if (m_tipTimer <= 0.0f)
        {
            Widget* pWidget = UiManager::Singleton().GetWidget(m_pUiRoot, "Image_Tip");
            pWidget->setVisible(false);
        }
        return;
    }

    if (m_isResuming)
    {
        m_resumeTimer -= dt;
        m_oneSecondTimer -= dt;
        if (m_oneSecondTimer < 0.0f)
        {
            char str[4];
            sprintf(str,"%d",--m_countDownValue);
            m_countDownLabel->setString(str);
            m_oneSecondTimer = 1.0f;
        }
        if (m_resumeTimer < 0.0f)
        {
            m_resumeTimer = 3.0f;
            m_isResuming = false;
            
            Director::getInstance()->getActionManager()->resumeTargets(m_pActionSet);

            // Resume Game
            m_isPuased = false;
            m_countDownLabel->setVisible(false);
            m_resumeButton->setVisible(true);
            m_mainMenuButton->setVisible(true);
            m_inGameMenuPanel->setVisible(false);

            SoundManager::Singleton().resumeBackgroundMusic();
        }
    }

    if (!m_isPuased)
    {
        UiManager::Singleton().Update(dt);

        GameObjectManager::Singleton().Update(dt);

        BackgroundLogic::Singleton().StateUpdate(dt);

        m_pGeneralGroupLogic->Update(dt);
    }
    else if (!m_showPauseUi)
    {
        UiManager::Singleton().Update(dt);
    }
}

void GameLayer::Pause(bool showPauseUi, Ref* pExceptTarget)
{
    m_isPuased = true;
    m_showPauseUi = showPauseUi;
    if (showPauseUi)
    {
        m_inGameMenuPanel->setVisible(true);
    }

    m_pActionSet = Director::getInstance()->getActionManager()->pauseAllRunningActions();
    if (pExceptTarget)
    {
        for (Vector<Node*>::iterator iter = m_pActionSet.begin(); iter != m_pActionSet.end(); ++iter)
        {
            if (*iter == pExceptTarget)
            {
                Director::getInstance()->getActionManager()->resumeTarget(*iter);
                m_pActionSet.eraseObject(*iter);

                break;
            }
        }
    }
    
    SoundManager::Singleton().pauseBackgroundMusic();
}

void GameLayer::Resume(bool immediate)
{
    m_isResuming = true;
    m_oneSecondTimer = 1.0f;
    m_resumeTimer = immediate ? 0.0f : 3.0f;
    m_countDownValue = 3;
    if (!immediate)
    {
        SoundManager::Singleton().PlayEffect("Sound/Effect_Resume.WAV");
    }
}

void GameLayer::OnTouchPauseButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");
            Pause(true, NULL);
        }
        break;
        default:break;
    }
}

void GameLayer::OnTouchResumeButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

            Resume(false);
            m_resumeButton->setVisible(false);
            m_mainMenuButton->setVisible(false);

            m_countDownLabel->setVisible(true);
            m_countDownLabel->setString("3");
        }
        break;
        default:break;
    }
}

void GameLayer::OnTouchMainMenuButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

            SceneManager::CreateScene(MainMenu_Home);
        }
        break;
        default:break;
    }
}

bool GameLayer::onTouchBegan( Touch *pTouch, Event *pEvent )
{
    MainPlayerLogic::Singleton().GetMC()->GasBottonPushDown();

    return true;
}

void GameLayer::onTouchEnded( Touch* /*pTouche*/, Event* /*pEvent*/ )
{
    MainPlayerLogic::Singleton().GetMC()->GasBottonRelease();

}

//------------------------------------------------------------------
//
// GameScene
//
//------------------------------------------------------------------
void SceneGame::runThisTest()
{
    GameLayer* pLayer = GameLayer::create();
    pLayer->setTag(1984);
    addChild(pLayer, 0);
    Director::getInstance()->replaceScene(this);
}
