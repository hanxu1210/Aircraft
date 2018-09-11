#include "CommonHeaders.h"

#include "SceneHome.h"
#include "Ui/UiManager.h"
#include "Scene/SceneManager.h"
#include "Plugin/PluginShareWrapper.h"
#include "Plugin/PluginManagerWrapper.h"
USING_NS_CC;
//------------------------------------------------------------------
//
// HomeLayer
//
//------------------------------------------------------------------
HomeLayer::HomeLayer(void)
    : m_timer(2.0f)
    , m_pUiRoot(NULL)
{
}

HomeLayer::~HomeLayer(void)
{
    CCLOG("HomeLayer destructor");
}

void HomeLayer::onEnter()
{
    Layer::onEnter();

    m_pUiRoot = UiManager::Singleton().SetupWidget(this, "UIProject/Json/Home.json");

    Button* pButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Button_Play"));
    pButton->addTouchEventListener(CC_CALLBACK_2(HomeLayer::OnTouchPlayButton, this));

    pButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Button_Credits"));
    pButton->addTouchEventListener(CC_CALLBACK_2(HomeLayer::OnTouchCreditsButton, this));

    pButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Button_Setting"));
    pButton->addTouchEventListener(CC_CALLBACK_2(HomeLayer::OnTouchSettingButton, this));

    pButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Button_Close"));
    pButton->addTouchEventListener(CC_CALLBACK_2(HomeLayer::OnTouchCloseButton, this));

    pButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Button_Sound"));
    pButton->addTouchEventListener(CC_CALLBACK_2(HomeLayer::OnTouchSoundButton, this));
    pButton->setBright(SoundManager::Singleton().isEnabled());
    
    pButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Button_Vibrate"));
    pButton->addTouchEventListener(CC_CALLBACK_2(HomeLayer::OnTouchVibrateButton, this));
    pButton->setBright(SoundManager::Singleton().isVibrateEnabled());
 
    if (!SoundManager::Singleton().isBackgroundMusicPlaying())
       SoundManager::Singleton().playBackgroundMusic("Sound/BGM_MainMenu.mp3", true);
}

void HomeLayer::OnTouchPlayButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");
            
            SceneManager::CreateScene(MainMenu_Upgrade);
        }
        break;
        default:break;
    }
}

void HomeLayer::OnTouchCreditsButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");
            

            SceneManager::CreateScene(MainMenu_Credits);

            // Test share.
//             NsAircraft::PluginShareWrapper* umengGameShare = dynamic_cast<NsAircraft::PluginShareWrapper*>
//             (NsAircraft::PluginManagerWrapper::sharedInstance()->getProtocol(NsAircraft::PluginManagerWrapper::PP_SHARE));

//             if (umengGameShare)
//             {
//                 //Value shareText = Localization::sharedInstance()->getValue(ShareConfig::ShareDefaultText);
//                 umengGameShare->share("test", "boss.png");
//             }
        }
        break;
        default:break;
    }
}

void HomeLayer::OnTouchSettingButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

            Layout* pSettingPanel = DynamicCast<Layout*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Panel_setting"));
            pSettingPanel->setVisible(true);
        }
        break;
        default:break;
    }
}

void HomeLayer::OnTouchCloseButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

            Layout* pSettingPanel = DynamicCast<Layout*>(UiManager::Singleton().GetWidget(m_pUiRoot, "Panel_setting"));
            pSettingPanel->setVisible(false);
        }
        break;
        default:break;
    }
}

void HomeLayer::OnTouchSoundButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

            bool isBright = static_cast<Button*>(pSender)->isBright();
            static_cast<Button*>(pSender)->setBright(!isBright);
            SoundManager::Singleton().setEnabled(!isBright);
        }
        break;
        default:break;
    }
}

void HomeLayer::OnTouchVibrateButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
        case Widget::TouchEventType::ENDED:
        {
            SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");
            
            bool isBright = static_cast<Button*>(pSender)->isBright();
            static_cast<Button*>(pSender)->setBright(!isBright);
            SoundManager::Singleton().setVibrateEnabled(!isBright);
        }
            break;
        default:break;
    }
}

//------------------------------------------------------------------
//
// SceneHome
//
//------------------------------------------------------------------
void SceneHome::runThisTest()
{
    HomeLayer* pLayer = HomeLayer::create();
    addChild(pLayer, 0);

    if (Director::getInstance()->getRunningScene())
        Director::getInstance()->replaceScene(this);
    else
        Director::getInstance()->runWithScene(this);
}
