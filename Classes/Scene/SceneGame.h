#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "SceneBase.h"
#include "Ui/UiManager.h"
#include "Ui/SpriteButton/SpriteButton.h"

NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

namespace cs{class CocoPanel;}
class BuildingLogic;
class BackgroundLogic;
class MonsterLogic;
class GeneralGroupLogic;

class GameLayer: public Layer
{
public:
    GameLayer(void);
    ~GameLayer(void);

    virtual void onEnter();
    virtual void onExit();

    CREATE_FUNC(GameLayer)

    void Pause(bool showPauseUi, Ref* pExceptTarget);
    void Resume(bool immediate);


private:
    void Init();
    void Update(float dt);  
    void firstUpdate();

    void OnTouchPauseButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchResumeButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchMainMenuButton( Ref* pSender, Widget::TouchEventType type );

    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouche, Event *pEvent);

private:
    bool m_updated;
    float m_tipTimer;
    Layout* m_pUiRoot;

    GeneralGroupLogic* m_pGeneralGroupLogic;

    Button* m_pauseButton;

    Layout* m_inGameMenuPanel;
    Button* m_resumeButton;
    
    Button* m_mainMenuButton;
    
    Text* m_countDownLabel;
    float m_resumeTimer;
    float m_oneSecondTimer;
    unsigned m_countDownValue;
    bool m_isResuming;
    bool m_isPuased;
    bool m_showPauseUi;
    Vector<Node*> m_pActionSet;
};

class SceneGame : public SceneBase
{
public:
    virtual void runThisTest();
};

#endif
