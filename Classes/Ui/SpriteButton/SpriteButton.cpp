#include "CommonHeaders.h"

#include "Ui/SpriteButton/SpriteButton.h"

SpriteButton::SpriteButton()
    : m_normal(NULL), m_pressed(NULL), m_disabled(NULL)
{

}

void SpriteButton::Init( Vec2 pos, Node* parent, std::string normal, std::string pressed, std::string disabled, int zOrder )
{
    m_normal = Sprite::createWithSpriteFrameName(normal.c_str());
    if (!pressed.empty())
        m_pressed = Sprite::createWithSpriteFrameName(pressed.c_str());
    if (!disabled.empty())    
        m_disabled = Sprite::createWithSpriteFrameName(disabled.c_str());

    m_normal->setPosition(pos);
    if (m_pressed) m_pressed->setPosition(pos);
    if (m_disabled) m_disabled->setPosition(pos);

    parent->addChild(m_normal, zOrder);
    if (m_pressed) parent->addChild(m_pressed, zOrder);
    if (m_disabled) parent->addChild(m_disabled, zOrder);

    m_normal->setVisible(true);
    if (m_pressed) m_pressed->setVisible(false);
    if (m_disabled) m_disabled->setVisible(false);
}

void SpriteButton::SetScale( float scale )
{
    m_normal->setScale(scale);
    if (m_pressed) m_pressed->setScale(scale);
    if (m_disabled) m_disabled->setScale(scale);
}

void SpriteButton::SetRotation(float angle)
{
    m_normal->setRotation(angle);
    if (m_pressed) m_pressed->setRotation(angle);
    if (m_disabled) m_disabled->setRotation(angle);
}

void SpriteButton::PushDown()
{
    m_normal->setVisible(false);
    if (m_pressed) m_pressed->setVisible(true);
    if (m_disabled) m_disabled->setVisible(false);
}

void SpriteButton::Release()
{
    m_normal->setVisible(true);
    if (m_pressed) m_pressed->setVisible(false);
    if (m_disabled) m_disabled->setVisible(false);
}

void SpriteButton::Disable()
{
    m_normal->setVisible(false);
    if (m_pressed) m_pressed->setVisible(false);
    if (m_disabled) m_disabled->setVisible(true);
}

void SpriteButton::Hide()
{
    m_normal->setVisible(false);
    if (m_pressed) m_pressed->setVisible(false);
    if (m_disabled) m_disabled->setVisible(false);
}


