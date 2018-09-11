#ifndef _UpgradeSCENE_H_
#define _UpgradeSCENE_H_

#include "SceneBase.h"
#include "Ui/SpriteButton/SpriteButton.h"

NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

class BuildingLogic;
class GameData;
class UpgradeLayer: public Layer
{
public:
    UpgradeLayer(void);
    ~UpgradeLayer(void);

    virtual void onEnter();
    virtual void onExit();

    CREATE_FUNC(UpgradeLayer)

private:
    void Update(float dt);  

    void OnTouchAddHpButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchAddAttackButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchStartButton( Ref* pSender, Widget::TouchEventType type );

    void TextFieldInserted( Ref* pSender );

    void OnTouchRightChangeButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchLeftChangeButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchPayItemButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchButtonReturn( Ref* pSender, Widget::TouchEventType type );
    
    void UpdateMcPropertyUI();
    void UpdatePowerUpgradeUI();
    void ReduceCoin(int cost);

    void onPageUpdated(Ref* pSender, PageView::EventType type);
private:
    Layout* m_pUiRoot;

    unsigned m_upgradeCoinCost;
    bool m_isUpgradeMcProperty;

    Text* m_currentHpLabel;
    Text* m_currentAttackLabel;
    Button* m_currentCoinButton;

    // Add Hp/Attack Button
    Button* m_addHpButton;
    Button* m_addAttackButton;
    
    //MC portrait
    std::vector<Sprite*> m_mcPortraitVec;
    
    Button* m_startButton;
    
    GameData* m_pGameData;

    Button* m_rightChangeMcButton;
    Button* m_leftChangeMcButton;

    std::vector<Button*> m_payItemButtonVec;
    std::vector<Button*> m_payItemCostButtonVec;

    PageView* m_pageViewMc;
    std::vector<ImageView*> m_imageMcVec;
};

class SceneUpgrade : public SceneBase
{
public:
    virtual void runThisTest();
};

#endif
