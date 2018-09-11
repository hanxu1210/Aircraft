#include "CommonHeaders.h"

#include "SceneBase.h"
#include "SceneManager.h"

SceneBase::SceneBase(bool bPortrait)
{
    Scene::init();
}

void SceneBase::onEnter()
{
    Scene::onEnter();
}