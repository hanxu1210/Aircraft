#ifndef  _APP_BackgroundLogic_H_
#define  _APP_BackgroundLogic_H_

#include "utilities/FSM/FsmInclude.h"
#include "GameObject/GameObjectEnum.h"

NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

class BuildingLogic;
class GpeLogic;
class GameObject;

enum EBackgroundMoveStatus
{
    eBMS_Normal,
    eBMS_Stop,
    eBMS_Dash
};

struct SectionPattern
{
    SectionPattern()
        : m_pBackgroundNode(NULL)
    {}

    Node* m_pBackgroundNode;

    std::string m_imageName;
    struct MonsterData
    {
        EMonsterType type;
        unsigned variationId;
        Vec2 pos;
        unsigned groupId;
    };
    std::vector<MonsterData> m_monsterVec;

    struct GpeData
    {
        unsigned type;
        Vec2 pos;
    };
    std::vector<GpeData> m_gpeVec;

    struct ItemData
    {
        unsigned type;
        Vec2 pos;
    };
    std::vector<ItemData> m_itemVec;
};

class BackgroundLogic : public NodeGrid
{
    DECLARE_SINGLETON(BackgroundLogic);

public:
    void Init(Layer* layer);
    void Uninit();    
    virtual void onEnter();
    virtual void onExit();

    void StateUpdate(float deltaTime);
    
    void SetMoveStatus(EBackgroundMoveStatus status);

    void ShakeCamera(float duration);

    float GetCurrentSpeed() const;

    Node* GetNextSectionNode();
    Node* GetCurrentSectionNode();

    void StartHurtEffect(float duration);
    void ShowEventTime(std::string text);

    // Every sec, player get scores
    void UpdateTimeScore();
private:    
    void LoadMap();

    DECLARE_FSM(BackgroundLogic);
    DECLARE_STATE(Idle);
    DECLARE_STATE(Move);
    DECLARE_STATE(Stop);
    DECLARE_STATE(Dash);

    float GetFlySpeedX();
    
    void CreateSection(SectionPattern& pattern, Vec2 point, unsigned depth, bool firstPattern = false);
    void RemoveSection(SectionPattern& pattern);

    void Move(float speed);

    unsigned GetNextPatternIndex(const std::vector<SectionPattern>& patternVec, unsigned currentIndex);
    void CheckShakeCamera();

    unsigned GetProgressToNextDash();
    void ShowBonusTimeAnimationFinished();

    unsigned getBossHpPercentage(GameObject*& pGameObject);
private:
    Layer* m_parent;
    float m_deltaTime;
   
    std::vector<SectionPattern> m_sectionPatternVec;
    std::vector<SectionPattern> m_backgroundPatternVec;
    std::vector<std::pair<unsigned, float>> m_eventMapIndexVec;

    EBackgroundMoveStatus m_moveStatus;

    float m_dashTimer;
    reflected float m_dashTime;

    // add score every 1 sec
    float m_addScoreTimer;

    unsigned m_currentSection;
    unsigned m_currentBackground;

    Action* m_pShakeCameraAction;
    float m_shadeCameraDuration;

    float m_currentSpeed;
    float m_dashSpeed;
    LoadingBar* m_progressBar;
    LoadingBar* m_progressBar2;
    Text* m_pProgressLabel;

    Sprite* m_pHurtSprite;
    RepeatForever* m_hurtEffectAction;
    float m_hurtEffectDuration;
    
    Sprite* m_pBonusTimeSprite;
    CCLabelTTF* m_eventText;

    static const unsigned MAX_MONSTER_GROUP_ID = 0xFFFFFFFF;
    unsigned m_usedMonsterGroupId;
    unsigned GetMonsterGroupId()
    {
        if (m_usedMonsterGroupId == MAX_MONSTER_GROUP_ID)
            m_usedMonsterGroupId = 0;
        else
            ++m_usedMonsterGroupId;

        return m_usedMonsterGroupId;
    }

};

#endif //_APP_BackgroundLogic_H_