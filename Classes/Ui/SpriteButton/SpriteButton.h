#ifndef __SpriteButton_H__
#define __SpriteButton_H__

class SpriteButton
{
public:
    SpriteButton();
    void Init(Vec2 pos, Node* parent, std::string normal, std::string pressed, std::string disabled, int zOrder = 0);
    void SetScale(float scale);
    void SetRotation(float angle);
    
    void PushDown();
    void Release();
    void Disable();
    void Hide();
protected:
    Sprite* m_normal;
    Sprite* m_pressed;
    Sprite* m_disabled;
};


#endif    // __SpriteButton_H__
