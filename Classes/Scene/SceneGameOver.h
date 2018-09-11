#ifndef _GAMEOVERSCENE_H_
#define _GAMEOVERSCENE_H_

#include "SceneBase.h"

NS_CC_BEGIN 
    namespace ui{class Layout; class ListView; class ScrollView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

class BuildingLogic;

class GameOverLayer: public Layer
{
public:
    GameOverLayer(void);
    ~GameOverLayer(void);

    virtual void onEnter();

    CREATE_FUNC(GameOverLayer)

private:
    void setLeaderboard();

    void OnTouchPlayAgainButton( Ref* pSender, Widget::TouchEventType type );
private:
    Layout* m_pUiRoot;

    Button* m_playAgainButton;

    ScrollView* m_leaderboardScrollView;
    std::vector<Sprite*> m_leaderboardItemSpriteVec;

    std::vector<Layout*> m_panelVec;
};

class SceneGameOver : public SceneBase
{
public:
    virtual void runThisTest();
};

#endif
