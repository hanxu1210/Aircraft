#include "CommonHeaders.h"

#include "Scene/SceneUpgrade.h"

#include "Ui/UiManager.h"
#include "GameObject/GameObjectManager.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "Scene/SceneManager.h"
#include "GameData.h"

UpgradeLayer::UpgradeLayer(void)
    : m_upgradeCoinCost(100)
    , m_isUpgradeMcProperty(true)
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(UpgradeLayer::Update), this, 0, false);
}

UpgradeLayer::~UpgradeLayer(void)
{
}

void UpgradeLayer::onEnter()
{
    Layer::onEnter();
    
    m_pGameData = GameData::SingletonPtr();

    m_pGameData->m_bonusIndexVec.clear();
    
    // UI setup
    m_pUiRoot = UiManager::Singleton().SetupWidget(this, "UIProject/Json/Upgrade.json");
    
    UiManager::Singleton().GetWidget(m_pUiRoot, "MCUpgradePanel")->setVisible(m_isUpgradeMcProperty);
    UiManager::Singleton().GetWidget(m_pUiRoot, "PowerUpgradePanel")->setVisible(!m_isUpgradeMcProperty);
    
    m_addHpButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "AddHpButton"));
    m_addHpButton->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchAddHpButton, this));

    m_addAttackButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "AddAttackButton"));
    m_addAttackButton->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchAddAttackButton, this));

    m_startButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "StartButton"));
    m_startButton->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchStartButton, this));

    Button* pButtonReturn = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Button_Return"));
    pButtonReturn->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchButtonReturn, this));

    //Coin
    char stringValue[20];
    m_currentCoinButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "CurrentCoinButton"));
    unsigned coin = m_pGameData->m_currentCoin;
    sprintf(stringValue, "%d", coin);
    m_currentCoinButton->setTitleText(stringValue);
    
    if (coin < m_upgradeCoinCost)
    {
        m_addHpButton->setEnabled(false);
        m_addAttackButton->setEnabled(false);
    }

    // PageView
    for (int i = 0; i < 3; ++i)
    {
        sprintf(stringValue, "Image_Mc%d", i);
        ImageView* pImageView = DynamicCast<ImageView*>(UiManager::Singleton().GetWidget(m_pUiRoot, stringValue));
        m_imageMcVec.push_back(pImageView);
    }
    
    m_pageViewMc = DynamicCast<PageView*>(UiManager::Singleton().GetWidget(m_pUiRoot, "PageView"));
    m_pageViewMc->addEventListener(CC_CALLBACK_2(UpgradeLayer::onPageUpdated, this));
    m_pageViewMc->scrollToPage(m_pGameData->m_mcIndex, false);

    Update(0.0f);

    // Change MC Button
    m_rightChangeMcButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "RightChangeMCButton"));
    m_rightChangeMcButton->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchRightChangeButton, this));

    m_leftChangeMcButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "LeftChangeMCButton"));
    m_leftChangeMcButton->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchLeftChangeButton, this));

    UpdateMcPropertyUI();

    for (int i = 0; i < 7; ++i)
    {
        sprintf(stringValue, "PayItemButton%d", i);

        Button* pTextButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, stringValue));
        m_payItemButtonVec.push_back(pTextButton);
    
        sprintf(stringValue, "PayButton%d", i);
        pTextButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, stringValue));
        m_payItemCostButtonVec.push_back(pTextButton);

        m_payItemButtonVec[i]->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchPayItemButton, this));

        m_payItemCostButtonVec[i]->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::OnTouchPayItemButton, this));
    }
}

void UpgradeLayer::onPageUpdated(Ref* pSender, PageView::EventType type)
{
    m_pGameData->m_mcIndex = static_cast<PageView*>(pSender)->getCurPageIndex();

    UpdateMcPropertyUI();
}

void UpgradeLayer::UpdatePowerUpgradeUI()
{
    for (int i = 0; i < 7; ++i)
    {
        std::string text = m_pGameData->GetValue(eGDS_Bonus, i, "Text");
        m_payItemButtonVec[i]->setTitleText(text.c_str());

        int cost = m_pGameData->GetValueF(eGDS_Bonus, i, "Cost");
        char stringValue[10];
        sprintf(stringValue, "     %d", cost);
        m_payItemCostButtonVec[i]->setTitleText(stringValue);
    }
}

