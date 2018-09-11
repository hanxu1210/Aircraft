#ifndef  _APP_Item_H_
#define  _APP_Item_H_

#include "GameObject/GameObject.h"

USING_NS_CC;

class Item : public GameObject
{
public:
    Item();
    virtual ~Item();

    virtual void onEnter();
    virtual void onExit();

    virtual void StateUpdate(float deltaTime);

protected:
    DECLARE_FSM(Item);
    DECLARE_VIRTUAL_STATE(Move);
    DECLARE_VIRTUAL_STATE(Hit);
    bool CheckUnspawn();
    Sprite* m_pMainSprite;

    float m_speed;
    float m_deltaTime;
    bool m_isHit;
};

#endif // _APP_Monster_H_