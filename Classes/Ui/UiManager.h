#ifndef  _APP_UIManager_H_
#define  _APP_UIManager_H_

// namespace cocos2d { namespace extension {class UIWidget;class UILayer;}}
// //using namespace cocos2d::extension;
// using namespace cocos2d;
// #include "Ui/SpriteButton/SpriteButton.h"

#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class UiManager
{
public:
    DECLARE_SINGLETON(UiManager);

public:
    //void Init(Node* pNode);
    void Update(float dt);

    Layout* SetupWidget(Node* pParentNode, const char* fileName);
    Layout* GetLayoutChild(const char *name);
    Widget* GetWidget(Widget* root, const char* name );
    Widget* GetWidget(const char* name );

    //void SetupWidget(const char* fileName);
    //cocos2d::extension::UIWidget* GetChildByName(const char *name);
    //void AddChildWidget(cocos2d::extension::UIWidget* child);


    bool IsInUi() const;
    void SetIsInUi(bool isInUi);
    void SetVisible(bool visible);
    bool IsVisible() const;

    static void SetButtonEnabled(ui::Button * paramButton, bool paramEnabled)
    {
        if (!paramButton) return;

        if (paramEnabled)
        {
            paramButton->setBright(true);
            paramButton->setEnabled(true);
            paramButton->setTouchEnabled(true);
        }
        else
        {
            paramButton->setBright(false);
            paramButton->setEnabled(false);
            paramButton->setTouchEnabled(false);
        }
    }
private:
    Layer* m_pLayer;
    Layout* m_pLayout;

    //cocos2d::extension::UILayer* m_pUiLayer;
    //cocos2d::extension::UIWidget* m_pCurrentWidget;

    bool m_isInUi;
};

#endif // _APP_UIManager_H_