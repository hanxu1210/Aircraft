#include "CommonHeaders.h"

#include "VisibleRect.h"

Rect VisibleRect::s_visibleRect;

void VisibleRect::lazyInit()
{
    if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
    {
        GLView* pEGLView = Director::getInstance()->getOpenGLView();
        s_visibleRect.origin = pEGLView->getVisibleOrigin();
        s_visibleRect.size = pEGLView->getVisibleSize();
    }
}

Rect VisibleRect::getVisibleRectWithBuffer()
{
    lazyInit();
    return Rect(s_visibleRect.origin.x - s_visibleRect.size.width * 0.25f, s_visibleRect.origin.y - s_visibleRect.size.height * 0.25f, 
        s_visibleRect.size.width * 1.5f, s_visibleRect.size.height * 1.5f);
}

Rect VisibleRect::getVisibleRectWithSmallBuffer()
{
    lazyInit();
    return Rect(s_visibleRect.origin.x - s_visibleRect.size.width * 0.025f, s_visibleRect.origin.y - s_visibleRect.size.height * 0.025f,
        s_visibleRect.size.width * 1.05f, s_visibleRect.size.height * 1.05f);
}

Rect VisibleRect::getVisibleRect()
{
    return Rect(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

Point VisibleRect::left()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::right()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::top()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::bottom()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

Point VisibleRect::center()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::leftTop()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::rightTop()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::leftBottom()
{
    lazyInit();
    return s_visibleRect.origin;
}

Point VisibleRect::rightBottom()
{
    lazyInit();
    return Vec2(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}
