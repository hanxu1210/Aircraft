#include "CommonHeaders.h"
#include "AppDelegate.h"

#include <vector>
#include <string>

#include "Scene/SceneManager.h"
#include "Scene/SceneGame.h"
#include "GameData.h"
#include "Sound/SoundManager.h"
#include "GameLogic/AdsLogic/AdsLogic.h"
#include "Plugin/PluginManagerWrapper.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    if (GameData::IsSingletonCreated())
        GameData::DestroySingleton();

    if (SoundManager::IsSingletonCreated())
        SoundManager::DestroySingleton();
    
    if (!UiManager::IsSingletonCreated())
        UiManager::DestroySingleton();

    if (NsAircraft::AdsLogic::IsSingletonCreated())
        NsAircraft::AdsLogic::DestroySingleton();
    
    NsAircraft::PluginManagerWrapper::sharedInstance()->purgeInstance();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set Resolution size
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    //auto designSize = Size(640, 960);
    //auto designSize = Size(1280, 1920);
    //auto designSize = Size(640, 1136);
    //auto designSize = Size(1024, 768);
    auto designSize = Size(1136, 640);
    glview->setFrameSize(1280, 720);
    glview->setFrameZoomFactor(0.75f);
#else
    auto designSize = Size(1136, 640);
#endif
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);


    SceneManager::CreateScene(MainMenu_Home);

    if (!UiManager::IsSingletonCreated())
        UiManager::CreateSingleton();

    // GamdData Init
    if (!GameData::IsSingletonCreated())
        GameData::CreateSingleton();

    if (!SoundManager::IsSingletonCreated())
        SoundManager::CreateSingleton();
    
    
    // Plugin Init
    NsAircraft::PluginManagerWrapper::sharedInstance()->init();
    
    if (!NsAircraft::AdsLogic::IsSingletonCreated())
        NsAircraft::AdsLogic::CreateSingleton();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

    Node* pNode = Director::getInstance()->getRunningScene()->getChildByTag(1984);
    if (pNode)
    {
        GameLayer* pGameLayer = (GameLayer*)pNode;
        pGameLayer->Pause(true, NULL);
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