void UpgradeLayer::UpdateMcPropertyUI()
{
    UiManager::SetButtonEnabled(m_leftChangeMcButton, m_pageViewMc->getCurPageIndex() != 0);
    UiManager::SetButtonEnabled(m_rightChangeMcButton, m_pageViewMc->getCurPageIndex() < m_pageViewMc->getPages().size() - 1);

    char stringValue[20];
    //Name
    std::string name = GameData::Singleton().GetValue(eGDS_MC, m_pGameData->m_mcIndex, "Name");
    DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Label_Type"))->setString(name.c_str());

    //Attack    
    EBulletType bulletType = eBT_TitanNormal;
    switch(m_pGameData->m_mcIndex)
    {
    case 0:
        bulletType = eBT_TitanNormal;
        break;
    case 1:
        bulletType = eBT_StormNormal;
        break;
    case 2:
        bulletType = eBT_CharmNormal;
    }

    unsigned attackPoint = (unsigned)GameData::Singleton().GetValueF(eGDS_Bullet, bulletType, "AttackPoint") + m_pGameData->m_mcExtraAttackPointVec[m_pGameData->m_mcIndex];

    sprintf(stringValue, "%d", attackPoint);
    
    Text* attackLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Label_Attack"));
    attackLabel->setString(stringValue);
    attackLabel->setColor(Color3B::WHITE);
    
    //HP
    unsigned hp = (unsigned)GameData::Singleton().GetValueF(eGDS_MC, m_pGameData->m_mcIndex, "Hp") + m_pGameData->m_mcExtraHpVec[m_pGameData->m_mcIndex];
    sprintf(stringValue, "%d", hp);
    Text* hpLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Label_Hp"));
    hpLabel->setString(stringValue);
    hpLabel->setColor(Color3B::WHITE);
    //Skill
    std::string skillName = GameData::Singleton().GetValue(eGDS_MC, m_pGameData->m_mcIndex, "SkillName");
    DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Label_Skill"))->setString(skillName.c_str());
}
void UpgradeLayer::TextFieldInserted( Ref* pSender )
{
    TextField* pUITextField = DynamicCast<TextField*>(pSender);
    std::string str = pUITextField->getStringValue();
    m_pGameData->m_userName = str;
}

void UpgradeLayer::onExit()
{
    Layer::onExit();
}

void UpgradeLayer::Update(float dt)
{
    for (int i = 0; i < m_imageMcVec.size(); ++i)
    {
        ImageView* pImage = m_imageMcVec[i];
        float posX = m_pageViewMc->getPage(i)->getPositionX();
        //float factor = 1 - abs(posX - 1.5f * m_pageViewMc->getContentSize().width) / (m_pageViewMc->getContentSize().width * 2);
        float factor = 1 - abs(posX) / (m_pageViewMc->getContentSize().width * 2);
        if (factor >= 0.0f)
        {
            float opacity = std::max((factor - 0.5f) * 0.8f / 0.5f + 0.2f, 0.0f);
            pImage->setOpacity(opacity*255);
        }
    }
}

void UpgradeLayer::OnTouchAddHpButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            m_pGameData->m_mcExtraHpVec[m_pGameData->m_mcIndex]++;

            ReduceCoin(m_upgradeCoinCost);

            if (m_pGameData->m_currentCoin < m_upgradeCoinCost)
            {
                m_addAttackButton->setEnabled(false);
                m_addHpButton->setEnabled(false);
            }

            unsigned hp = (unsigned)GameData::Singleton().GetValueF(eGDS_MC, m_pGameData->m_mcIndex, "Hp") + m_pGameData->m_mcExtraHpVec[m_pGameData->m_mcIndex];
            char stringValue[10];
            sprintf(stringValue, "%d", hp);
            Text* hpLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Label_Hp"));
            hpLabel->setString(stringValue);
            hpLabel->setColor(Color3B::GREEN);
        }
        break;
        default:break;
    }
}

void UpgradeLayer::OnTouchAddAttackButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            m_pGameData->m_mcExtraAttackPointVec[m_pGameData->m_mcIndex]++;
            ReduceCoin(m_upgradeCoinCost);

            //Attack
            EBulletType bulletType = eBT_TitanNormal;
            switch(m_pGameData->m_mcIndex)
            {
            case 0:
                bulletType = eBT_TitanNormal;
                break;
            case 1:
                bulletType = eBT_StormNormal;
                break;
            case 2:
                bulletType = eBT_CharmNormal;
            }

            if (m_pGameData->m_currentCoin < m_upgradeCoinCost)
            {
                m_addAttackButton->setEnabled(false);
                m_addHpButton->setEnabled(false);
            }

            unsigned attackPoint = (unsigned)GameData::Singleton().GetValueF(eGDS_Bullet, bulletType, "AttackPoint") + m_pGameData->m_mcExtraAttackPointVec[m_pGameData->m_mcIndex];
            char stringValue[10];
            sprintf(stringValue, "%d", attackPoint);
            Text* attackLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Label_Attack"));
            attackLabel->setString(stringValue);
            attackLabel->setColor(Color3B::GREEN);
        }
        break;
        default:break;
    }
}

