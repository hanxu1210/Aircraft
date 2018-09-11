#ifndef  _APP_GeneralGroupLogic_H_
#define  _APP_GeneralGroupLogic_H_

#include "GameObject/GameObjectEnum.h"

NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;

typedef std::vector< EGeneralType > TGeneralDataVec;
typedef std::map< Button*, float > TButtonCdMap;

class GameData;
class MainCharacter;

class GeneralGroupLogic : public Node
{
public:
    GeneralGroupLogic();
    GeneralGroupLogic(const TGeneralDataVec& generalData);
    virtual ~GeneralGroupLogic();

    virtual void onEnter();
    virtual void onExit();

    void Update(float dt);

    void SetIsLaunchingSkillFlag(bool isLaunching);
    
private:
    void OnTouchSkillButton( Ref* pSender, Widget::TouchEventType type );
    void OnTouchNoSkillButton( Ref* pSender, Widget::TouchEventType type );

    Button* m_pSkillButton;
    LoadingBar* m_SkillButtonMaskBar;

    MainCharacter* m_pMC;
    
private:
    TGeneralDataVec m_generalData;

    TButtonCdMap m_buttonCdMap;

    GameData* m_pGameData;
    bool m_skillDisable;
    
    float m_skillTime;
    float m_skillTimer;
    bool m_isLaunchingSkill;
};

#endif //_APP_GeneralGroupLogic_H_