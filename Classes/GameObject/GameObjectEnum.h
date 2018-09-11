#ifndef  _APP_GameObjectEnum_H_
#define  _APP_GameObjectEnum_H_

enum EGameObjectType
{
    eGOT_MainCharacter,
    eGOT_Monster,
    eGOT_FireBall,
    eGOT_Item,
    eGOT_General,
    eGOT_MagicMatrix,
    eGOT_Bullet,
    eGOT_Gpe,

    eGOT_Count,
    eGOT_Invalid
};
enum EFactionType
{
    eFT_Good,
    eFT_Bad,

    eFT_Count
};

enum EMonsterType
{
    eMT_DashPlane,
    eMT_TurnBackPlane,
    eMT_StreightPlane,
    eMT_StayPlane,
    eMT_EmperorGhost,
    eMT_Emperor,

    eMT_Count,
    eMT_Invalid
};

enum EGpeType
{
    eGT_EnergyStation,
    eGT_Building1,
    eGT_Building2
};

enum EGeneralType
{
    eGT_OptimusPrime,
    eGT_Hornet,

    eGT_Count,
    eGT_Invalid
};

enum EItemType
{
    eIT_TimeFreezeItem,
    eIT_Gold,
    eIT_Bomb,

    eIT_Count,
    eIT_Invalid
};

enum EBulletType
{
    eBT_Normal,
    eBT_Laser,
    eBT_BossLaser,
    eBT_TitanNormal,
    eBT_StormNormal,
    eBT_CharmNormal
};

#endif //_APP_GameObjectEnum_H_