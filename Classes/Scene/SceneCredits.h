#ifndef _CREDITS_SCENE_H_
#define _CREDITS_SCENE_H_

#include "SceneBase.h"
NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

class CreditsLayer: public Layer
{
protected:

public:
    CreditsLayer(void);
    ~CreditsLayer(void);

    virtual std::string title();
    virtual void onEnter();
    CREATE_FUNC(CreditsLayer)
private:
    void Update(float dt);  
    bool onTouchBegan(Touch *pTouche, Event *pEvent);
    void onTouchEnded( Touch* /*pTouche*/, Event* /*pEvent*/ );

private:
    float m_timer;

    Layout* m_pUiRoot;
    Text* m_label1;
};

class SceneCredits : public SceneBase
{
public:
    virtual void runThisTest();
};

#endif
