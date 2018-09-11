#include "CommonHeaders.h"

#include "Gamelogic/MainPlayerLogic.h"
#include "Ui/UiManager.h"
#include "GameObject/MainCharacter.h"
#include "GameData.h"

static const int g_mainPlayerTotalHp = 10;
static const int g_originalCoinNum = 0;

IMPLEMENT_SINGLETON(MainPlayerLogic);

MainPlayerLogic::MainPlayerLogic()
    : m_pHpBar(NULL)
    , m_pCoinNumLableAtlas(NULL)
    , m_originalHp(0)
    , m_currentHp(0)
    , m_originalCoin(0)
    , m_currentCoin(0)
    , m_parent(NULL)
    , m_pMC(NULL)
{
}

MainPlayerLogic::~MainPlayerLogic()
{
}

void MainPlayerLogic::Init(Layer* layer)
{
    m_parent = layer;

    m_originalHp = g_mainPlayerTotalHp;
    m_currentHp = g_mainPlayerTotalHp;
    m_originalCoin = g_originalCoinNum;
    m_currentCoin = g_originalCoinNum;

    m_pHpBar = DynamicCast<LoadingBar*>(UiManager::Singleton().GetWidget("HpBar"));
    m_pCoinNumLableAtlas = DynamicCast<TextAtlas*>(UiManager::Singleton().GetWidget("Gold"));
    m_pScoreLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget("Label_Score"));
    m_pScoreLabel->setString("0");

    // TODO. Temporary create it here.
    m_pMC = new MainCharacter(GameData::Singleton().m_mcIndex);
    m_pMC->autorelease();
    m_parent->addChild(m_pMC, 3);
}

void MainPlayerLogic::Uninit()
{
    // TODO. Temporary delete it here.
    m_parent->removeChild(m_pMC);

    Reset();
    m_pHpBar = NULL;
}

void MainPlayerLogic::Reset()
{
    m_currentHp = g_mainPlayerTotalHp;
}

void MainPlayerLogic::ReduceCoin( int reduceCoin )
{
    if (m_currentCoin > (unsigned)0)
    {
        m_currentCoin -= reduceCoin;

        DrawCoinValue();
    }
}

void MainPlayerLogic::IncreaseCoin( int increaseCoin )
{
    m_currentCoin += increaseCoin;

    DrawCoinValue();
}

void MainPlayerLogic::ReduceScore(int reduceScore)
{
    GameData::Singleton().ReduceScore(reduceScore);
    
    DrawScoreValue(0);
}
void MainPlayerLogic::IncreaseScore(int increaseScore)
{
    GameData::Singleton().IncreaseScore(increaseScore);
    
    int type = increaseScore >= 1000 ? 3 : (increaseScore >= 300 ? 2 : (increaseScore > 1 ? 1 : 0));
    DrawScoreValue(type);
}

void MainPlayerLogic::DrawCoinValue()
{

}

void MainPlayerLogic::DrawScoreValue(int animationType)
{
    char stringValue[20];
    sprintf(stringValue, "%d", GameData::Singleton().m_currentScore);

    m_pScoreLabel->setString(stringValue);

    Action* pAction = NULL;

    switch (animationType)
    {
    case 1:
        {
            Sequence* scaleAction = Sequence::create( CCScaleTo::create(0.1, 1.5f), CCScaleTo::create(0.1f, 1.0f), NULL);
            Sequence* tintAction = Sequence::create( CCTintTo::create(0.1, 255, 0, 0), CCTintTo::create(0.1f, 255, 255, 255), NULL);
            pAction = CCSpawn::create(scaleAction, tintAction, NULL);
        }
        break;
    case 2:
        {
            Sequence* scaleAction = Sequence::create( CCScaleTo::create(0.12f, 2.0f), CCScaleTo::create(0.12f, 1.0f), NULL);
            Sequence* tintAction = Sequence::create( CCTintTo::create(0.12f, 255, 0, 0), CCTintTo::create(0.12f, 255, 255, 255), NULL);
            pAction = CCSpawn::create(scaleAction, tintAction, NULL);
        }
        break;
    case 3:
        {
            Sequence* scaleAction = Sequence::create( CCScaleTo::create(0.2, 2.5f), CCScaleTo::create(0.2f, 1.0f), NULL);
            Sequence* tintAction = Sequence::create( CCTintTo::create(0.2, 255, 0, 0), CCTintTo::create(0.2f, 255, 255, 255), NULL);
            pAction = CCSpawn::create(scaleAction, tintAction, NULL);
        }
        break;
    default:
        break;
    }
    if (pAction)
        m_pScoreLabel->runAction(pAction);
}

MainCharacter* MainPlayerLogic::GetMC()
{
    return m_pMC;
}
