#ifndef _HOME_SCENE_H_
#define _HOME_SCENE_H_

#include "SceneBase.h"
NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

class HomeLayer: public Layer
{
protected:

public:
    HomeLayer(void);
    ~HomeLayer(void);

    virtual void onEnter();

    CREATE_FUNC(HomeLayer)

private:
    void OnTouchPlayButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchCreditsButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchSettingButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchCloseButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchSoundButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchVibrateButton( Ref* pSender, Widget::TouchEventType type );

private:
    float m_timer;

    Layout* m_pUiRoot;
};

class SceneHome : public SceneBase
{
public:
    virtual void runThisTest();
};

#endif
