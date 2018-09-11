#ifndef  _APP_MainCharacter_H_
#define  _APP_MainCharacter_H_

#include "GameObject/GameObject.h"
#include "GameLogic/SkillDefines.h"

NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;
class Replication;
#include "CryptoData.h"

class MainCharacter : public GameObject
{
public:
    MainCharacter(unsigned index);
    virtual ~MainCharacter();

    virtual void onEnter();
    virtual void onExit();
    virtual void Killed();

    void StateUpdate(float deltaTime);
    float GetSpeedX();
    void AccelerateSpeedX();

    float GetSpeedY();
    
    void TransactToIdle();
    void TransactToFly();

    void GasBottonPushDown();
    void GasBottonRelease();
    void GasBottonCancel();

    bool ReduceMp( int reduceEnergy );
    bool IncreaseMp( int increaseEnergy );
    bool IsMpFull();
    INT GetMp() const;
    INT GetSkillMpCost() const;
    
    void Shoot();
    float GetShootInterval();
    //Skill
    void SetIsLaunchingSkill(bool enable);
    ESkill GetCurSkill() const;
    void SetCurSkill(ESkill skill);
    ESkill GetMcSkillType();

    //Bonus
    float GetBonusKillingScore();
private:
    void ReduceHp( unsigned hp );
    void FlyUpdate();
    enum EMCAnimationState
    {
        eMCAS_FlyUp,
        eMCAS_FlyUpLoop,
        eMCAS_FlyDown,
        eMCAS_FlyDownLoop,
        eMCAS_Transform,
        eMCAS_Stand
    };
    //void PlayAnimation(EMCAnimationState state, bool repeat, bool reverse, SEL_CallFunc func);
    void PlayAnimation(EMCAnimationState state, bool repeat, bool reverse, const std::function<void()>& func);
    void PreFlyAnimationCallback();
    void FinishTransfromAnimationCallback();
    //For Fly up&down
    void FlyUpAnimationCallback();
    void FlyDownAnimationCallback();

    void CheckInvisible();
    void ApplyBonus();

    DECLARE_FSM(MainCharacter);
    DECLARE_STATE(Idle);
    DECLARE_STATE(Fly);
    DECLARE_STATE(Transform);
    DECLARE_STATE(Killed);
private:
    unsigned m_index;
    Sprite* m_pMainSprite;
    
    Text* m_pHpLabel;
    Text* m_pMpLabel;

    float m_speedY;
    float m_gasSpeedY;
    float m_speedX;
    bool m_isGasOn;
    bool m_isShooting;
    float m_shootTimer;
    
    float m_shootInterval;
    float m_gravityAccelarte;
    float m_gravitySpeed;
    float m_deltaTime;

    float m_invisibleTime;
    float m_invisibleTimer;
    bool m_isLaunchingSkill;
    float m_moveYThreshold;
    bool m_enterScene; // use to check in Fly state contractor for play transform to plane animation
                       // when just start game, rather switch to Fly from Idle(adding oil)
    float m_initPositionX;
    float m_dragBackSpeed;

    float m_killTimer;

    ParticleSystem* m_fireParticle;
    ParticleBatchNode* m_fireParticleBatch;
    float m_accelerateSpeedXAfterDash;
    
    unsigned m_bulletCount;

    CryptoData<INT> m_maxMp;
    CryptoData<INT> m_mp;
    CryptoData<INT> m_skillMpCost;
    
    ESkill m_curSkill;

    // bonus
    float m_bonusKillingScore;

    // Animation
    FiniteTimeAction* m_animationAction;
    unsigned m_animationFlyUpNum;
    float m_animationFlyUpInterval;
    unsigned m_animationFlyDownNum;
    float m_animationFlyDownInterval;
    unsigned m_animationTransformNum;
    float m_animationTransformInterval;
    unsigned m_animationStandNum;
    float m_animationStandInterval;
    std::vector<Replication*> m_replicationVec;
    bool m_playingTransformAnimation;
};

#endif // _APP_MainCharacter_H_