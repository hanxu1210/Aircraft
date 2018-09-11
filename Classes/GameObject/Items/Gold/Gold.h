#ifndef  _APP_Gold_H_
#define  _APP_Gold_H_

#include "GameObject/Items/Item.h"

USING_NS_CC;

class Gold : public Item
{
public:
    enum EGoldType
    {
        eGT_Small,
        eGT_Big
    };

    Gold(EGoldType type);
    virtual ~Gold();

    virtual void onEnter();
    virtual void onExit();

    virtual int GetDetailTypeIndex();

private:
    OVERLOAD_FSM(Gold);
    DECLARE_VIRTUAL_STATE(Move);
    DECLARE_VIRTUAL_STATE(Hit);
private:
    EGoldType m_type;
    unsigned m_coinValue;

};

#endif // _APP_Gold_H_