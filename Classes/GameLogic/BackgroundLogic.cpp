#include "CommonHeaders.h"

#include "GameLogic/BackgroundLogic.h"
#include "GameData.h"
#include "GameObject/MainCharacter.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/Monster/Monster.h"
#include "GameObject/Gpe/EnergyStation/EnergyStation.h"
#include "GameObject/Items/Gold/Gold.h"
#include "GameObject/GameObjectManager.h"
#include "GameLogic/GpeLogic.h"
#include "Gamelogic/MainPlayerLogic.h"
#include "Ui/UiManager.h"

IMPLEMENT_SINGLETON(BackgroundLogic);

BackgroundLogic::BackgroundLogic()
    : m_parent(NULL)
    , m_moveStatus(eBMS_Normal)
    , m_dashTime(11.5f)
    , m_addScoreTimer(1.0f)
    , m_currentSection(0)
    , m_currentBackground(0)
    , m_pShakeCameraAction(NULL)
    , m_shadeCameraDuration(0.0f)
    , m_currentSpeed(0.0f)
    , m_pHurtSprite(NULL)
    , m_hurtEffectAction(NULL)
    , m_hurtEffectDuration(0.0f)
    , m_pBonusTimeSprite(NULL)
    , m_usedMonsterGroupId(0)
{
    Texture2D *texture2D = Director::getInstance()->getTextureCache()->addImage("bg.png");
    auto spBatchNode = SpriteBatchNode::createWithTexture(texture2D);
    addChild(spBatchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bg.plist");

    texture2D = Director::getInstance()->getTextureCache()->addImage("bgLong.png");
    spBatchNode = SpriteBatchNode::createWithTexture(texture2D);
    addChild(spBatchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bgLong.plist");

    texture2D = Director::getInstance()->getTextureCache()->addImage("npc.png");
    spBatchNode = SpriteBatchNode::createWithTexture(texture2D);
    addChild(spBatchNode);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("npc.plist");
}

BackgroundLogic::~BackgroundLogic()
{   
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("bg.plist");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("bgLong.plist");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("npc.plist");
}

void BackgroundLogic::Init( Layer* layer )
{
    m_parent = layer;
    m_parent->addChild(this, 0);
}

void BackgroundLogic::Uninit()
{
    removeFromParent();
    m_hurtEffectAction->release();
}

void BackgroundLogic::onEnter()
{
    Node::onEnter();

    m_dashSpeed = GetFlySpeedX() * 3.0f;

    LoadMap();
    
    INIT_FSM(Idle);

    // Progress bar
    m_progressBar = DynamicCast<LoadingBar*>(UiManager::Singleton().GetWidget("ProgressBar"));
    m_progressBar2 = DynamicCast<LoadingBar*>(UiManager::Singleton().GetWidget("ProgressBar2"));

    m_pProgressLabel = DynamicCast<Text*>(UiManager::Singleton().GetWidget("Label_Progress"));
    if (m_pProgressLabel)
    {
        std::string value("0");
        m_pProgressLabel->setString(value);
    }
    // Hurt Effect
    m_pHurtSprite = Sprite::create();
    m_pHurtSprite->setPosition(VisibleRect::center());
    m_pHurtSprite->setVisible(false);
    m_pHurtSprite->setScaleX(1136.0f / 256.0f * Director::getInstance()->getContentScaleFactor());
    m_pHurtSprite->setScaleY(640.0f / 192.0f * Director::getInstance()->getContentScaleFactor());

    addChild(m_pHurtSprite,20);
    Animation* pAnim = Animation::create();
    char str[20];
    for (unsigned i = 0; i <= 5; ++i)
    {
        sprintf(str,"Hurt_%d.png",i);
        pAnim->addSpriteFrameWithFile(str);
    }

    pAnim->setDelayPerUnit(0.04f);
    pAnim->setRestoreOriginalFrame(true);
    m_hurtEffectAction = RepeatForever::create(Animate::create(pAnim));
    m_hurtEffectAction->retain();
    
    // Rain Effect
    Sprite* pRainSprite = Sprite::create();
    addChild(pRainSprite, 21);
    ParticleSystem* m_emitter = CCParticleRain::create();
    pRainSprite->addChild(m_emitter, 10);
    
    Vec2 p = m_emitter->getPosition();
    m_emitter->setPosition( Vec2( p.x, p.y-100) );
    m_emitter->setLife(4);
    
    m_emitter->setTexture( Director::getInstance()->getTextureCache()->addImage("rain.png") );
    
    // Event Time (Bonus Time) Text
    m_pBonusTimeSprite = Sprite::createWithSpriteFrameName("Warning0.png");
    m_pBonusTimeSprite->setVisible(false);
    m_pBonusTimeSprite->setPosition(VisibleRect::center() + Vec2(0,100));
    m_pBonusTimeSprite->setScale(Director::getInstance()->getContentScaleFactor()*4);
    addChild(m_pBonusTimeSprite,10);
    m_eventText = CCLabelTTF::create("", "Arial", 32);
    m_eventText->setPosition(VisibleRect::center() + Vec2(0,100));
    m_eventText->setVisible(false);
    addChild(m_eventText,10);
}

void BackgroundLogic::LoadMap()
{
    GameData& gameData = GameData::Singleton();
    unsigned size = gameData.GetSize(eGDS_Map);
    
    for (unsigned i = 0; i < size; ++i)
    {
        SectionPattern pattern;

        pattern.m_imageName = gameData.GetValue(eGDS_Map, i, "Image");
        
        // Monster Pattern Data
        std::string monsterPatternStr = gameData.GetValue(eGDS_Map, i, "MonsterPattern");
        if (monsterPatternStr != "" && monsterPatternStr != "null")
        {
            std::vector<std::string> monsterPatternVec = SplitString(monsterPatternStr, ";");
            for (unsigned j = 0; j < monsterPatternVec.size(); ++j)
            {
                unsigned groupId = GetMonsterGroupId();
                std::vector<std::string> monsterPatternInfo = SplitString(monsterPatternVec[j], ":");
                unsigned patternId = atoi(monsterPatternInfo[0].c_str());
                std::vector<std::string> monsterPatternPosStr = SplitString(monsterPatternInfo[1], ",");
                Vec2 monsterPatternPos(atof(monsterPatternPosStr[0].c_str()), atof(monsterPatternPosStr[1].c_str()));

                std::string monsterStr = gameData.GetValue(eGDS_MonsterPattern, patternId, "Monster");
                XAssert(monsterStr != "" && monsterStr != "null", "");

                std::vector<std::string> monsterVec = SplitString(monsterStr, ";");
                for (unsigned k = 0; k < monsterVec.size(); ++k)
                {
                    SectionPattern::MonsterData monsterData;
                    std::vector<std::string> monsterInfo = SplitString(monsterVec[k], ":");
                    monsterData.variationId = atoi(monsterInfo[0].c_str());
                    monsterData.type = (EMonsterType)((unsigned)(gameData.GetValueF(eGDS_Monster, monsterData.variationId, "AI")));
                    monsterData.groupId = groupId;
                    std::vector<std::string> monsterPos = SplitString(monsterInfo[1], ",");
                    monsterData.pos.x = atof(monsterPos[0].c_str()) + monsterPatternPos.x;
                    monsterData.pos.y = atof(monsterPos[1].c_str()) + monsterPatternPos.y;

                    pattern.m_monsterVec.push_back(monsterData);
                }
            }
        }

        // Gpe Data
        if ((GameData::Singleton().GetValue(eGDS_Config, 0, "EnableGroundGpe") == "Yes"))
        {
            std::string gpeStr = gameData.GetValue(eGDS_Map, i, "Gpe");
            // Sample: 0:200.2; 0:100
            if (gpeStr != "" && gpeStr != "null")
            {
                std::vector<std::string> gpeVec = SplitString(gpeStr, ";");
                for (unsigned j = 0; j < gpeVec.size(); ++j)
                {
                    SectionPattern::GpeData gpeData;
                    std::vector<std::string> gpeInfo = SplitString(gpeVec[j], ":");
                    gpeData.type = atoi(gpeInfo [0].c_str());

                    std::vector<std::string> gpePos = SplitString(gpeInfo[1], ",");
                    gpeData.pos.x = atof(gpePos[0].c_str());
                    gpeData.pos.y = atof(gpePos[1].c_str());

                    pattern.m_gpeVec.push_back(gpeData);
                }
            }
        }
        
        // Item Data
        std::string itemPatternStr = gameData.GetValue(eGDS_Map, i, "ItemPattern");
        bool hasItemPattern = (itemPatternStr != "" && itemPatternStr != "null");
        if (hasItemPattern)
        {
            std::vector<std::string> itemPatternVec = SplitString(itemPatternStr, ";");
            for (unsigned j = 0; j < itemPatternVec.size(); ++j)
            {
                std::vector<std::string> itemPatternInfo = SplitString(itemPatternVec[j], ":");
                unsigned patternId = atoi(itemPatternInfo[0].c_str());
                std::vector<std::string> itemPatternPosStr = SplitString(itemPatternInfo[1], ",");
                Vec2 itemPatternPos(atof(itemPatternPosStr[0].c_str()), atof(itemPatternPosStr[1].c_str()));

                std::string itemStr = gameData.GetValue(eGDS_ItemPattern, patternId, "Item");
                XAssert(itemStr != "" && itemStr != "null", "");
                std::vector<std::string> itemVec = SplitString(itemStr, ";");
                for (unsigned k = 0; k < itemVec.size(); ++k)
                {
                    SectionPattern::ItemData itemData;
                    std::vector<std::string> itemInfo = SplitString(itemVec[k], ":");
                    //unsigned itemType = atoi(itemVec[0].c_str());
                    std::vector<std::string> itemPos = SplitString(itemInfo[1], ",");
                    itemData.pos.x = atof(itemPos[0].c_str()) + itemPatternPos.x;
                    itemData.pos.y = atof(itemPos[1].c_str()) + itemPatternPos.y;

                    pattern.m_itemVec.push_back(itemData);
                }
                
            }
        }

        m_sectionPatternVec.push_back(pattern);

        // Load Background
        std::string name = gameData.GetValue(eGDS_Map, i, "LongBackground");
        if (name != "null" && name != "")
        {
            SectionPattern bgPattern;
            bgPattern.m_imageName = name;
            m_backgroundPatternVec.push_back(bgPattern);
        }

        // Event
        std::string eventInfoStr = gameData.GetValue(eGDS_Map, i, "Event");
        if (eventInfoStr != "" && eventInfoStr != "null")
        {
            std::vector<std::string> eventInfoVec = SplitString(eventInfoStr, ":");
            XAssert(eventInfoVec.size() == 2, "Event Info is invalid.");
            float length = atoi(eventInfoVec[1].c_str());

            XAssert(i > 1, "Event Info Index is invalid.");
            length += ((i-1) * VisibleRect::right().x);

            m_eventMapIndexVec.push_back(std::pair<unsigned,float>(i, length));
        }
        
    }
}

void BackgroundLogic::onExit()
{
    Node::onExit();
}

unsigned BackgroundLogic::getBossHpPercentage(GameObject*& pGameObject)
{
    unsigned hpPercentage = 0;
    TGameObjectList objectList;
    GameObjectManager::Singleton().GetGameObjectList(eGOT_Monster, objectList);
    auto iter = std::find_if(objectList.begin(), objectList.end(), [=](GameObject* pGameObject)
                                                       {
                                                           Monster* pMonster = static_cast<Monster*>(pGameObject);
                                                           return pMonster->GetType() == eMT_Emperor;
                                                       });
    if (iter != objectList.end())
    {
        pGameObject = *iter;
        hpPercentage = ((float)pGameObject->GetHp() / pGameObject->GetMaxHp()) * 100;
    }

    return hpPercentage;
}

void BackgroundLogic::StateUpdate( float deltaTime )
{
    m_deltaTime = deltaTime;
    
    GetFsm().Update();

    CheckShakeCamera();

    // Progress bar
    GameObject* pBoss = nullptr;
    unsigned progress = getBossHpPercentage(pBoss);
    if (!pBoss)
    {
        progress = GetProgressToNextDash();
        m_progressBar->setPercent(progress);
    }
    else
    {
        m_progressBar2->setPercent(progress);
    }

    if (m_progressBar->isVisible() == (pBoss!=nullptr))
        m_progressBar->setVisible(!pBoss);
    if (m_progressBar2->isVisible() != (pBoss!=nullptr))
        m_progressBar2->setVisible(pBoss);

    if (m_pProgressLabel)
    {
        char stringValue[20];
        if (pBoss)
        {
            sprintf(stringValue, "Boss HP: %d", pBoss->GetHp());
        }
        else
        {
            sprintf(stringValue, "%d%%", progress);
        }
        
        m_pProgressLabel->setString(stringValue);
    }
    
    // HP effect
    if (m_pHurtSprite->isVisible())
    {
        m_hurtEffectDuration -= deltaTime;
        if (m_hurtEffectDuration < 0)
        {
            m_pHurtSprite->stopAction(m_hurtEffectAction);
            m_pHurtSprite->setVisible(false);
        }
    }

    // Score Bonus each sec
    UpdateTimeScore();
}

float BackgroundLogic::GetFlySpeedX()
{
    return MainPlayerLogic::Singleton().GetMC()->GetSpeedX();
}

void BackgroundLogic::RemoveSection(SectionPattern& pattern)
{
    removeChild(pattern.m_pBackgroundNode);
}

Node* BackgroundLogic::GetCurrentSectionNode()
{
    return m_sectionPatternVec[m_currentSection].m_pBackgroundNode;
}

Node* BackgroundLogic::GetNextSectionNode()
{
    return m_sectionPatternVec[GetNextPatternIndex(m_sectionPatternVec, m_currentSection)].m_pBackgroundNode;
}

void BackgroundLogic::CreateSection(SectionPattern& pattern, Vec2 point, unsigned depth, bool firstPattern)
{
    // CreateNode
    pattern.m_pBackgroundNode = Node::create();
    
    pattern.m_pBackgroundNode->setPosition(point);

    addChild(pattern.m_pBackgroundNode, depth);

    // Create Sprite on node
    Sprite* pSprite = Sprite::createWithSpriteFrameName(pattern.m_imageName.c_str());
    pSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
    pSprite->setPosition(VisibleRect::leftBottom());
    pSprite->setScale(Director::getInstance()->getContentScaleFactor());
    pattern.m_pBackgroundNode->addChild(pSprite, 0);

    
    if (!firstPattern)
    {
        // Create Monster on node
        for (unsigned i = 0; i < pattern.m_monsterVec.size(); ++i)
        {
            Monster* pMonster = GameObjectFactory::Singleton().CreateMonster(
                pattern.m_pBackgroundNode,
                pattern.m_monsterVec[i].type, 
                pattern.m_monsterVec[i].variationId,
                pattern.m_monsterVec[i].groupId
                );

            pMonster->setPosition(pattern.m_monsterVec[i].pos);
            //pattern.m_pBackgroundNode->addChild(pMonster, 3);
        }

        // Create Gpe on node
        for (unsigned i = 0; i < pattern.m_gpeVec.size(); ++i)
        {
            Gpe* pGpe = NULL;
            switch (pattern.m_gpeVec[i].type)
            {
            case 0:
                {
                    pGpe = new EnergyStation();
                    pGpe->autorelease();
                }
                break;
            default:
                break;
            }
            pGpe->setPosition(pattern.m_gpeVec[i].pos);
            pattern.m_pBackgroundNode->addChild(pGpe);
        }

        // Create Item on node
        for (unsigned i = 0; i < pattern.m_itemVec.size(); ++i)
        {
            Gold* pItem = new Gold((Gold::EGoldType)(pattern.m_itemVec[i].type));
            pItem->autorelease();
            pItem->setPosition(pattern.m_itemVec[i].pos);
            pattern.m_pBackgroundNode->addChild(pItem);
        }
    }
}

void BackgroundLogic::SetMoveStatus(EBackgroundMoveStatus status)
{
    m_moveStatus = status;
}

unsigned BackgroundLogic::GetNextPatternIndex(const std::vector<SectionPattern>& patternVec, unsigned currentIndex)
{
    if (currentIndex < patternVec.size() - 1)
        return ++currentIndex;
    else
        return 0;
}

void BackgroundLogic::Move(float speed)
{
    m_sectionPatternVec[m_currentSection].m_pBackgroundNode->setPositionX(m_sectionPatternVec[m_currentSection].m_pBackgroundNode->getPositionX() - speed * m_deltaTime);
    m_sectionPatternVec[GetNextPatternIndex(m_sectionPatternVec, m_currentSection)].m_pBackgroundNode->setPositionX(m_sectionPatternVec[GetNextPatternIndex(m_sectionPatternVec, m_currentSection)].m_pBackgroundNode->getPositionX() - speed * m_deltaTime);

    if (m_sectionPatternVec[m_currentSection].m_pBackgroundNode->getPositionX() < -VisibleRect::right().x)
    {
        RemoveSection(m_sectionPatternVec[m_currentSection]);
        m_currentSection = GetNextPatternIndex(m_sectionPatternVec, m_currentSection);

        float currentSectionPos = m_sectionPatternVec[m_currentSection].m_pBackgroundNode->getPositionX();
        CreateSection(m_sectionPatternVec[GetNextPatternIndex(m_sectionPatternVec, m_currentSection)], Vec2(currentSectionPos + VisibleRect::right().x, 0.0f), 1);
    }

    //background
    float rate = GameData::Singleton().GetValueF(eGDS_Config, 0, "BackLayerSpeedRate");
    m_backgroundPatternVec[m_currentBackground].m_pBackgroundNode->setPositionX(m_backgroundPatternVec[m_currentBackground].m_pBackgroundNode->getPositionX() - speed * m_deltaTime * rate);
    m_backgroundPatternVec[GetNextPatternIndex(m_backgroundPatternVec, m_currentBackground)].m_pBackgroundNode->setPositionX(m_backgroundPatternVec[GetNextPatternIndex(m_backgroundPatternVec, m_currentBackground)].m_pBackgroundNode->getPositionX() - speed * m_deltaTime * rate);

    if (m_backgroundPatternVec[m_currentBackground].m_pBackgroundNode->getPositionX() < -VisibleRect::right().x)
    {
        RemoveSection(m_backgroundPatternVec[m_currentBackground]);
        m_currentBackground = GetNextPatternIndex(m_backgroundPatternVec, m_currentBackground);

        float currentSectionPos = m_backgroundPatternVec[m_currentBackground].m_pBackgroundNode->getPositionX();
        CreateSection(m_backgroundPatternVec[GetNextPatternIndex(m_backgroundPatternVec, m_currentBackground)], Vec2(currentSectionPos + VisibleRect::right().x, 0.0f), 0);
    }
    
}

IMPLEMENT_STATE_BEGIN(BackgroundLogic, Idle)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        CreateSection(m_sectionPatternVec[m_currentSection], Vec2(), 1, true);
        CreateSection(m_sectionPatternVec[GetNextPatternIndex(m_sectionPatternVec, m_currentSection)], Vec2(VisibleRect::right().x, -3.0f), 1);

        //create background
        CreateSection(m_backgroundPatternVec[m_currentSection], Vec2(), 0, true);
        CreateSection(m_backgroundPatternVec[GetNextPatternIndex(m_backgroundPatternVec, m_currentSection)], Vec2(VisibleRect::right().x, -3.0f), 0);
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        TRANSIT_TO_STATE( true, NoTransitionAction, Move );
    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(BackgroundLogic, Move)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_moveStatus = eBMS_Normal;
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {      
        m_currentSpeed = GetFlySpeedX();
        Move(m_currentSpeed);
        
        TRANSIT_TO_STATE( m_moveStatus == eBMS_Stop, NoTransitionAction, Stop );

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

    IMPLEMENT_STATE_BEGIN(BackgroundLogic, Stop)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_moveStatus = eBMS_Stop;
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        m_currentSpeed = 0.0f;
        Move(m_currentSpeed);
        TRANSIT_TO_STATE( m_moveStatus == eBMS_Normal, NoTransitionAction, Move );
        TRANSIT_TO_STATE( m_moveStatus == eBMS_Dash, NoTransitionAction, Dash );

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

IMPLEMENT_STATE_BEGIN(BackgroundLogic, Dash)
{
    STATE_CONSTRUCTOR_BEGIN
    {
        m_moveStatus = eBMS_Dash;
        m_dashTimer = m_dashTime;

        SoundManager::Singleton().PlayEffect("Sound/Effect_RefuelDash.mp3");

        Waves3D* pWaves3dAction = Waves3D::create(m_dashTime, CCSize(15,10), 5, 40);
        pWaves3dAction->setTag(8668);
        runAction(pWaves3dAction);
    }
    STATE_CONSTRUCTOR_END

        STATE_UPDATE_BEGIN
    {
        m_dashTimer -= m_deltaTime;
        if (m_dashTimer > 0)
        {
            m_currentSpeed = m_dashSpeed;
            Move(m_currentSpeed);
        }
        else
            TRANSIT_TO_STATE( true, NoTransitionAction, Move );

        TRANSIT_TO_STATE( m_moveStatus == eBMS_Stop, NoTransitionAction, Stop );

    }
    STATE_UPDATE_END

        STATE_DESTRUCTOR_BEGIN
    {
        MainPlayerLogic::Singleton().GetMC()->AccelerateSpeedX();

        stopActionByTag(8668);
        setGrid(NULL);
    }
    STATE_DESTRUCTOR_END
}
IMPLEMENT_STATE_END

void BackgroundLogic::ShakeCamera( float duration )
{
    duration = 1.0f;
    m_shadeCameraDuration = duration;

    if (m_pShakeCameraAction)
    {
        stopAction(m_pShakeCameraAction);
        setPosition(Vec2());
        m_pShakeCameraAction = NULL; 
    }

    m_pShakeCameraAction = runAction(RepeatForever::create(
        Sequence::create( CCMoveBy::create(0.05f, Vec2(10,2)), CCMoveBy::create(0.05f, Vec2(-10,-2)),NULL)
    ));
    
    SoundManager::Singleton().vibrate();
}

void BackgroundLogic::CheckShakeCamera()
{
    if (m_pShakeCameraAction)
    {
        m_shadeCameraDuration -= m_deltaTime;
        if (m_shadeCameraDuration < 0)
        {
            stopAction(m_pShakeCameraAction);
            setPosition(Vec2());
            m_pShakeCameraAction = NULL; 
        }
    }
}

float BackgroundLogic::GetCurrentSpeed() const
{
    return m_currentSpeed;
}

unsigned BackgroundLogic::GetProgressToNextDash()
{
    int result = 0;
    float progress = 0.0f;

    for (unsigned i = 0; i < m_eventMapIndexVec.size(); ++i)
    {
        Node* currentNode = GetCurrentSectionNode();
        float currentOffset = m_currentSection * VisibleRect::right().x - currentNode->getPositionX();
        if (m_eventMapIndexVec[i].second > currentOffset)
        {
            float lastEventOffset = i > 0 ? m_eventMapIndexVec[i-1].second : 0;
            progress = (currentOffset - lastEventOffset)/(m_eventMapIndexVec[i].second - lastEventOffset);
            break;
        }
//         else if(m_currentSection < m_eventMapIndexVec[i].first)
//         {
//             progress = 100.0f;
//             break;
//         }
    }

    result = progress * 100 + 1;

    if (result < 0)
        result = 0;
    if (result > 100)
        result = 100;

    return (unsigned)result ;
}

void BackgroundLogic::StartHurtEffect(float duration)
{
    m_hurtEffectDuration = duration;
    if (!m_pHurtSprite->isVisible())
    {
        m_pHurtSprite->setVisible(true);
        m_pHurtSprite->runAction(m_hurtEffectAction);
    }
}

void BackgroundLogic::ShowEventTime(std::string text)
{
    FadeOutUpTiles* fadeout = FadeOutUpTiles::create(0.5f, Size(16,12));

    ActionInterval* back = fadeout->reverse();
    ActionInterval* pEffect = Sequence::create(fadeout, back, NULL);
    runAction(pEffect);

    m_pBonusTimeSprite->setVisible(true);
    
    Animation* pAnim = Animation::create();

    char str[20];
    for (unsigned i = 0; i < 4; ++i)
    {
        sprintf(str,"Warning%d.png", i);
        SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        pAnim->addSpriteFrame(frame);
    }
    pAnim->setDelayPerUnit(0.5f);
    pAnim->setRestoreOriginalFrame(true);

    ActionInterval* pAction = Animate::create(pAnim);
    pAction->setDuration(2.0f);
    DelayTime* delay = DelayTime::create(0.5f);
    pAction = Sequence::create(delay, pAction, CallFunc::create(std::bind(&BackgroundLogic::ShowBonusTimeAnimationFinished, this)), NULL);

    m_pBonusTimeSprite->runAction(pAction);

    m_eventText->setVisible(true);
    m_eventText->setString(text.c_str());
}

void BackgroundLogic::ShowBonusTimeAnimationFinished()
{
    m_pBonusTimeSprite->setVisible(false);
    m_eventText->setVisible(false);
}

void BackgroundLogic::UpdateTimeScore()
{
    m_addScoreTimer -= m_deltaTime;
    if (m_addScoreTimer < 0.0f)
    {
        unsigned addScore = (m_moveStatus == eBMS_Dash) ? 3 : 1;
        MainPlayerLogic::Singleton().IncreaseScore(addScore);
        m_addScoreTimer = 1.0f;
    }

}
