#ifndef  _APP_MagicPoint_H_
#define  _APP_MagicPoint_H_

#include "GameObject/GameObject.h"

namespace cs{class CocoImageView;}

USING_NS_CC_EXT;

class MagicPoint : public GameObject
{
public:
    MagicPoint();
    virtual ~MagicPoint();

    virtual void onEnter();
    virtual void onExit();
    /*void draw()
    {
        glLineWidth( 2.0f );
        ccDrawCircle(Vec2(0.0f, 0.0f), m_collisionRadius, (float)M_PI*2.0f, 50, false);
    }*/
protected:
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    float m_collisionRadius;
};

#endif // _APP_MagicPoint_H_