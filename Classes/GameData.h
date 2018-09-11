#ifndef  _APP_GAMEDATA_H_
#define  _APP_GAMEDATA_H_

USING_NS_CC_EXT;

enum EMcId
{   
    eMC_Titan,
    eMC_Storm,
    eMC_Charm,

    eMC_Count
};

enum EGameDataSheet
{
    eGDS_MC,
    eGDS_Monster,
    eGDS_Map,
    eGDS_Bullet,
    eGDS_Gpe,
    eGDS_ItemPattern,
    eGDS_Skill,
    eGDS_Config,
    eGDS_Boss,
    eGDS_MonsterPattern,
    eGDS_Bonus
};

class GameData
{
    DECLARE_SINGLETON(GameData);
    
public:
    std::string GetValue(EGameDataSheet sheet, unsigned index, std::string key);
    float GetValueF  (EGameDataSheet sheet, unsigned index, std::string key);
    /*template<>
    float GetValue(EGameDataSheet sheet, unsigned index, std::string key);

    template<>
    std::string GetValue(EGameDataSheet sheet, unsigned index, std::string key);
*/
    unsigned GetSize(EGameDataSheet sheet);

    void ReduceScore(int reduceScore);
    void IncreaseScore(int increaseScore);
    
    bool ReduceCoin(int reduceEnergy);

    int m_currentScore;
    std::vector<int> m_mcExtraAttackPointVec;
    std::vector<int> m_mcExtraHpVec;
    int m_currentCoin;
    std::string m_userName;
    int m_mcIndex;
    std::vector<int> m_bonusIndexVec;

private:
    cJSON* GetJsonByType(EGameDataSheet sheet);

private:
    cJSON* m_pMCJson;
    cJSON* m_pMonsterJson;
    cJSON* m_pMapJson;
    cJSON* m_pBulletJson;
    cJSON* m_pGpeJson;
    cJSON* m_pItemPatternJson;
    cJSON* m_pSkillJson;
    cJSON* m_pCnofigJson;
    cJSON* m_pBossJson;
    cJSON* m_pMonsterPatternJson;
    cJSON* m_pBonusJson;
};

#include "GameData.hpp"

#endif // _APP_GAMEDATA_H_