#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

USING_NS_CC;
class SceneBase;

enum
{
    MainMenu_Game = 0,
    MainMenu_Credits,
    MainMenu_GameOver,
    MainMenu_Upgrade,
    MainMenu_Home,

    MainMenu_Count,
};

class SceneManager
{
public:
    static SceneBase* CreateScene(int nIdx);

};

#endif // __SCENE_MANAGER_H__