void UpgradeLayer::OnTouchButtonReturn( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            if (m_isUpgradeMcProperty)
            {
                SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

                SceneManager::CreateScene(MainMenu_Home);
            }
            else
            {
                SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

                m_isUpgradeMcProperty = true;

                UiManager::Singleton().GetWidget(m_pUiRoot, "TextField0")->setVisible(m_isUpgradeMcProperty);
                UiManager::Singleton().GetWidget(m_pUiRoot, "TextField1")->setVisible(!m_isUpgradeMcProperty);

                UiManager::Singleton().GetWidget(m_pUiRoot, "MCUpgradePanel")->setVisible(m_isUpgradeMcProperty);
                UiManager::Singleton().GetWidget(m_pUiRoot, "PowerUpgradePanel")->setVisible(!m_isUpgradeMcProperty);

                UpdateMcPropertyUI();

                m_rightChangeMcButton->setVisible(true);
                m_leftChangeMcButton->setVisible(true);
            }
        }
    }
}

void UpgradeLayer::OnTouchStartButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            if (m_pGameData->m_userName != "")
            {
                if (m_isUpgradeMcProperty)
                {
                    SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

                    m_isUpgradeMcProperty = false;
                    
                    UiManager::Singleton().GetWidget(m_pUiRoot, "TextField0")->setVisible(m_isUpgradeMcProperty);
                    UiManager::Singleton().GetWidget(m_pUiRoot, "TextField1")->setVisible(!m_isUpgradeMcProperty);

                    UiManager::Singleton().GetWidget(m_pUiRoot, "MCUpgradePanel")->setVisible(m_isUpgradeMcProperty);
                    UiManager::Singleton().GetWidget(m_pUiRoot, "PowerUpgradePanel")->setVisible(!m_isUpgradeMcProperty);
                    m_rightChangeMcButton->setVisible(false);
                    m_leftChangeMcButton->setVisible(false);

                    UpdatePowerUpgradeUI();
                }
                else
                {
                    SoundManager::Singleton().PlayEffect("Sound/Effect_UIClickStart.WAV");
                    
                    SceneManager::CreateScene(MainMenu_Game);

                    SoundManager::Singleton().stopBackgroundMusic();
                }
            }
        }
        break;
        default:break;
    }
}

void UpgradeLayer::OnTouchRightChangeButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            if (m_isUpgradeMcProperty)
            {
                int curPageIndex = m_pageViewMc->getCurPageIndex();
                if (curPageIndex < m_pageViewMc->getPages().size()-1)
                {
                    m_pageViewMc->scrollToPage(++curPageIndex);
                }
            }

        }
        break;
        default:break;
    }
}

void UpgradeLayer::OnTouchLeftChangeButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            if (m_isUpgradeMcProperty)
            {
                int curPageIndex = m_pageViewMc->getCurPageIndex();
                if (curPageIndex > 0)
                {
                    m_pageViewMc->scrollToPage(--curPageIndex);
                }
            }
        }
        break;
        default:break;
    }
}

void UpgradeLayer::OnTouchPayItemButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            int index = static_cast<Node*>(pSender)->getTag();
            m_pGameData->m_bonusIndexVec.push_back(index);
            int cost = m_pGameData->GetValueF(eGDS_Bonus, index, "Cost");
            ReduceCoin(cost);
        }
        break;
        default:break;
    }
}

void UpgradeLayer::ReduceCoin(int cost)
{
    m_pGameData->ReduceCoin(cost);

    char stringValue[10];
    unsigned energy = m_pGameData->m_currentCoin;
    sprintf(stringValue, "%d", energy);
    m_currentCoinButton->setTitleText(stringValue);
}
//------------------------------------------------------------------
//
// SceneUpgrade
//
//------------------------------------------------------------------
void SceneUpgrade::runThisTest()
{
    UpgradeLayer* pLayer = UpgradeLayer::create();
    addChild(pLayer, 0);

    if (Director::getInstance()->getRunningScene())
        Director::getInstance()->replaceScene(this);
    else
        Director::getInstance()->runWithScene(this);
}