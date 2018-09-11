#ifndef __VISIBLERECT_H__
#define __VISIBLERECT_H__

USING_NS_CC;

class VisibleRect
{
public:
    static Rect getVisibleRect();
    static Rect getVisibleRectWithBuffer();
    static Rect getVisibleRectWithSmallBuffer();

    static Point left();
    static Point right();
    static Point top();
    static Point bottom();
    static Point center();
    static Point leftTop();
    static Point rightTop();
    static Point leftBottom();
    static Point rightBottom();
private:
    static void lazyInit();
    static Rect s_visibleRect;
};

#endif /* __VISIBLERECT_H__ */
