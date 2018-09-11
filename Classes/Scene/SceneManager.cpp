#include "CommonHeaders.h"

#include "Scene/SceneManager.h"

#include "Scene/SceneCredits.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneGameOver.h"
#include "Scene/SceneUpgrade.h"
#include "Scene/SceneHome.h"

USING_NS_CC;


SceneBase* SceneManager::CreateScene( int nIdx )
{
    Director::getInstance()->purgeCachedData();

    SceneBase* pScene = NULL;

    switch (nIdx)
    {
    case MainMenu_Credits:
        pScene = new SceneCredits(); break;
    case MainMenu_Game:
        pScene = new SceneGame(); break;
    case MainMenu_GameOver:
        pScene = new SceneGameOver(); break;
    case MainMenu_Upgrade:
        pScene = new SceneUpgrade(); break;
    case MainMenu_Home:
        pScene = new SceneHome(); break;
    default:
        break;
    }

    if (pScene)
    {
        pScene->runThisTest();
        pScene->autorelease();
    }

    return pScene;
}