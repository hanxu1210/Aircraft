#include "CommonHeaders.h"

#include "GameObject/Generals/GeneralFactory/GeneralFactory.h"
#include "GameObject/Generals/Hornet/Hornet.h"
#include "GameObject/Generals/OptimusPrime/OptimusPrime.h"
#include "GameLogic/GeneralGroupLogic.h"

GeneralFactory::GeneralFactory()
{

}

GeneralFactory::~GeneralFactory()
{

}

void GeneralFactory::CreateGeneral( Node* parent, EGeneralType type )
{
    switch (type)
    {
    case eGT_Hornet:
        {
            Hornet* pHornet = new Hornet();
            pHornet->autorelease();
            parent->addChild(pHornet);
            pHornet->SetGeneralGroupLogic(DynamicCast<GeneralGroupLogic*>(parent));
        }
        break;
    case eGT_OptimusPrime:
        {
            OptimusPrime* pOptimusPrime = new OptimusPrime();
            pOptimusPrime->autorelease();
            parent->addChild(pOptimusPrime);
            pOptimusPrime->SetGeneralGroupLogic(DynamicCast<GeneralGroupLogic*>(parent));
        }
        break;
    default:
        break;
    }
}