#include "CommonHeaders.h"

#include "GameObject/Gpe/Gpe.h"

Gpe::Gpe()
    : GameObject(10.0f, eGOT_Gpe,10.0f)
    , m_type(0)
    , m_pMainSprite(NULL)
    , m_deltaTime(0.0f)
{
}

Gpe::Gpe(unsigned type)
    : GameObject(10.0f, eGOT_Gpe,10.0f)
    , m_type(type)
    , m_pMainSprite(NULL)
    , m_deltaTime(0.0f)
{
}

Gpe::~Gpe()
{

}
