#ifndef  _APP_GENERAL_H_
#define  _APP_GENERAL_H_

#include "GameObject/GameObject.h"

namespace cs{class CocoImageView;}
class MagicCircle;
namespace cocos2d { namespace extension {class UIImageView;}}
USING_NS_CC_EXT;
class GeneralGroupLogic;

typedef std::vector<Vec2> TPointVector;

class General : public GameObject 
{
public:
    General();
    virtual ~General();

    virtual void onEnter();
    virtual void onExit();

    virtual void StateUpdate(float deltaTime);
    void SetGeneralGroupLogic(GeneralGroupLogic* pGeneralGroupLogic);

protected:
    void Pause();
    void Resume();

    DECLARE_FSM(General);
    DECLARE_VIRTUAL_STATE(Idle);
    DECLARE_VIRTUAL_STATE(ReadyToShowUp);
    DECLARE_VIRTUAL_STATE(ShowUp);
    DECLARE_VIRTUAL_STATE(CastSkill);
    DECLARE_VIRTUAL_STATE(Dead);

    EGeneralType m_type;

    Sprite* m_pMainSprite;
    Sprite* m_pProfileImage;

    float m_elapasedTime;
    float m_deltaTime;

    MagicCircle* m_magicCircle;
    TPointVector m_magicPointVec;
    TPointVector m_magicPointLogicVec;
    
    GeneralGroupLogic* m_pGeneralGroupLogic;
};

#endif // _APP_GENERAL_H_