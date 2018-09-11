#ifndef  _APP_MagicCircle_H_
#define  _APP_MagicCircle_H_

#include "GameObject/GameObject.h"

USING_NS_CC;

class MagicCircle : public GameObject
{
    typedef std::vector<Vec2> TPointVector;

public:
    MagicCircle();
    MagicCircle(const TPointVector& magicPointVector, const TPointVector& magicPointLogicVector, float durationTime, EGeneralType type);
    virtual ~MagicCircle();

    virtual void onEnter();
    virtual void onExit();

    virtual void StateUpdate(float deltaTime);

    bool IsSucceed();
    bool IsFailed();

protected:
    DECLARE_FSM(MagicCircle);
    DECLARE_STATE(Idle);
    DECLARE_STATE(Operating);
    DECLARE_STATE(Succeed);
    DECLARE_STATE(Failed);
    DECLARE_STATE(Dead);

    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouche, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouche, Event *pEvent);

    void AddGraphics();

    void MoveParticle();

    EGeneralType m_associateGType;

    TPointVector m_magicPointVector;
    TPointVector m_magicPointLogicVector;
    float m_deltaTime;
    float m_elapsedTime;
    float m_durationTime;

    Sprite* m_pMainSprite;
    MotionStreak* m_pStreak;
    ParticleGalaxy* m_pParticle;

    unsigned int m_particlePointNum;
};

#endif // _APP_MagicCircle_H_