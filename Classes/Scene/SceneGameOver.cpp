#include "CommonHeaders.h"

#include "Scene/SceneGameOver.h"

#include "Ui/UiManager.h"
#include "GameObject/GameObjectManager.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "Scene/SceneManager.h"
#include "GameData.h"

#include "GameLogic/AdsLogic/AdsLogic.h"

GameOverLayer::GameOverLayer(void)
    : m_leaderboardScrollView(nullptr)
{
}

GameOverLayer::~GameOverLayer(void)
{
}

void GameOverLayer::onEnter()
{
    Layer::onEnter();
    
    NsAircraft::AdsLogic::Singleton().PopUpPopUpAdds();

    SoundManager::Singleton().PlayEffect("Sound/Effect_GameOver.WAV");
    
    // UI setup
    m_pUiRoot = UiManager::Singleton().SetupWidget(this, "UIProject/Json/GameOver.json");

    Text* label = DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Label_FinalScore"));
    char stringValue[20];
    sprintf(stringValue, "%d", GameData::Singleton().m_currentScore);
    label->setString(stringValue);

    m_playAgainButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "PlayAgainButton"));
    m_playAgainButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::OnTouchPlayAgainButton, this));

    m_leaderboardScrollView = DynamicCast<ui::ScrollView*>(UiManager::Singleton().GetWidget(m_pUiRoot, "ScrollView_Leaderboard"));
    
    for (int i = 0; i < 20; ++i)
    {
        sprintf(stringValue, "Panel_%d", i);
        Layout* panel = DynamicCast<Layout*>(UiManager::Singleton().GetWidget(m_pUiRoot, stringValue));
        m_panelVec.push_back(panel); 
    }
    setLeaderboard();
}

void GameOverLayer::OnTouchPlayAgainButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SceneManager::CreateScene(MainMenu_Upgrade);
        }
        break;
        default:break;
    }
}

void GameOverLayer::setLeaderboard()
{
    for (int i = 0; i < m_panelVec.size(); ++i)
    {
        Text* pLabelRank = static_cast<Text*>(m_panelVec[i]->getChildByTag(1));
        char stringValue[20];
        sprintf(stringValue, "%d", i+1);
        pLabelRank->setString(stringValue);

        Text* pLabelName = static_cast<Text*>(m_panelVec[i]->getChildByTag(2));
        sprintf(stringValue, "Name%d", i+1);
        pLabelName->setString(stringValue);

        Text* pLabelScore = static_cast<Text*>(m_panelVec[i]->getChildByTag(3));
        sprintf(stringValue, "%d", 999);
        pLabelScore->setString(stringValue);
    }
}
//------------------------------------------------------------------
//
// SceneGameOver
//
//------------------------------------------------------------------
void SceneGameOver::runThisTest()
{
    GameOverLayer* pLayer = GameOverLayer::create();
    addChild(pLayer, 0);
    Director::getInstance()->replaceScene(this);
}