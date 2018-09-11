#include "CommonHeaders.h"

#include "GameLogic/GeneralGroupLogic.h"
#include "GameObject/Generals/GeneralFactory/GeneralFactory.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "Ui/UiManager.h"
#include "GameData.h"
#include "GameObject/MainCharacter.h"
#include "GameLogic/SkillDefines.h"


const char* NormalPng[] =
{
    "G_DiaoChan.png",
    "G_MaChao.png"
};

const char* SelectedPng[] =
{
    "G_DiaoChanSelect.png",
    "G_MachaoSelect.png"
};

const char* DisablePng[] =
{
    "G_DiaoChanDisable.png",
    "G_MachaoDisable.png"
};

GeneralGroupLogic::GeneralGroupLogic()
{
}

GeneralGroupLogic::GeneralGroupLogic( const TGeneralDataVec& generalData)
    : m_generalData(generalData)
    , m_skillTime(6.0f)
    , m_skillTimer(0.0f)
    , m_isLaunchingSkill(false)
    , m_pMC(NULL)
{
}

GeneralGroupLogic::~GeneralGroupLogic()
{
    m_generalData.clear();
    m_buttonCdMap.clear();
}

void GeneralGroupLogic::onEnter()
{
    Node::onEnter();

    m_pGameData = GameData::SingletonPtr();

    m_pSkillButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget("SkillButton"));
    m_pSkillButton->addTouchEventListener(CC_CALLBACK_2(GeneralGroupLogic::OnTouchSkillButton, this));

    Button* noSkillButton = DynamicCast<Button*>(UiManager::Singleton().GetWidget("NoSkillButton"));
    noSkillButton->addTouchEventListener(CC_CALLBACK_2(GeneralGroupLogic::OnTouchNoSkillButton, this));

    m_SkillButtonMaskBar = DynamicCast<LoadingBar*>(UiManager::Singleton().GetWidget("SkillMaskBar"));

    ImageView* pImageHint = DynamicCast<ImageView*>(UiManager::Singleton().GetWidget("Image_SkillHint"));
    pImageHint->setOpacity(0);

    m_pMC = MainPlayerLogic::Singleton().GetMC();
    m_skillDisable = m_pMC->GetMp() == 0;
    UiManager::SetButtonEnabled(m_pSkillButton, !m_skillDisable);
}

void GeneralGroupLogic::onExit()
{
    Node::onExit();
}

void GeneralGroupLogic::Update( float dt )
{
    if (m_skillDisable && m_pMC->GetMp() >= m_pMC->GetSkillMpCost())
    {
        m_skillDisable = false;
        UiManager::SetButtonEnabled(m_pSkillButton, !m_skillDisable);
    }
    else if (!m_skillDisable && m_pMC->GetMp() < m_pMC->GetSkillMpCost())
    {
        m_skillDisable = true;
        UiManager::SetButtonEnabled(m_pSkillButton, !m_skillDisable);
    }

    if (m_isLaunchingSkill)
    {
        m_skillTimer -= dt;

        m_SkillButtonMaskBar->setPercent(m_skillTimer/m_skillTime*100);

        if (m_skillTimer < 0)
        {
            m_isLaunchingSkill = false;
            UiManager::SetButtonEnabled(m_pSkillButton, m_pMC->GetMp() >= m_pMC->GetSkillMpCost());
            m_pMC->SetCurSkill(eS_Null);
        }
    }

}

void GeneralGroupLogic::OnTouchSkillButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            m_pMC->GasBottonRelease();

            if (!m_isLaunchingSkill)
            {
                if (m_pMC->GetMp() >= m_pMC->GetSkillMpCost())
                {
                    if (m_pMC->ReduceMp(m_pMC->GetSkillMpCost()))
                    {
                        m_pMC->SetCurSkill(m_pMC->GetMcSkillType());

                        SoundManager::Singleton().PlayEffect("Sound/Effect_UIClick.mp3");

                        UiManager::SetButtonEnabled(m_pSkillButton, false);
                        m_isLaunchingSkill = true;
                        m_skillTimer = m_skillTime;
                    }
                }
            }
        }
        break;
        default:break;
    }
}

void GeneralGroupLogic::OnTouchNoSkillButton( Ref* pSender, Widget::TouchEventType type )
{
    switch (type)
    {
    case Widget::TouchEventType::ENDED:
        {
            m_pMC->GasBottonRelease();

            ImageView* pImageHint = DynamicCast<ImageView*>(UiManager::Singleton().GetWidget("Image_SkillHint"));
            Sequence* pSequence = Sequence::create(CCFadeIn::create(0.2), CCDelayTime::create(0.5), CCFadeOut::create(0.2), NULL);
            pImageHint->runAction(pSequence);
        }
        break;
    case Widget::TouchEventType::BEGAN:
        {
            m_pMC->GasBottonPushDown();
        }
        break;
        default:break;
    }
}

void GeneralGroupLogic::SetIsLaunchingSkillFlag(bool isLaunchingSkill)
{
    m_pMC->SetIsLaunchingSkill(isLaunchingSkill);
}
