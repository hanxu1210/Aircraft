#ifndef  _APP_MAINPLAYERLOGIC_H_
#define  _APP_MAINPLAYERLOGIC_H_

USING_NS_CC_EXT;

class MainCharacter;
class FireBall;
NS_CC_BEGIN 
    namespace ui{class Layout; class ListView;}
NS_CC_END
    USING_NS_CC;
#include "ui/CocosGUI.h"
using namespace ui;
#include "CryptoData.h"

class MainPlayerLogic : public Node
{
    DECLARE_SINGLETON(MainPlayerLogic);

public:
    void Init(Layer* layer);
    void Uninit();
    void Reset();

    void ReduceCoin(int reduceCoin);
    void IncreaseCoin(int increaseCoin);

    void ReduceScore(int reduceScore);
    void IncreaseScore(int increaseScore);

    MainCharacter* GetMC();
    void DrawEnergyValue();

private:
    void DrawCoinValue();
    void DrawScoreValue(int animationType);

    LoadingBar* m_pHpBar;
    TextAtlas* m_pCoinNumLableAtlas;
    Text* m_pScoreLabel;

    CryptoData<unsigned> m_originalHp;
    CryptoData<unsigned> m_currentHp;

    CryptoData<unsigned> m_originalCoin;
    CryptoData<unsigned> m_currentCoin;

    Layer* m_parent;
    MainCharacter* m_pMC;
};

#endif //_APP_GAMEOBJECTMANAGER_H_