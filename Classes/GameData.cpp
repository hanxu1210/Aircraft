#include "CommonHeaders.h"

#include "GameData.h"

IMPLEMENT_SINGLETON(GameData);

GameData::GameData()
    : m_pMCJson(NULL)
    , m_pMonsterJson(NULL)
    , m_pMapJson(NULL)
    , m_pBulletJson(NULL)
    , m_pGpeJson(NULL)
    , m_pItemPatternJson(NULL)
    , m_pSkillJson(NULL)
    , m_pCnofigJson(NULL)
    , m_pBossJson(NULL)
    , m_pMonsterPatternJson(NULL)
    , m_pBonusJson(NULL)
    , m_currentScore(0)
    , m_mcExtraAttackPointVec(0)
    , m_mcExtraHpVec(0)
    , m_currentCoin(500)
    , m_userName("test")
    , m_mcIndex(0)
{
    m_mcExtraAttackPointVec.resize(eMC_Count);
    m_mcExtraHpVec.resize(eMC_Count);

    ssize_t size = 0;

    std::string tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_MC.json");
    const char* pMCBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pMCJson = cJSON_Parse(pMCBuffer);
    
    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Monster.json");
    const char* pMonsterBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pMonsterJson = cJSON_Parse(pMonsterBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Map.json");
    const char* pMapBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pMapJson = cJSON_Parse(pMapBuffer);
    
    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Bullet.json");
    const char* pBulletBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pBulletJson = cJSON_Parse(pBulletBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Gpe.json");
    const char* pGpeBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pGpeJson= cJSON_Parse(pGpeBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_ItemPattern.json");
    const char* pItemPatternBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pItemPatternJson = cJSON_Parse(pItemPatternBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Skill.json");
    const char* pSkillBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pSkillJson = cJSON_Parse(pSkillBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Config.json");
    const char* pConfigBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pCnofigJson  = cJSON_Parse(pConfigBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Boss.json");
    const char* pBossBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pBossJson  = cJSON_Parse(pBossBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_MonsterPattern.json");
    const char* pMonsterPatternBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pMonsterPatternJson = cJSON_Parse(pMonsterPatternBuffer);

    tFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("gamedata_Bonus.json");
    const char* pBonusBuffer = (char*)(cocos2d::FileUtils::getInstance()->getFileData(tFullPath.c_str(),"r" , &size)); 
    m_pBonusJson = cJSON_Parse(pBonusBuffer);
}

GameData::~GameData()
{
    if (m_pMCJson)
        cJSON_Delete(m_pMCJson);
    if (m_pMonsterJson)
        cJSON_Delete(m_pMonsterJson);
    if (m_pMapJson)
        cJSON_Delete(m_pMapJson);
    if (m_pBulletJson)
        cJSON_Delete(m_pBulletJson);
    if (m_pGpeJson)
        cJSON_Delete(m_pGpeJson);
    if (m_pItemPatternJson)
        cJSON_Delete(m_pItemPatternJson);
    if (m_pSkillJson)
        cJSON_Delete(m_pSkillJson);
    if (m_pCnofigJson)
        cJSON_Delete(m_pCnofigJson);
    if (m_pBossJson)
        cJSON_Delete(m_pBossJson); 
    if (m_pMonsterPatternJson)
        cJSON_Delete(m_pMonsterPatternJson); 
    if (m_pBonusJson)
        cJSON_Delete(m_pBonusJson );
}

unsigned GameData::GetSize( EGameDataSheet sheet )
{
    cJSON* pJsonRoot = GetJsonByType(sheet);

    return cJSON_GetArraySize(pJsonRoot);
}

cJSON* GameData::GetJsonByType( EGameDataSheet sheet )
{
    cJSON* pJsonRoot = NULL;

    switch (sheet)
    {
    case eGDS_MC:
        pJsonRoot = m_pMCJson;
        break;
    case eGDS_Monster:
        pJsonRoot = m_pMonsterJson;
        break;
    case eGDS_Map:
        pJsonRoot = m_pMapJson;
        break;
    case eGDS_Bullet:
        pJsonRoot = m_pBulletJson;
        break;
    case eGDS_Gpe:
        pJsonRoot = m_pGpeJson;
        break;
    case eGDS_ItemPattern:
        pJsonRoot = m_pItemPatternJson;
        break;
    case eGDS_Skill:
        pJsonRoot = m_pSkillJson;
        break;
    case eGDS_Config:
        pJsonRoot = m_pCnofigJson;
        break;
    case eGDS_Boss:
        pJsonRoot = m_pBossJson;
        break;
    case eGDS_MonsterPattern:
        pJsonRoot = m_pMonsterPatternJson;
        break;
    case eGDS_Bonus:
        pJsonRoot = m_pBonusJson;
        break;
    default:
        XAssert(false, "Unexpected switch branch.");
        break;
    }

    return pJsonRoot;
}

float GameData::GetValueF( EGameDataSheet sheet, unsigned index, std::string key )
{
    cJSON* pJsonRoot = GetJsonByType(sheet);
    
    cJSON* pStructJson = cJSON_GetArrayItem(pJsonRoot, index);
    if (!pStructJson)
        return 0.0f;
    
    cJSON* pValueJson = cJSON_GetObjectItem(pStructJson, key.c_str());
    if (!pValueJson)
        return 0.0f;
    
    return pValueJson->valuedouble;
}

std::string GameData::GetValue(EGameDataSheet sheet, unsigned index, std::string key)
{
    cJSON* pJsonRoot = GetJsonByType(sheet);
    
    cJSON* pStructJson = cJSON_GetArrayItem(pJsonRoot, index);
    if (!pStructJson)
        return "";
    
    cJSON* pValueJson = cJSON_GetObjectItem(pStructJson, key.c_str());
    if (!pValueJson)
        return "";
    
    return pValueJson->valuestring;
}

bool GameData::ReduceCoin( int reduceEnergy )
{
    if (m_currentCoin != 0)
    {
        m_currentCoin -= reduceEnergy;
        clampf(m_currentCoin, 0, 100);
        return true;
    }

    return false;
}

void GameData::ReduceScore(int reduceScore)
{
    if (m_currentScore > 0)
    {
        m_currentScore -= reduceScore;
    }
}
void GameData::IncreaseScore(int increaseScore)
{
    m_currentScore += increaseScore;
}

