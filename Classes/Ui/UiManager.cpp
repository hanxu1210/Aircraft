#include "CommonHeaders.h"

#include "Ui/UiManager.h"
#include "cocostudio/CCSGUIReader.h"

IMPLEMENT_SINGLETON(UiManager);

UiManager::UiManager()
    : m_pLayer(NULL)
    , m_pLayout(NULL)
    , m_isInUi(false)
{

}

UiManager::~UiManager()
{

}

// void UiManager::Init( Node* pNode )
// {
//     m_pUiLayer = cocos2d::extension::UILayer::create();
//     m_pUiLayer->scheduleUpdate();
//     pNode->addChild(m_pUiLayer, 5);
// }

// void UiManager::SetupWidget(const char* fileName)
// {
//     //m_pCurrentWidget = CCUIHELPER->createWidgetFromJsonFile("../UIProject/Json/Login.json");
//     if (m_pCurrentWidget)
//     {
//         m_pUiLayer->removeWidgetAndCleanUp(m_pCurrentWidget, true);
//         m_pCurrentWidget = NULL;
//     }
// 
//     m_pCurrentWidget = CCUIHELPER->createWidgetFromJsonFile(fileName);
//     m_pUiLayer->addWidget(m_pCurrentWidget);
// }

void UiManager::Update( float dt )
{
    m_isInUi = false;
}

// cocos2d::extension::UIWidget* UiManager::GetChildByName( const char* name )
// {
//     XAssert(m_pUiLayer, "m_pUiLayer is invalid.");
// 
//     return m_pUiLayer->getWidgetByName(name);
// }

// void UiManager::AddChildWidget( cocos2d::extension::UIWidget* child )
// {
//     XAssert(m_pUiLayer, "m_pUiLayer is invalid.");
// 
//     m_pUiLayer->addWidget(child);
// }

bool UiManager::IsInUi() const
{
    return m_isInUi;
}

void UiManager::SetIsInUi( bool isInUi )
{
    m_isInUi = isInUi;
}

void UiManager::SetVisible( bool visible )
{
    m_pLayer->setVisible(visible);
}

bool UiManager::IsVisible() const
{
    return m_pLayer->isVisible();
}

Layout* UiManager::SetupWidget(Node* pParentNode, const char* fileName)
{
    m_pLayer = Layer::create();

    pParentNode->addChild(m_pLayer, 10);

    m_pLayout = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fileName));
    m_pLayer->addChild(m_pLayout);

    return m_pLayout;
}

Layout* UiManager::GetLayoutChild( const char* name )
{
    XAssert(m_pLayout, "m_pUiLayer is invalid.");

    return static_cast<Layout*>(m_pLayout->getChildByName(name));
}

Widget* UiManager::GetWidget(const char* name)
{
    return GetWidget(m_pLayout, name);
}

Widget* UiManager::GetWidget(Widget* root, const char* name )
{
    XAssert(root, "root is invalid.");

    return Helper::seekWidgetByName(root, name);
}
